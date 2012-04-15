/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  The pid is designed to output an analog value,
 * but the relay can only be On/Off.
 *
 *   To connect them together we use "time proportioning
 * control"  Tt's essentially a really slow version of PWM.
 * First we decide on a window size (5000mS say.) We then 
 * set the pid to adjust its output between 0 and that window
 * size.  Lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the 
 * window being "Relay Off Time"
 ********************************************************/

#include <PID_v1.h>
#define RelayPin 7

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

float outputMult = 50;
float WindowSize = 5000;
unsigned long windowStartTime;
void setup()
{
  pinMode(RelayPin, OUTPUT);
  windowStartTime = millis();

  //initialize the variables we're linked to
  // Our target temperature is 25 deg celcius
  Setpoint = 25;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  
  tempSetup();
  Serial.begin(115200); //Start serial communication at 115200 baud
}

void loop()
{
  // Get the temperature measurement
  Input = getTemp102();
  
  myPID.Compute();

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  unsigned long now = millis();
  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
    // print measurements
    Serial.print("Input = ");
    Serial.println(Input);
    Serial.print("Ouput = ");
    Serial.println(Output);
  }
  
  if(outputMult*Output > now - windowStartTime) digitalWrite(RelayPin,HIGH);
  else digitalWrite(RelayPin,LOW);
  
  

}
