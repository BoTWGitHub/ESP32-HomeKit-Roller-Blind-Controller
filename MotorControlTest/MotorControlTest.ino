#include "ESP32Encoder.h"
#include "AM2861.h"

AM2861 am2861(0, 12, 1, 13, 5000, 12);
ESP32Encoder encoder;

unsigned long myTime;
int32_t lastTick;
float velocity;
float error_i, error_pos_i, error_pos_d, last_error_pos;

String inString = "";
int command = 0;

void setup()
{
  Serial.begin(115200);

  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachSingleEdge(4, 5);
  encoder.clearCount();
  encoder.setFilter(1023);
}

void loop()
{
  float error;
  float error_pos;
  if(millis() - myTime > 100)
  {
    int32_t ticks = (int32_t)encoder.getCount();
    float temp = ((float)ticks - (float)lastTick)*1000*60/276/100;
    velocity = velocity * 0.8 + temp * 0.2;

    Serial.print(command);
    Serial.print(" ");
    Serial.print(ticks);
    Serial.print(" ");
    Serial.print(velocity);
    Serial.print("/");
    
    error_pos = command - (float)ticks;
    error_pos_i += error_pos;
    error_pos_d = error_pos - last_error_pos;
    last_error_pos = error_pos;
    if(error_pos_i > 1000000) error_pos_i = 1000000;
    else if(error_pos_i < -1000000) error_pos_i = -1000000;
    float command_vel = error_pos * 0.8 + error_pos_i * 0.00001 + error_pos_d*1.5;
    if(abs(error_pos) < 10)
    {
      error_pos_i = 0;
      command_vel = 0;
    }
    
    Serial.print(error_pos);
    Serial.print(" ");
    Serial.print(error_pos_i);
    Serial.print(" ");
    Serial.print(error_pos_d);
    Serial.print(" ");
    Serial.print(command_vel);
    Serial.print("/");
    
    if(command_vel > 100) command_vel = 100;
    else if(command_vel < -100) command_vel = -100;
    
    error = command_vel - velocity;
    error_i += error;
    if(error_i > 1000) error_i = 1000;
    else if(error_i < -1000) error_i = -1000;
    float control_u = error*1 + error_i*0.02;
    Serial.print(error);
    Serial.print(" ");
    Serial.print(error_i);
    Serial.print(" ");
    Serial.println(control_u);
    if(control_u > 70) control_u = 70;
    else if(control_u < -70) control_u = -70;
    if(abs(control_u) < 10) control_u = 0;
    if(control_u > 0)
    {
      am2861.control(false, control_u);
    }
    else
    {
      am2861.control(true, -control_u);
    }
    lastTick = ticks;
    myTime = millis();
  }
  if (Serial.available() > 0)
  {
    int inByte = Serial.read();
    if(isDigit(inByte))
    {
      inString += (char)inByte;
    }
    if(inByte == '\n')
    {
      command = inString.toInt();
      inString = "";
    }
  }
}
