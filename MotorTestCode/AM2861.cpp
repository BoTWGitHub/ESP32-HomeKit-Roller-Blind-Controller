#include "AM2861.h"
#include "esp32-hal-ledc.h"
#include <math.h>

AM2861::AM2861(int _ledc_ch1, int _pin1, int _ledc_ch2, int _pin2, int _freq, int _res)
{
	ledc_ch1 = _ledc_ch1;
	ledc_ch2 = _ledc_ch2;
	pin1 = _pin1;
	pin2 = _pin2;
	freq = _freq;
	res = _res;
	
	ledcSetup(ledc_ch1, freq, res);
	ledcAttachPin(pin1, ledc_ch1);
	ledcSetup(ledc_ch2, freq, res);
	ledcAttachPin(pin2, ledc_ch2);
}

AM2861::~AM2861(){}

void AM2861::control(bool dir, int duty)
{
	if(duty > 100) duty = 100;
	else if(duty < 0) duty = 0;
	
	int revDuty = 100 - duty;
	
	uint32_t fullDuty = pow(2, res) - 1;
	uint32_t actualDuty = fullDuty * revDuty / 100;
	
	if(duty > 0)
	{
		if(dir)
		{
			ledcWrite(ledc_ch1, fullDuty);
			ledcWrite(ledc_ch2, actualDuty);
		}
		else
		{
			ledcWrite(ledc_ch1, actualDuty);
			ledcWrite(ledc_ch2, fullDuty);
		}
	}
	else
	{
		ledcWrite(ledc_ch1, 0);
		ledcWrite(ledc_ch2, 0);
	}
}
