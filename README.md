# arduino-pid-dcmotor
A short and smart code for closed loop speed control of dc motor using arduino. The code also includes interrupt for measuring speed of the motor by counting pulses from an attached quadrature encoder. The motor is connected to arduino using a H-bridge.

The speed is continuously written to the serial port. New speed can be set by writing the value to the serial port in the format *xx.xx*, e.g. 3.14. Please note the speed units are rounds/sec.

A labview virtual instrument(vi) file can be used to display the motor speed on a guage. Also, the speed can be set with a knob.

### Included files:
	1. picontrol.ino
	2. speedmeasure.vi
