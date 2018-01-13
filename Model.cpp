#include "ClockController.cpp"
#include "ButtonsController.cpp"
#include "MemoryController.cpp"
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
    /*The _currenttact field is using to control of blinking of
    the decimal point and digits with frequency of 1 Hz together
    with calculated _lighttactscount value. The control function
    is turnOnLeds() .*/
    _currenttact = 0;    
    _lighttactscount = (500 / DYNAMICINDICATIONDELAYTIME) / 4;
    _setdatetimeflag = false;
    _correctioncoefficient = 0;
    _correctiondirection = false;
   }
   
  void run()
   {
    //обнуление текущего такта на каждой новой секунде
    //для обеспечения гашения индикаторов после 0,5сек горения
    /*The current tact variable is set to 0 every new second.*/
    _clock.readDateTime();
    uint8_t sec = _clock.getSecond();
    if (sec != _currentsecond)
     {
       _currentsecond = sec;
       _currenttact = 0;
     }
    ++_currenttact;
    //pressing button event handling
    _buttons.readButtons();
    if (_buttons.isButtonPressed(RETURNBUTTON)) returnButtonPressed();     
    if (_buttons.isButtonPressed(MODEBUTTON)) modeButtonPressed();          
    if (_buttons.isButtonPressed(UPBUTTON)) upButtonPressed();
    if (_buttons.isButtonPressed(DOWNBUTTON)) downButtonPressed();
    switch (_mode)
     {
       case MODE_SHOWTIME: {showTime(); break;};
       case MODE_SHOWDATETIME: {showDateTime();  break;};   
       case MODE_SETYEAR: {showSetYear(); break;};    
       case MODE_SETMONTH: {showSetMonth(); break;};    
       case MODE_SETDAY:   {showSetDay();  break;};    
       case MODE_SETWEEKDAY:  {showSetWeekDay();  break;};   
       case MODE_SETHOUR: {showSetHour();  break;};       
       case MODE_SETMINUTE: {showSetMinute();  break;};
       case MODE_SETSECOND:  {showSetSecond();  break;};    
       case MODE_SETCORRECTIONCOEFFICIENT_DIGIT1:  {showSetCorrectionCoefficientDigit1();  break;};    
       case MODE_SETCORRECTIONCOEFFICIENT_DIGIT2:  {showSetCorrectionCoefficientDigit2();  break;}; 
       case MODE_SETCORRECTIONCOEFFICIENT_DIGIT3:  {showSetCorrectionCoefficientDigit3();  break;}; 
       case MODE_SETCORRECTIONCOEFFICIENT_DIGIT4:  {showSetCorrectionCoefficientDigit4();  break;}; 
       case MODE_SETCORRECTIONDIRECTION:  {showSetCorrectionDirection();  break;};
     }
    _view.showDigits(); 
  }
  
 private:
  ClockController _clock;
  ButtonsController _buttons;
  MemoryController _mem;
  IndicatorView _view;
  uint8_t _mode;
  uint8_t _currentsecond; 
  uint16_t _currenttact;
  uint8_t _maxday;
  datetime _updatedt;
  bool _setdatetimeflag;
  uint16_t _lighttactscount;
  uint16_t _d4, _d3, _d2, _d1;
  uint16_t _correctioncoefficient;
  bool _correctiondirection;

  void fetchCorrectionCoefficient()
   {
    _correctioncoefficient = _mem.readWord (CORRECTIONCOEFFICIENTADDRESS);
    _correctiondirection = (_mem.readByte(CORRECTIONDIRECTIONADDRESS) != 0);
    if (_correctioncoefficient > 9999) 
     {
      _correctioncoefficient = 0;
      writeCorrectionCoefficient();
     }
   }

  void writeCorrectionCoefficient()
   {
    _mem.writeWord (CORRECTIONCOEFFICIENTADDRESS, _correctioncoefficient);
    byte b;
    if (_correctiondirection) b = 1; else b = 0;
    _mem.writeByte (CORRECTIONDIRECTIONADDRESS, b); 
   }

  void calculateCorrectionCoefficient()
   {
     _correctioncoefficient = (_d1*1000)+(_d2*100)+(_d3*10)+_d4;
   }

  void divideCorrectionCoefficient()
   {
     _d1 = _correctioncoefficient/1000;
     _d2 = (_correctioncoefficient%1000)/100;
     _d3 = (_correctioncoefficient%100)/10;
     _d4 = _correctioncoefficient % 10;
   }

  void returnButtonPressed()
   { 
     _mode = MODE_SHOWDATETIME;
     _setdatetimeflag = false;
   }
     
  void modeButtonPressed()
   { 
     if (_mode == MODE_SETSECOND) _updatedt.second = 0;
     _mode++;
     if (_mode > MAXMODE)
      {
       _clock.setDateTime (_updatedt);
       _mode = MODE_SHOWDATETIME;
       _setdatetimeflag = false;
       return;
      }     
     if (_mode == MODE_SETYEAR)
      if (!_setdatetimeflag)
        {
         _updatedt = _clock.read();
         _setdatetimeflag = true;
        }
     if (_mode == MODE_SETCORRECTIONCOEFFICIENT_DIGIT1) divideCorrectionCoefficient(); 
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
        _maxday = calculateDaysInMonth (_updatedt.month, _updatedt.year);        
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
         if (_updatedt.hour > 23) _updatedt.hour = 0;
         break;
       };       
      case MODE_SETMINUTE:
       {
         _updatedt.minute++;
         if (_updatedt.minute > 59) _updatedt.minute = 0;
         break;
       };
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT1:
       {
         _d1++;
         if (_d1 > 9) _d1 = 0;
         calculateCorrectionCoefficient();
         break;
       }; 
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT2:
       {
         _d2++;
         if (_d2 > 9) _d2 = 0;
         calculateCorrectionCoefficient();
         break;
       };
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT3:
       {
         _d3++;
         if (_d3 > 9) _d3 = 0;
         calculateCorrectionCoefficient();
         break;
       }; 
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT4:
       {
         _d4++;
         if (_d4 > 9) _d4 = 0;
         calculateCorrectionCoefficient();
         break;
       }; 
      case MODE_SETCORRECTIONDIRECTION:
       {
         _correctiondirection = (!_correctiondirection);
         break;
       };  
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
          if (_updatedt.month < 1) _updatedt.month = 12;
          break;
        };    
       case MODE_SETDAY:
        {
          _updatedt.day--;
          if (_updatedt.day < 1) _updatedt.day = calculateDaysInMonth (_updatedt.month, _updatedt.year);
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
          if (_updatedt.hour == 0) _updatedt.hour = 23; else _updatedt.hour--;
          break;
        };       
       case MODE_SETMINUTE:
        {
          if (_updatedt.minute == 0) _updatedt.minute = 59; else _updatedt.minute--;
          break;
        }; 
       case MODE_SETCORRECTIONCOEFFICIENT_DIGIT1:
       {
         if (_d1 == 0) _d1 = 9; else _d1--;
         calculateCorrectionCoefficient();
         break;
       }; 
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT2:
       {
         if (_d2 == 0) _d2 = 9; else _d2--;
         calculateCorrectionCoefficient();
         break;
       };
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT3:
       {
         if (_d3 == 0) _d3 = 9; else _d3--;
         calculateCorrectionCoefficient();
         break;
       }; 
      case MODE_SETCORRECTIONCOEFFICIENT_DIGIT4:
       {
         if (_d4 == 0) _d4 = 9; else _d4--;
         calculateCorrectionCoefficient();
         break;
       };  
      case MODE_SETCORRECTIONDIRECTION:
       {
         _correctiondirection = (!_correctiondirection);
         break;
       };               
      }
   }

  uint8_t calculateDaysInMonth(uint8_t mon, uint16_t yr)
   {
     if (mon == 2)
      {
        if ((yr % 2) == 0) return 29; else return 28;
      }
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
       decimals = yr - (thousand * 100);
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
     Показ часа. Будем отображать левые мигающие цифры часа и 
     правые горящие цифры минут; точка будет мигать.    
     */
     uint8_t hr = _updatedt.hour;
     uint8_t m = _updatedt.minute;
     if (turnOnLeds())
      {
       _view.setDigit (1, hr / 10);
       _view.setDigit (2, hr % 10);
       _view.turnOnDecimalPoint();
      }
      else
       {
         _view.setDigit (1, 10);
         _view.setDigit (2, 10);
         _view.turnOffDecimalPoint(); 
       }
     _view.setDigit (3, m / 10);
     _view.setDigit (4, m % 10);  
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
     /*Показ мигающих нулей.*/
     _view.setDigit (1, 10);
     _view.setDigit (2, 10);
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      {
       _view.setDigit (3, 0);
       _view.setDigit (4, 0);
       
      } 
      else
       {
         _view.setDigit (3, 10);
         _view.setDigit (4, 10); 
       }
   }

  void showSetCorrectionCoefficientDigit1()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (1, _d1);    
      else _view.setDigit (1, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (3, _d3);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectionCoefficientDigit2()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (2, _d2);    
      else _view.setDigit (2, 10);
     _view.setDigit (1, _d1);
     _view.setDigit (3, _d3);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectionCoefficientDigit3()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (3, _d3);    
      else _view.setDigit (3, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (1, _d1);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectionCoefficientDigit4()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (4, _d4);    
      else _view.setDigit (4, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (3, _d3);
     _view.setDigit (1, _d1);
   }   

  void showSetCorrectionDirection()
   {
     _view.turnOffDecimalPoint();
     if (_correctiondirection) _view.setDigit (1, 1);    
      else _view.setDigit (1, 0);
     _view.setDigit (2, 10);
     _view.setDigit (3, 10);
     _view.setDigit (4, 10);
   }    
  
};

