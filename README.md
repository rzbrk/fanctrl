# fanctrl

fanctrl is a Arduino program to control the speed of fans as a function of the
ambient temperature. This setup is used by to support the cooling of a game
console placed in a shelf.

For the temperature reading a 1-wire digital thermometer
[DS18B20](https://www.maximintegrated.com/en/products/sensors/DS18B20.html) is
used which is connected to pin 2 of an Arduino Nano microcontroller. For the
fans models providing a 4-pin connector are used. The speed of this fans can be
controlled via a PWM signal rather by changing the supply voltage. The
[Noctua NF-R8 redux-1800 PWM](https://noctua.at/en/nf-r8-redux-1800-pwm)
fans are used. The Arduino Nano provides the PWM for the fans on pin 3.

This project heavily relies on
[this project of Frederico Dossena](https://fdossena.com/?p=ArduinoFanControl/i.md).

The microcontroller reads the temperature every seconds. The reading is stored
in a buffer while the oldest reading is deleted. The PWM duty cycle is then
calculated from the mean value of all temperature readings in the buffer
effectively acting as a low pass filter.

The following pictures show the finished device.

![Fanctrl Device, front](https://raw.githubusercontent.com/rzbrk/fanctrl/master/fanctrl_front.jpg)

![Fanctrl Device, back](https://raw.githubusercontent.com/rzbrk/fanctrl/master/fanctrl_back.jpg)
