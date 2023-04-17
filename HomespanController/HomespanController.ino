#include <HomeSpan.h>
#include "DEV_Identify.h"      
#include "DEV_RollerBlind.h"

void setup()
{
  Serial.begin(115200);
  homeSpan.enableOTA();
  homeSpan.setStatusPin(2);
  homeSpan.begin(Category::WindowCoverings,"HomeSpan RollerBlind");

  new SpanAccessory();
  new DEV_Identify("Roller Blind Controller","BoLin","BoLin_20230404","RB Controller","3.0");
  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0"); 
    
  new DEV_RollerBlind(); 
}

void loop()
{
  homeSpan.poll();
}
