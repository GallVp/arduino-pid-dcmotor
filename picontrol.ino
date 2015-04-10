
long encoderval=0;
float mspeed = 0;
int channelA = 2;
int channelB = 3;
float Ts=1000;//microsec
unsigned long prevtime=0;
long prevencoderval=0;
float Kp=10;
float Ki=20;
int en = 5;
int pina = 11;
int pinb = 13;
unsigned char pwm;
float reference =0;
float error = 0;
float controllaw=0;
float preverror=0;
float integrat=0;
float previntegrat=0;
int availablebytes=0;
char readreference[5] ={'0','0','0','0','0'};

void setup()
{
  attachInterrupt(0, encoderInt, RISING);//Interrupt 0 is on pin 2
  pinMode(channelB, INPUT);
  pinMode(en, OUTPUT); 
  pinMode(pina, OUTPUT); 
  pinMode(pinb, OUTPUT); 
  digitalWrite(pina, HIGH);
  digitalWrite(pinb, LOW);
  Serial.begin(115200);
}

void loop()
{
  availablebytes=Serial.available();
  if(availablebytes>0)
  {
    readreference[0] = 0;
    readreference[1] = 0;
    readreference[2] = 0;
    readreference[3] = 0;
    Serial.readBytes(readreference,5);
    reference = atof(readreference);
    encoderval=0;
    prevencoderval=0; 
  }
  mspeed = (float)(encoderval-prevencoderval);
  prevencoderval = encoderval;
  mspeed = mspeed*1000/(2*Ts);
  
  error= reference - mspeed;
  integrat = previntegrat + (preverror/1000000)*Ts;
  previntegrat = integrat;
  preverror = error;
  controllaw = error*Kp + Ki * integrat;
  if(controllaw>12)
  {
    controllaw=12;
  }
  if(controllaw<-12)
  {
    controllaw=-12;
  }
  if(controllaw>0)
  {
    digitalWrite(pina, HIGH);
    digitalWrite(pinb, LOW);
  }
  if(controllaw<0)
  {
    digitalWrite(pina, LOW);
    digitalWrite(pinb, HIGH);
  }
  Serial.println(mspeed);
  controllaw= abs(controllaw);
  pwm = (controllaw/12)*255;
  analogWrite(en, pwm);
  
  Ts=micros()-prevtime;
  prevtime = micros();
}

void encoderInt()
{
  if(digitalRead(channelB)==HIGH){encoderval++;}
  else if(digitalRead(channelB)==LOW){encoderval--;}
}
