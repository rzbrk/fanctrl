#include <OneWire.h> 
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//Array for temperature readings
#define ARRAY_LEN 10
float TEMP_ARRAY[ARRAY_LEN];

//Temperature range (degrees celsius) in between the fan speed is modulated
#define TEMP_DUTY_MIN 25.
#define TEMP_DUTY_MAX 40.
#define DUTY_MIN 0.
#define DUTY_MAX 1.

//Configure Timer 2 (pin 3) to output 25kHz PWM. Pin 11 will be unavailable for output in this mode
void setupTimer2(){
  //Set PWM frequency to about 25khz on pin 3 (timer 2 mode 5, prescale 8, count to 79)
  TIMSK2 = 0;
  TIFR2 = 0;
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << WGM22) | (1 << CS21);
  OCR2A = 79;
  OCR2B = 0;
}

//Equivalent of analogWrite on pin 3
void setPWM2(float f){
    f=f<0?0:f>1?1:f;
    OCR2B = (uint8_t)(79*f);
}

// Update temperature array
void updateTempArr(float temp) {
  for (int i = ARRAY_LEN -1; i >= 1; i--) {
    TEMP_ARRAY[i] = TEMP_ARRAY[i-1];
  }
  TEMP_ARRAY[0] = temp;
}

// Output temperature array
void outputTempArr() {
  Serial.print("[");
  for (int i = 0; i < ARRAY_LEN; i++) {
    Serial.print(TEMP_ARRAY[i]);
    if (i != ARRAY_LEN -1) {
      Serial.print(", ");
    } else {
      Serial.print("]\n");
    }
  }
}

// Get mean value of temperature array
float meanTempArr() {
  float temp = 0;
  for (int i = 0; i < ARRAY_LEN; i++) {
    temp = temp + TEMP_ARRAY[i];
  }
  return temp / ARRAY_LEN;
}

void setup() {
  // start serial port 
  Serial.begin(9600); 
  // Start up the library 
  sensors.begin();

  //enable outputs for Timer 2
  pinMode(3,OUTPUT); //2
  setupTimer2();
}

void loop() {
  // Call sensors.requestTemperatures() to issue a global temperature 
  // Request to all devices on the bus 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  //Serial.print(sensors.getTempCByIndex(0));
  float temp = float(sensors.getTempCByIndex(0));
  updateTempArr(temp);
  Serial.println("");
  outputTempArr();
  Serial.println("Mean temp: " + String(meanTempArr()));

  // Calculate PWM from temp reading
  float duty;
  if (temp <= TEMP_DUTY_MIN) {
    duty = 0.0f;
  } else if (temp >= TEMP_DUTY_MAX) {
    duty = 1.0f;
  } else {
    duty = (temp - TEMP_DUTY_MIN) / (TEMP_DUTY_MAX - TEMP_DUTY_MIN);
  }
  Serial.println("Fan duty cycle: " + String(duty));

  // Set speed of fan
  setPWM2(duty);
  
  delay(1000);
}
