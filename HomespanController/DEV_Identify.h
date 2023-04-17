struct DEV_Identify : Service::AccessoryInformation
{
  SpanCharacteristic *identify;
  DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version) : Service::AccessoryInformation()
  {
    new Characteristic::Name(name);
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);    
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify=new Characteristic::Identify();

    pinMode(homeSpan.getStatusPin(),OUTPUT);
  }
};
