#include "ClockController.cpp"
#include "ButtonsController.cpp"
#include "View.cpp"

class ClockModel
{
 public:
  void init()
   {
    _buttons.init();
    _view.init();
    _clock.init();    
    _mode = MODE_SHOWDATETIME;    
    _currentsecond = 0;
    _currenttact = 0;   
    _setdatetimeflag = false;
    _lighttactscount = (500 / DYNAMICINDICATIONDELAYTIME) / 4;
   }
   
  void run()
   {
    //обнуление текущего такта на каждой новой секунде
    //для обеспечения гашения индикаторов после 0,5сек горения
    _clock.readDateTime();
    uint8_t sec = _clock.getSecond();
    if (sec != _currentsecond)
    {
      _currentsecond = sec;
      _currenttact = 0;
    }
    ++_currenttact;
    //обработка нажатия кнопок
    _buttons.readButtons();
    if (_buttons.isButtonPressed(RETURNBUTTON)) returnButtonPressed();     
    if (_buttons.isButtonPressed(MODEBUTTON)) modeButtonPressed();     
    if (_mode == MODE_SETYEAR)
     if (!_setdatetimeflag)
      {
       _updatedt= _clock.read();
       _setdatetimeflag = true;
      }    
    if (_buttons.isButtonPressed(UPBUTTON)) upButtonPressed();
    if (_buttons.isButtonPressed(DOWNBUTTON)) downButtonPressed();
    
    switch (_mode)
     {
       case MODE_SHOWTIME:
        {
         showTime();
         break;
        };
       case MODE_SHOWDATETIME:
        {
         showDateTime();
         break;
        };   
       case MODE_SETYEAR:
        {
         showSetYear();
         break;
        };    
       case MODE_SETMONTH:
        {
         showSetMonth();
         break;
        };    
       case MODE_SETDAY:
        {
         showSetDay();
         break;
        };    
       case MODE_SETWEEKDAY:
        {
         showSetWeekDay();
         break;
        };   
       case MODE_SETHOUR:
        {
         showSetHour();
         break;
        };       
       case MODE_SETMINUTE:
        {
         showSetMinute();
         break;
        };
       case MODE_SETSECOND:
        {
         showSetSecond();
         break;
        };    
    }
  _view.showDigits(); 
  }
  
 private:
  ClockController _clock;
  ButtonsController _buttons;
  IndicatorView _view;
  uint8_t _mode;
  uint8_t _currentsecond; 
  uint16_t _currenttact;
  uint8_t _maxday;
  datetime _updatedt;
  bool _setdatetimeflag;
  uint8_t _lighttactscount;

  void returnButtonPressed()
   { 
     _mode = MODE_SHOWDATETIME;
     _setdatetimeflag = false;
   }
   
  void modeButtonPressed()
   {
     _mode++;
     if (_mode > MAXMODE)
      {
       _clock.setDateTime (_updatedt);
       _mode = 0;
       _setdatetimeflag = false;
      }
   }
   
  void upButtonPressed()
   {
    switch (_mode)
     {
      case MODE_SETYEAR:
       {
        _updatedt.year++;
        break;
       };    
      case MODE_SETMONTH:
       {
        _updatedt.month++;
        if (_updatedt.month > 12) _updatedt.month = 1;
        break;
       };    
      case MODE_SETDAY:
       {
        _maxday = calculateDaysInMonth (_updatedt.month);        
        _updatedt.day++;
        if (_updatedt.day > _maxday) _updatedt.day = 1;
        break;
       };    
      case MODE_SETWEEKDAY:
       {
        _updatedt.weekday++;
        if (_updatedt.weekday > 7) _updatedt.weekday = 1;
        break;
       };  
      case MODE_SETHOUR:
       {
         _updatedt.hour++;
         if (_updatedt.hour>23) _updatedt.hour = 0;
         break;
       };       
      case MODE_SETMINUTE:
       {
         _updatedt.minute++;
         if (_updatedt.minute>59) _updatedt.minute = 0;
         break;
       };
      case MODE_SETSECOND:
       {
         _updatedt.second++;
         if (_updatedt.second>59) _updatedt.second = 0;
         break;
       }   
     }
   }

  void downButtonPressed()
   {
    switch (_mode)
      {
       case MODE_SETYEAR:
        {
         _updatedt.year--;
         if (_updatedt.year < 1) _updatedt.year = 2100;
         break;
        };    
       case MODE_SETMONTH:
        {
          _updatedt.month--;
          if (_updatedt.month<1) _updatedt.month = 12;
          break;
        };    
       case MODE_SETDAY:
        {
          _updatedt.day--;
          if (_updatedt.day<1) _updatedt.day = calculateDaysInMonth (_updatedt.month);
          break;
        };    
       case MODE_SETWEEKDAY:
        {
          _updatedt.weekday--;
          if (_updatedt.weekday < 1) _updatedt.weekday = 7;
          break;
        };   
       case MODE_SETHOUR:
        {
          if (_updatedt.hour = 0) _updatedt.hour = 23; else _updatedt.hour--;
          break;
        };       
       case MODE_SETMINUTE:
        {
          if (_updatedt.minute = 0) _updatedt.minute = 59; else _updatedt.minute--;
          break;
        };
       case MODE_SETSECOND:
        {
          if (_updatedt.second = 0) _updatedt.second = 59; else _updatedt.second--;
          break;
        };
      }
   }

  uint8_t calculateDaysInMonth(uint8_t mon)
   {
     if (mon == 2) return 29;
      else if ((mon == 1) || (mon == 3) || (mon == 5) || (mon == 7) 
            || (mon == 8) || (mon == 10) || (mon == 12))
            return 31; else return 30;
   }
   
