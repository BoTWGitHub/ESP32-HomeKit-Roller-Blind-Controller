#include "AM2861.h"
#include "ESP32Encoder.h"
#include "ControlParameter.h"

AM2861 am2861(LEDC_CH0, PWM_CH0_PIN, LEDC_CH1, PWM_CH1_PIN, PWM_FREQ, PWM_RESOLUTION);
ESP32Encoder encoder;

unsigned long LastMillis;
bool MotorControlling = false;
int32_t LastTicks, TargetTicks = 0;
double MotorRpm;
double error_rpm_i, error_pos_i, error_pos_d, last_error_pos;

int CurtainPositionToEncoderTicks(int pos) //pos 100 = ticks 3600
{
  int tempPos = pos;
  if(tempPos > 100) tempPos = 100;
  else if(tempPos < 0) tempPos = 0;

  return (tempPos*36);
}

int EncoderTicksToCurtainPosition(int ticks)
{
  int tempticks = ticks;
  if(tempticks > 3600) tempticks = 3600;
  else if(tempticks < 0) tempticks = 0;

  return (tempticks/36);
}

void MotorPositionController()
{
  //////////////update Motor Position and Motor RPM
  int32_t Ticks = (int32_t)encoder.getCount();
  double TempRpm = ((double)(Ticks - LastTicks)) * TicksToRpm;
  MotorRpm = MotorRpm * MotorRpmFilter + TempRpm * (1 - MotorRpmFilter);
  
  /////////////update Position Controller
  double error_pos = (double)(TargetTicks - Ticks);
  error_pos_i += error_pos;
  error_pos_d = error_pos - last_error_pos;
  last_error_pos = error_pos;
  if(error_pos_i > PosControlILimit) error_pos_i = PosControlILimit;
  else if(error_pos_i < -PosControlILimit) error_pos_i = -PosControlILimit;
  double command_rpm = error_pos * PosControlKp + error_pos_i * PosControlKi + error_pos_d * PosControlKd;
  if(abs(error_pos) < PosControlDeadZone)
  {
    error_pos_i = 0;
    command_rpm = 0;
    am2861.control(false, 0);
    MotorControlling = false;
    return;
  }
  if(command_rpm > MotorMaxRpm) command_rpm = MotorMaxRpm;
  else if(command_rpm < -MotorMaxRpm) command_rpm = -MotorMaxRpm;

  /////////////update RPM Controller
  double error_rpm = command_rpm - MotorRpm;
  error_rpm_i += error_rpm;
  if(error_rpm_i > RpmControlILimit) error_rpm_i = RpmControlILimit;
  else if(error_rpm_i < -RpmControlILimit) error_rpm_i = -RpmControlILimit;
  double control_u = error_rpm * RpmControlKp + error_rpm_i * RpmControlKi;

  if(control_u > MotorMaxDuty) control_u = MotorMaxDuty;
  else if(control_u < -MotorMaxDuty) control_u = -MotorMaxDuty;

  Serial.println("TargetTicks," + (String)TargetTicks + ",Ticks," + (String)Ticks + ",TargetRpm," + (String)command_rpm + ",Rpm," + (String)MotorRpm + ",Duty," + (String)control_u);

  if(control_u > 0)
  {
    am2861.control(false, control_u);
  }
  else
  {
    control_u*=0.7;
    am2861.control(true, -control_u);
  }
  
  LastTicks = Ticks;
}

struct DEV_RollerBlind : Service::WindowCovering
{
  SpanCharacteristic *current;
  SpanCharacteristic *target;

  DEV_RollerBlind() : Service::WindowCovering()
  {
    ESP32Encoder::useInternalWeakPullResistors=UP;
    encoder.attachSingleEdge(ENCODER_PIN1, ENCODER_PIN2);
    encoder.setFilter(ENCODER_FILTER);
    
    current=new Characteristic::CurrentPosition(100, true); //set True to save and restore value
    target=new Characteristic::TargetPosition(100, true);
    target->setRange(0,100,10);

    int tempPos = current->getVal();
    int tempTicks = CurtainPositionToEncoderTicks(tempPos);
    
    encoder.setCount(tempTicks);

    Serial.println("Configuring Motorized Roller Blind");
    Serial.println("Curren,"+(String)tempPos+",Ticks,"+(String)tempTicks);
  }

  boolean update()
  {
    int TargetPos = target->getNewVal();
    Serial.println("Got new Pos " + (String)TargetPos);
    TargetTicks = CurtainPositionToEncoderTicks(TargetPos);
    MotorControlling = true; 
        
    return true;
  }

  void loop()
  {
    if(MotorControlling)
    {
      if(millis() - LastMillis > 100) //10Hz
      {
        MotorPositionController();
        LastMillis = millis();
      }
    }

    if(current->getVal()!=target->getVal() && target->timeVal()>3000) //over 3s and motor not controlling
    {
      if(!MotorControlling)
      {
        int tempTicks = encoder.getCount();
        current->setVal(EncoderTicksToCurtainPosition(tempTicks));
        target->setVal(EncoderTicksToCurtainPosition(tempTicks));
      }
    }
  }
};
