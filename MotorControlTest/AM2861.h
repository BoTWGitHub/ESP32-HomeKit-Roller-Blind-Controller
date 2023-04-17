#ifndef AM2861_H
#define AM2861_H

class AM2861
{
  public:
    AM2861(int _ledc_ch1, int _pin1, int _ledc_ch2, int _pin2, int _freq, int _res);
    ~AM2861();
    void control(bool dir, int duty);
  protected:
    int ledc_ch1;
    int ledc_ch2;
    int pin1;
    int pin2;
    int freq;
    int res;
  private:
};

#endif
