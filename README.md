# arduino-pid-dcmotor
A short and smart code for dc motor speed control using arduino. The code also includes interrupt for measuring speed of the motor by counting pulses from attached quadrature encoder. The speed is continuously written onto the serial port. New speed can be set by writing the value to the serial port.

A labview virtual instrument(vi) file can be used to display the motor speed on a guage. Also, the speed can be from a knob.

### Included files:
	1. picontrol.ino
	2. speedmeasure.vi