  bool turnOnLeds(){return (_currenttact < _lighttactscount);}
   
  void showTime()
  {
   uint8_t b = _clock.getHour();
   _view.setDigit (1, b / 10);
   _view.setDigit (2, b % 10);
   b = _clock.getMinute();
   _view.setDigit (3, b / 10);
   _view.setDigit (4, b % 10);
   /*
   b = _clock.getSecond();
   if ((b % 2) == 0) 
    _view.turnOnDecimalPoint();
    else _view.turnOffDecimalPoint();  
    */
   if (turnOnLeds()) _view.turnOnDecimalPoint();
    else _view.turnOffDecimalPoint();  
  }
 
  void showDateTime()
  {
   /*
    Характеристика режима:
    Первые 10 секунд отображаем время с морганием точки.
    6 секунд - дату с горящей точкой.
    4 секунды - день недели в последней цифре без точек. Далее - сначала.
   */   
   uint8_t b, b1;
   if (_currentsecond>=20) b1 = _currentsecond % 20; else b1 = _currentsecond;
   if ((b1>=0) && (b1<=10))  {showTime();}
    else 
     {
       if ((b1>=11) && (b1<=16))     
        {
         _view.turnOnDecimalPoint();
         b = _clock.getDay();
         _view.setDigit (1, b / 10);
         _view.setDigit (2, b % 10);
         b = _clock.getMonth();
         _view.setDigit (3, b / 10);
         _view.setDigit (4, b % 10);
        }
        else
         {
          b = _clock.getWeekDay();
          _view.turnOffDecimalPoint();
          _view.setDigit (1, 10);
          _view.setDigit (2, 10);
          _view.setDigit (3, 10);
          _view.setDigit (4, b);
         }    
     }
  }
  
  void showSetYear()
  {
    /*Показ года. Будем отображать четыре цифры без точки.*/
    uint16_t yr = _updatedt.year;
    uint16_t thousand = 0;
    uint16_t decimals = 0;
    if (yr > 999)
     {
      thousand = yr / 100;
      decimals = yr - (thousand * 1000);
     }
      else decimals = yr;
    _view.turnOffDecimalPoint();
    _view.setDigit (1, thousand / 10);
    _view.setDigit (2, thousand % 10);
    _view.setDigit (3, decimals / 10);
    _view.setDigit (4, decimals % 10);
  }
  
  void showSetMonth()
  {
    /*
    Показ месяца. Будем отображать правые мигающие цифры месяца и 
    левые горящие цифры дня; точка будет гореть.    
    */
    uint8_t mon = _updatedt.month;
    uint8_t day = _updatedt.day;        
    _view.setDigit (1, day / 10);
    _view.setDigit (2, day % 10);
    if (turnOnLeds())
      {
       _view.setDigit (3, mon / 10);
       _view.setDigit (4, mon % 10);
      } 
       else
        {
          _view.setDigit (3, 10);
          _view.setDigit (4, 10);
        }
    _view.turnOnDecimalPoint();  
  }
  
  void showSetDay()
  {
    /*
    Показ дня. Будем отображать левые горящие цифры месяца и 
    правые мигающие цифры дня; точка будет гореть.    
    */
    uint8_t mon = _updatedt.month;
    uint8_t day = _updatedt.day;      
    if (turnOnLeds())
     {
      _view.setDigit (1, day / 10);
      _view.setDigit (2, day % 10);
     }     
     else
      {
        _view.setDigit (1, 10);
        _view.setDigit (2, 10);
      } 
    _view.setDigit (3, mon / 10);
    _view.setDigit (4, mon % 10);
    _view.turnOnDecimalPoint();
  }

  void showSetWeekDay()
  {   
    uint8_t wd = _updatedt.weekday;      
    _view.setDigit (1, 10);
    _view.setDigit (2, 10);
    _view.setDigit (3, 10);
    _view.setDigit (4, wd);
    _view.turnOffDecimalPoint();
  }
  
  void showSetHour()
  {
    /*
    Показ часа. Будем отображать правые мигающие цифры часа и 
    левые горящие цифры минут; точка будет мигать.    
    */
    uint8_t hr = _updatedt.hour;
    uint8_t m = _updatedt.minute;
    _view.setDigit (1, hr / 10);
    _view.setDigit (2, hr % 10);
   
    if (turnOnLeds())
     {
      _view.setDigit (3, m / 10);
      _view.setDigit (4, m % 10);
      _view.turnOnDecimalPoint();
     }
     else
      {
        _view.setDigit (3, 10);
        _view.setDigit (4, 10);
        _view.turnOffDecimalPoint(); 
      }
  }

  void showSetMinute()
  {
    /*
    Показ дня. Будем отображать левые горящие цифры месяца и 
    правые мигающие цифры дня; точка будет гореть.    
    */
    uint8_t hr = _updatedt.hour;
    uint8_t m = _updatedt.minute;
    _view.setDigit (1, hr / 10);
    _view.setDigit (2, hr % 10);

    if (turnOnLeds())
     {
      _view.setDigit (3, m / 10);
      _view.setDigit (4, m % 10);
      _view.turnOnDecimalPoint();
     } 
     else
      {
        _view.setDigit (3, 10);
        _view.setDigit (4, 10);
        _view.turnOffDecimalPoint();
      }
  }

  void showSetSecond()
  {
    /*
    Показ секунд. Левые цифры гореть не будут. 
    правые - показывать секунды.    
    */
    uint8_t s = _updatedt.second;
    _view.setDigit (1, 10);
    _view.setDigit (2, 10);
    _view.setDigit (3, s / 10);
    _view.setDigit (4, s % 10);
    _view.turnOffDecimalPoint();
  }
  
};
