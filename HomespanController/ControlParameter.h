#ifndef CONTROLPARAMETER_H
#define CONTROLPARAMETER_H

#define LEDC_CH0           0
#define LEDC_CH1           1
#define PWM_CH0_PIN        12
#define PWM_CH1_PIN        13
#define PWM_FREQ           5000
#define PWM_RESOLUTION     12

#define ENCODER_PIN1       4
#define ENCODER_PIN2       5
#define ENCODER_FILTER     1023

#define MotorRpmFilter     0.8

#define TicksToRpm        (600/276) //10*60/276 276Ticks/Round 10Hz

#define PosControlILimit   100000
#define PosControlKp       0.8
#define PosControlKi       0.0001
#define PosControlKd       1.2
#define PosControlDeadZone 5

#define MotorMaxRpm        100

#define RpmControlILimit   10000
#define RpmControlKp       1
#define RpmControlKi       0.0035

#define MotorMaxDuty       85 //19V*0.85=16.15V

#endif
