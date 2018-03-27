#include "Arduino.h"
#include "TroykaRTC.h"

static const uint32_t SECONDSINHOUR = 3600;
static const uint32_t SECONDSINDAY = 86400;
static const uint32_t SECONDSINYEAR = SECONDSINDAY*365;
static const uint32_t SECONDSINLEAPYEAR = SECONDSINDAY+SECONDSINYEAR;
static const uint8_t  DAYSINMONTH[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 
                                 30, 31, 30, 31};

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

static void copyDateTime (datetime& src, datetime& dest)
{
  dest.hour = src.hour;
  dest.minute = src.minute;
  dest.second = src.second;
  dest.day = src.day;
  dest.month = src.month;
  dest.year = src.year;
  dest.weekday = src.weekday;
};

static bool compareTime (datetime& dt1, datetime& dt2)
{
  if ((dt1.hour == dt2.hour) && (dt1.minute == dt2.minute) && (dt1.second == dt2.second))
   return true; else return false;
}

static bool isLeapYear(uint16_t yr)
{
  if ((yr % 4) == 0) return true; else return false;
};

static uint32_t dtToUnixTime (datetime& src)
{
  uint32_t days=0;
  for (uint16_t i=1970; i<src.year; ++i)
   {
    days+=365;
    if (isLeapYear(i)) days++;
   }
  for (uint8_t i=1; i<src.month; ++i)
   {
    days+=DAYSINMONTH [i]; 
    if (i==2) if (isLeapYear(src.year)) days++;
   }
  days+=(src.day-1);
  uint32_t utim = days*SECONDSINDAY;
  utim += (src.hour*SECONDSINHOUR);
  utim += (src.minute*60);
  utim += src.second;
  return utim;
};

static void unixTimeToDT (uint32_t ut, datetime& res)
{ 
  uint16_t year;
  uint8_t month, monthLength;
  uint32_t tim;
  uint32_t days;
  tim=ut;
  res.second=tim%60;
  tim/=60;
  res.minute=tim%60;
  tim/=60;
  res.hour=tim%24;
  tim/=24;
  res.weekday=(tim+4)%7;
  if (res.weekday==0) res.weekday=7;
  year=1970;  
  days=0;
  while ((days+=(isLeapYear(year)?366:365))<=tim) year++;
  res.year=year;
  days-=isLeapYear(year)?366:365;
  tim-=days;
  days=0;
  month=0;
  monthLength=0;
  for (month=1; month<13; month++)
   {
    monthLength=DAYSINMONTH[month];
    if (month==2) if (isLeapYear(year)) monthLength++;         
    if (tim>=monthLength) tim-=monthLength; else break;
   }
  res.month=month;
  res.day = tim+1;  
};

class ClockController
{
 public:
  void init()
  {
    _RTC.begin();
    if (!isTimePresented()) _RTC.set(__TIMESTAMP__);
  };
    
 void read(datetime& dt)
  {
    _RTC.read();   
    dt.hour = _RTC.getHour();
    dt.minute = _RTC.getMinute();
    dt.second = _RTC.getSecond();
    dt.day = _RTC.getDay();
    dt.month = _RTC.getMonth();
    dt.year = _RTC.getYear();
    dt.weekday = _RTC.getWeekDay();    
  };
  
  void setDateTime (datetime& dt)
    {
     _RTC.setHour (dt.hour);
     _RTC.setMinute (dt.minute);
     _RTC.setSecond (dt.second);
     _RTC.setDay (dt.day);
     _RTC.setMonth (dt.month);
     _RTC.setYear (dt.year);
     _RTC.setWeekDay (dt.weekday);     
    }
    
 private:
  RTC _RTC;
  
  bool isTimePresented()
   {
     _RTC.read();
     uint32_t t1 = _RTC.getUnixTime();
     delay (1200);
     _RTC.read();
     if (t1 != _RTC.getUnixTime()) return true; else return false;
   }
};
