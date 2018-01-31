// Pin sssignment
int channelA              = 2;      // Channel A of the encoder
int channelB              = 3;      // Channel B of the encoder
int en                    = 5;      // Enable pin of the H-bridge
int pina                  = 11;     // Input 1(a) of the H-bridge
int pinb                  = 13;     // Input 2(b) of the H-bridge
// Controller gains
float Kp                  = 10;     // Proportional gain
float Ki                  = 20;     // Integral gain
// Control law variables
float Ts                  = 1000;   // Sample time in microseconds. Recalculated on each sample.
float reference           = 0;
float error               = 0;
float controllaw          = 0;
float integrat            = 0;
// Encoder variables
long encoderval           = 0;
float mspeed              = 0;
// Previous sample data
float previntegrat        = 0;
float preverror           = 0;
unsigned long prevtime    = 0;
long prevencoderval       = 0;
// Calculated PWM
unsigned char pwm;
// Serial port variables
int availablebytes        = 0;
char readreference[5]     = {'0','0','0','0','0'};

void setup()
{
  attachInterrupt(0, encoderInt, RISING); // Interrupt 0 is on pin 2
  pinMode(channelB, INPUT);
  pinMode(en, OUTPUT); 
  pinMode(pina, OUTPUT); 
  pinMode(pinb, OUTPUT); 
  digitalWrite(pina, HIGH);
  digitalWrite(pinb, LOW);
  Serial.begin(115200); // A lower serial port speed can affect controller performance
}

void loop()
{
  availablebytes = Serial.available();
  if(availablebytes > 0)
  {
    readreference[0] = 0;
    readreference[1] = 0;
    readreference[2] = 0;
    readreference[3] = 0;
    Serial.readBytes(readreference, 5);
    reference = atof(readreference);
    encoderval = 0;
    prevencoderval = 0; 
  }
  //Measuring motor speed
  mspeed = (float)(encoderval - prevencoderval);
  prevencoderval = encoderval;
  // This encoder produces 2000 pulses per round.
  // And sample time is in micro seconds.
  // Thus, mspeed is in rounds/sec units.
  // The original formula was: mspeed = mspeed * 1000000 / (2000 * Ts)
  mspeed = mspeed * 1000 / (2 * Ts);
  
  error = reference - mspeed;

  // Dividing by 1000000 as Ts is in microseconds
  integrat = previntegrat + (preverror / 1000000) * Ts;
  previntegrat = integrat;
  preverror = error;
  controllaw = error * Kp + Ki * integrat;

  // Applying saturation operation
  if(controllaw > 12)
  {
    controllaw = 12;
  }
  if(controllaw < -12)
  {
    controllaw = -12;
  }

  // Setting direction with H-bridge
  if(controllaw > 0)
  {
    digitalWrite(pina, HIGH);
    digitalWrite(pinb, LOW);
  }
  if(controllaw < 0)
  {
    digitalWrite(pina, LOW);
    digitalWrite(pinb, HIGH);
  }

  // Updating motor speed
  Serial.println(mspeed);
  controllaw = abs(controllaw);
  pwm = (controllaw / 12) * 255;
  analogWrite(en, pwm);
  
  // Measuring sample time
  Ts = micros() - prevtime;
  prevtime = micros();
}

void encoderInt()
{
  if(digitalRead(channelB) == HIGH){encoderval++;}
  else if(digitalRead(channelB) == LOW){encoderval--;}
}
