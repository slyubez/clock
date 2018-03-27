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
    _lighttactscount = 500 / (DYNAMICINDICATIONDELAYTIME*4);   
    _currentsecond = 0;
    _currenttact = 0;   
    readCorrectingCoefficient();
    _mode = readDefaultShowMode(); 
   }
   
  void test()
   {
/*    
     Serial.begin(9600);
     datetime dt;
     dt.day = 13;
     dt.month = 3;
     dt.year = 2018;
     dt.hour = 1;
     dt.minute = 50;
     dt.second = 0;
     dt.weekday = 2;
     datetime dt2;
         
     String s = "Begin test";
     Serial.println (s);
     copyDateTime (dt, dt2);   
    s = dt2.day;
    s = s +".";
    s = s +dt2.month;
    s = s +".";
    s = s+dt2.year;
    s = s+"  ";
    s = s + dt2.weekday;
    s = s + "  ";
    s = s+dt2.hour;
    s = s+":";
    s = s+dt2.minute;
    s = s +":";
    s = s+dt2.second;
    Serial.println (s);   
    while (1 == 1) {}; */
   };
  
  void run()
   {
    _clock.read(_currentdt);
    //обнуление текущего такта на каждой новой секунде
    //для обеспечения гашения индикаторов после 0,5сек горения    
    if ((_mode == MODE_SHOWDATETIMEDAY) || (_mode == MODE_SHOWDATETIME) || (_mode == MODE_SHOWTIME))    
     if (_correctingcoefficient>0)
      if (compareTime (_currentdt, _correctingtime))
        {
          correctTime();
          _clock.read(_currentdt);
        }
    if (_currentdt.second != _currentsecond)
     {
      _currentsecond = _currentdt.second;
      _currenttact = 0;
     }
    //обработка нажатия кнопок
    if (_buttons.readButtons())
     {
      if (_buttons.isButtonPressed(RETURNBUTTON)) _mode = readDefaultShowMode();         
      if (_buttons.isButtonPressed(MODEBUTTON)) isModeButtonPressed();      
      if (_buttons.isButtonPressed(UPBUTTON)) isUpButtonPressed();
      if (_buttons.isButtonPressed(DOWNBUTTON)) isDownButtonPressed();
     }      
    switch (_mode)
     {
      case MODE_SHOWTIME: {showTime(); break;};
      case MODE_SHOWDATETIME: {showDateTime(); break;};   
      case MODE_SHOWDATETIMEDAY: {showDateTimeDay(); break;};   
      case MODE_SETYEAR: {showSetYear(); break;};    
      case MODE_SETMONTH: {showSetMonth(); break;};    
      case MODE_SETDAY: {showSetDay(); break;};    
      case MODE_SETWEEKDAY: {showSetWeekDay(); break;};   
      case MODE_SETHOUR: {showSetHour(); break;};       
      case MODE_SETMINUTE: {showSetMinute(); break;};
      case MODE_SETSECOND: {showSetSecond(); break;};    
      case MODE_SETCORRECTINGCOEFFICIENT_DIGIT1:  {showSetCorrectingCoefficientDigit1();  break;};    
      case MODE_SETCORRECTINGCOEFFICIENT_DIGIT2:  {showSetCorrectingCoefficientDigit2();  break;}; 
      case MODE_SETCORRECTINGCOEFFICIENT_DIGIT3:  {showSetCorrectingCoefficientDigit3();  break;}; 
      case MODE_SETCORRECTINGCOEFFICIENT_DIGIT4:  {showSetCorrectingCoefficientDigit4();  break;}; 
      case MODE_SETCORRECTINGDIRECTION: {showSetCorrectingDirection(); break;}; 
     }
   _view.showDigits(); 
   ++_currenttact;
  }
  
 private:
  ClockController _clock;
  ButtonsController _buttons;
  MemoryController _mem;
  IndicatorView _view;
  uint8_t _mode;
  datetime _currentdt;
  uint8_t _currentsecond; 
  uint8_t _currenttact;
  uint8_t maxday;
  datetime _updatedt;  
  uint16_t _lighttactscount;
  uint8_t _d4, _d3, _d2, _d1;
  uint16_t _correctingcoefficient;
  uint16_t _updatecorrectingcoefficient;
  bool _correctingdirection;
  datetime _correctingtime;

  void isUpButtonPressed()
   {
    switch (_mode)
     {
       case MODE_SHOWTIME: 
        {
          _mode = MODE_SHOWDATETIMEDAY; 
          writeDefaultShowMode (_mode);
          break;
        };
       case MODE_SHOWDATETIMEDAY: 
        {
          _mode = MODE_SHOWDATETIME; 
          writeDefaultShowMode (_mode);
          break;
        }; 
       case MODE_SHOWDATETIME: 
        {
          _mode = MODE_SHOWTIME; 
          writeDefaultShowMode (_mode);
          break;
        };  
       case MODE_SETYEAR:
        {
         _updatedt.year++;
         if (_updatedt.year >= 2100) _updatedt.year = 2000;
         divideNumber (_updatedt.year);
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
         maxday = DAYSINMONTH [_updatedt.month];
         if (_updatedt.month == 2) if (isLeapYear (_updatedt.year)) maxday++;         
         _updatedt.day++;
         if (_updatedt.day > maxday) _updatedt.day = 1;
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
          if (_updatedt.minute>59) _updatedt.minute=0;
          break;
        };
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT1:
        {
          _d1++;
          if (_d1 > 9) _d1 = 0;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        }; 
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT2:
        {
          _d2++;
          if (_d2 > 9) _d2 = 0;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        };
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT3:
        {
          _d3++;
          if (_d3 > 9) _d3 = 0;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        }; 
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT4:
        {
          _d4++;
          if (_d4 > 9) _d4 = 0;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        }; 
       case MODE_SETCORRECTINGDIRECTION:
        {
          _correctingdirection = (!_correctingdirection);
          break;
        };    
     };
   }

 void isDownButtonPressed()
  {
    switch (_mode)
      {   
       case MODE_SHOWDATETIMEDAY: 
        {
          _mode = MODE_SETCORRECTINGCOEFFICIENT_DIGIT1; 
          _updatecorrectingcoefficient = _correctingcoefficient;
          divideNumber(_updatecorrectingcoefficient);
          break;
        };     
       case MODE_SHOWDATETIME: 
        {
          _mode = MODE_SETCORRECTINGCOEFFICIENT_DIGIT1; 
          _updatecorrectingcoefficient = _correctingcoefficient;
          divideNumber(_updatecorrectingcoefficient);
          break;
        };
       case MODE_SHOWTIME: 
        {
          _mode = MODE_SETCORRECTINGCOEFFICIENT_DIGIT1; 
          _updatecorrectingcoefficient = _correctingcoefficient;
          divideNumber(_updatecorrectingcoefficient);
          break;
        }; 
       case MODE_SETYEAR:
        {
         _updatedt.year--;
         if (_updatedt.year < 2000) _updatedt.year = 2100;
         divideNumber (_updatedt.year);
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
          if (_updatedt.day<1)
           {
             maxday = DAYSINMONTH [_updatedt.month];
             if (_updatedt.month == 2)
              if (isLeapYear (_updatedt.year)) maxday++;
             _updatedt.day = maxday;
           }           
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
          if (_updatedt.hour == 0) _updatedt.hour = 23; 
           else _updatedt.hour--;          
          break;
        };       
       case MODE_SETMINUTE:
        {
          if (_updatedt.minute == 0) _updatedt.minute = 59; else _updatedt.minute--;
          break;
        };
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT1:
        {
          if (_d1 == 0) _d1 = 9; else _d1--;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        }; 
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT2:
        {
          if (_d2 == 0) _d2 = 9; else _d2--;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        };
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT3:
        {
          if (_d3 == 0) _d3 = 9; else _d3--;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        }; 
       case MODE_SETCORRECTINGCOEFFICIENT_DIGIT4:
        {
          if (_d4 == 0) _d4 = 9; else _d4--;
          _updatecorrectingcoefficient = calculateNumber();
          break;
        };  
       case MODE_SETCORRECTINGDIRECTION:
        {
          _correctingdirection = (!_correctingdirection);
          break;
        }; 
      };
  };

  void isModeButtonPressed()
   {
    //включение и настройка режима установки даты и времени
    if ((_mode == MODE_SHOWDATETIME) || (_mode == MODE_SHOWTIME) || (_mode == MODE_SHOWDATETIMEDAY))
     {
      _mode = MODE_SETYEAR;
      copyDateTime (_currentdt, _updatedt);
      divideNumber (_updatedt.year);
      return;
     }
    //выход из режима настройки даты/времени
    if (_mode == MODE_SETSECOND)
     {
      _updatedt.second = 0;
      _clock.setDateTime (_updatedt);
      copyDateTime (_updatedt, _correctingtime);
      writeCorrectingTime ();
      delay (1000);
      _mode = readDefaultShowMode();     
      return;
     }
    //выход из режима установки поправочного коэффициента
    if (_mode == MODE_SETCORRECTINGDIRECTION)
     {
      _correctingcoefficient = _updatecorrectingcoefficient;
      writeCorrectingCoefficient(); 
      _mode = readDefaultShowMode();     
      return;
     }
    _mode++;
   };

  void addSeconds (datetime& dt, uint16_t cnt)
   {
    uint32_t ut = dtToUnixTime (dt);
    ut += cnt;
    unixTimeToDT (ut, dt);
   }

  void correctTime()
   {
    datetime dt1;
    copyDateTime (_currentdt, dt1);    
    if (_correctingdirection)
     {                 
       if ((_correctingcoefficient%1000) == 0)
        {
          addSeconds (dt1, _correctingcoefficient/1000);         
        }
        else
          {
            addSeconds (dt1, 1+(_correctingcoefficient/1000));        
            uint16_t dl = 1000-(_correctingcoefficient%1000);
            delay (dl);
          } 
       _clock.setDateTime (dt1);      
     }
     else
      {
        delay (_correctingcoefficient);
        _clock.setDateTime (dt1);
        delay (1000);
      }
   }

  bool turnOnLeds(){return (_currenttact < _lighttactscount);}
   
  void showTime()
  {
   uint8_t b = _currentdt.hour;
   _view.setDigit (1, b / 10);
   _view.setDigit (2, b % 10);
   b = _currentdt.minute;
   _view.setDigit (3, b / 10);
   _view.setDigit (4, b % 10);   
   if (turnOnLeds()) _view.turnOnDecimalPoint();
    else _view.turnOffDecimalPoint();
  }
 
  void showDateTimeDay()
  {
   /*
    Характеристика режима:
    Первые 6 секунд отображаем время с морганием точки.
    3 секунды - дату с горящей точкой.
    Последнюю секунду - день недели в последней цифре без точек.
   */
   uint8_t b = _currentdt.second;   
   uint8_t b1;
   if (b>=20) b1 = b % 20; else b1 = b;
   if ((b1>=0) && (b1<=10))  {showTime();}
    else 
     {
       if ((b1>=11) && (b1<=16))     
        {
         _view.turnOnDecimalPoint();
         b = _currentdt.day;
         _view.setDigit (1, b / 10);
         _view.setDigit (2, b % 10);
         b = _currentdt.month;
         _view.setDigit (3, b / 10);
         _view.setDigit (4, b % 10);
        }
        else
         {
          b = _currentdt.weekday;
          _view.turnOffDecimalPoint();
          _view.setDigit (1, 10);
          _view.setDigit (2, 10);
          _view.setDigit (3, 10);
          _view.setDigit (4, b);
         }    
     }
  }

  void showDateTime()
   {
    /*
     Характеристика режима:
     Первые 12 секунд отображаем время с морганием точки.
     3 секунды - дату с горящей точкой.    
    */
    uint8_t b = _currentdt.second;    
    uint8_t b1;
    if (b>=15) b1 = b % 15; else b1 = b;
    if ((b1>=0) && (b1<12))  {showTime();}
     else 
      {
         _view.turnOnDecimalPoint();
         b = _currentdt.day;
         _view.setDigit (1, b / 10);
         _view.setDigit (2, b % 10);
         b = _currentdt.month;
         _view.setDigit (3, b / 10);
         _view.setDigit (4, b % 10);
      }
   }
  
  void showSetYear()
  {
    /*
    Показ года. Будем отображать четыре цифры без точки.    
    */
    _view.turnOffDecimalPoint();
    _view.setDigit (1, _d1);
    _view.setDigit (2, _d2);
    _view.setDigit (3, _d3);
    _view.setDigit (4, _d4);
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
    _view.setDigit (3, m / 10);
    _view.setDigit (4, m % 10);   
    if (turnOnLeds())
     {_view.setDigit (1, hr / 10);
      _view.setDigit (2, hr % 10);      
      _view.turnOnDecimalPoint();
     }
     else
      {
        _view.setDigit (1, 10);
        _view.setDigit (2, 10);
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
    правые - показывать нули.    
    */    
    _view.setDigit (1, 10);
    _view.setDigit (2, 10);
    _view.setDigit (3, 0);
    _view.setDigit (4, 0);
    _view.turnOffDecimalPoint();
   }

  void showSetCorrectingCoefficientDigit1()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (1, _d1);    
      else _view.setDigit (1, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (3, _d3);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectingCoefficientDigit2()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (2, _d2);    
      else _view.setDigit (2, 10);
     _view.setDigit (1, _d1);
     _view.setDigit (3, _d3);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectingCoefficientDigit3()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (3, _d3);    
      else _view.setDigit (3, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (1, _d1);
     _view.setDigit (4, _d4);
   }
  void showSetCorrectingCoefficientDigit4()
   {
     _view.turnOffDecimalPoint();
     if (turnOnLeds())
      _view.setDigit (4, _d4);    
      else _view.setDigit (4, 10);
     _view.setDigit (2, _d2);
     _view.setDigit (3, _d3);
     _view.setDigit (1, _d1);
   }   

  void showSetCorrectingDirection()
   {
     _view.turnOffDecimalPoint();
     if (_correctingdirection) _view.setDigit (1, 1);    
      else _view.setDigit (1, 0);
     _view.setDigit (2, 10);
     _view.setDigit (3, 10);
     _view.setDigit (4, 10);
   } 

 uint8_t readDefaultShowMode()
  {
    uint8_t b = _mem.readByte (SHOWMODEADDRESS);
    if (b == 255) {b = MODE_SHOWDATETIME; writeDefaultShowMode (b);}
    return b;
  }
 void writeDefaultShowMode (uint8_t value)
  {
    _mem.writeByte (SHOWMODEADDRESS, value);
  }

 void writeCorrectingTime()
  {
    _mem.writeByte (CORRECTINGTIMEADDRESS, _correctingtime.hour);
    _mem.writeByte (CORRECTINGTIMEADDRESS+1, _correctingtime.minute);
    _mem.writeByte (CORRECTINGTIMEADDRESS+2, _correctingtime.second); 
  }

 void readCorrectingCoefficient()
  {
   _correctingcoefficient = _mem.readWord (CORRECTINGCOEFFICIENTADDRESS);
   _correctingdirection = (_mem.readByte(CORRECTINGDIRECTIONADDRESS) != 0);
   if (_correctingcoefficient > 9999) 
    {
     _correctingcoefficient = 0;
     writeCorrectingCoefficient();
     _clock.read(_correctingtime);     
     writeCorrectingTime();
     return;
    }
    _correctingtime.hour = _mem.readByte (CORRECTINGTIMEADDRESS);
    _correctingtime.minute = _mem.readByte (CORRECTINGTIMEADDRESS+1);
    _correctingtime.second = _mem.readByte (CORRECTINGTIMEADDRESS+2);
   }
   
  void writeCorrectingCoefficient()
   {
    _mem.writeWord (CORRECTINGCOEFFICIENTADDRESS, _correctingcoefficient);
    byte b;
    if (_correctingdirection) b = 1; else b = 0;
    _mem.writeByte (CORRECTINGDIRECTIONADDRESS, b);
    
   }
   
  uint16_t calculateNumber()
   {
     return ((_d1*1000)+(_d2*100)+(_d3*10)+_d4);
   }

  void divideNumber (uint16_t n)
   {
     _d1 = n/1000;
     _d2 = (n%1000)/100;
     _d3 = (n%100)/10;
     _d4 = n % 10;
   }
};

