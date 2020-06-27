#include <DI.hpp>

DI bp;
bool stat_led;
void setup() 
{

  bp.init(6,INPUT,DI::Signal::PULL_UP);
  
  bp.set_filter_freq(20.0);
  
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
  stat_led=false;
}

void loop()
{
    bp.read(DI::Filter::PASS_LOW);

    if(bp.n_fixed())
    {
      stat_led=!stat_led;
      digitalWrite(7,stat_led);
    }
}


