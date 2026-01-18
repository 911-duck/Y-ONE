class Sensor{
  public:
  Sensor(int pin, int read, int mode = 1){
    _pin = pin;
    _mode_type = mode;
    _read_type = read;
    if(_read_type) begin();
  }

  void begin(){
    pinMode(_pin, _mode_type);
  }

  void setMaxMin(int min, int max){
    _max = max;
    _min = min;
  }

  bool digitalread(){
    if(!digitalRead(_pin) && _last_read){
      _last_read = 0;
      return 1;
    }else if(digitalRead(_pin)) {
      _last_read = 1;
    }
    return 0;
  }

  int read(){
    if(_read_type) return digitalread();
    else return  map(analogRead(_pin),0,1024,_min,_max);
  }
  private:
  int _pin,_mode_type,_read_type;
  int _last_read = 1; 
  int _max = 1024;
  int _min = 0;
};