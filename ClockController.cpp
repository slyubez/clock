#include "Arduino.h"
#include "TroykaRTC.h"


struct datetime
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t weekday;
};

static void copyDateTime (datetime src, datetime dest)
{
  dest.hour = src.hour;
  dest.minute = src.minute;
  dest.second = src.second;
  dest.day = src.day;
  dest.month = src.month;
  dest.year = src.year;
  dest.weekday = src.weekday;
};


class ClockController
{
 public:
  void init()
  {
    _RTC.begin();
    //_RTC.set(__TIMESTAMP__);
  };
  
  void readDateTime()
  {    
    _RTC.read();
    _hour = _RTC.getHour();
    _minute = _RTC.getMinute();
    _second = _RTC.getSecond();
    _day = _RTC.getDay();
    _month = _RTC.getMonth();
    _year = _RTC.getYear();
    _weekday = _RTC.getWeekDay();
  };
  
  datetime read()
  {
    datetime dt;
    _RTC.read();
    dt.hour = _RTC.getHour();
    dt.minute = _RTC.getMinute();
    dt.second = _RTC.getSecond();
    dt.day = _RTC.getDay();
    dt.month = _RTC.getMonth();
    dt.year = _RTC.getYear();
    dt.weekday = _RTC.getWeekDay();
    return dt;
  };
  
  void setDateTime (datetime dt)
    {
     _RTC.setHour (dt.hour);
     _RTC.setMinute (dt.minute);
     _RTC.setSecond (dt.second);
     _RTC.setDay (dt.day);
     _RTC.setMonth (dt.month);
     _RTC.setYear (dt.year);
     _RTC.setWeekDay (dt.weekday);     
    }
    
  uint8_t getHour(){return _hour;}
  uint8_t getMinute(){return _minute;}
  uint8_t getSecond(){return _second;}
  uint8_t getDay(){return _day;}
  uint8_t getMonth(){return _month;}
  uint16_t getYear(){return _year;}
  uint8_t getWeekDay(){return _weekday;}
 private:
  RTC _RTC;
  uint8_t _hour;
  uint8_t _minute;
  uint8_t _second;
  uint8_t _day;
  uint8_t _month;
  uint8_t _year;
  uint8_t _weekday;
};
