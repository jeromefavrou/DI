
#ifndef DI_h
#define DI_h

#include <Ebool.h> // https://github.com/jeromefavrou/Ebool


class DI : public Ebool
{
  public:

  enum class Filter : uint8_t{NONE =0 , PASS_LOW };
  enum class Signal : uint8_t{PULL_DOWN, PULL_UP };
  
  DI(void):Ebool(false),m_freq(.0),m_pin(0),m_sig(Signal::PULL_UP)
  {
    
  }

  DI operator=(bool const & cpy)
  {
    this->set_bool(cpy);
    return *this;
  }

  DI(uint8_t const _pin, uint8_t const _mode , Signal const & _sig):Ebool(false),m_freq(.0)
  {
    this->init(_pin , _mode , _sig);
  }

  bool init(uint8_t const _pin, uint8_t const _mode , Signal const & _sig)
  {
    if(_mode != INPUT && _mode != INPUT_PULLUP)
      return false;

    this->m_pin = _pin;
    this->m_sig = _sig;
    
    pinMode(this->m_pin  , _mode);

    this->m_filter=millis();
    
    return true;
  }

  void set_filter_freq(float freq)
  {
    this->m_freq= freq;
  }

  bool read(Filter const type_filter =Filter::NONE)
  {
    
     bool r_value = (bool) digitalRead(this->m_pin);

     if(this->m_sig == Signal::PULL_UP)
      r_value = !r_value;

     switch(type_filter)
     {
      case Filter::NONE :
        this->set_bool(r_value);
      break;
      

      case Filter::PASS_LOW :
       if( r_value != this->stat() && millis() - this->m_filter >= 1000.0/this->m_freq)
       {
          this->m_filter = millis();
          this->set_bool(r_value);
       }
        
      break;

      default : this->set_bool(r_value);  break;
      
     }
      
     return this->stat();
  }

  //front montant avec lecture alterante
  bool p_fixed(void)
  {
    bool tmp = this->p();

    if(tmp)
      this->set_bool(true);
    
    return tmp;
  }
  
  //front descendant avec lecture alterante
  bool n_fixed(void)
  {
    bool tmp = this->n();

    if(tmp)
      this->set_bool(false);
    
    return tmp;
  }

  private:
  
  uint8_t m_pin ;
  uint64_t m_filter;
  float m_freq;

  Signal m_sig;
  
};

#endif

