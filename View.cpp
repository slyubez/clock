#include <stdint.h> 
#include "Consts.cpp"
#include "Arduino.h"

class IndicatorView
{
public:
 void init()
 {
  _pindigits[0] = PIN_DIGIT1;
  _pindigits[1] = PIN_DIGIT2;
  _pindigits[2] = PIN_DIGIT3;
  _pindigits[3] = PIN_DIGIT4;
  pinMode (PIN_DIGIT1, OUTPUT);
  pinMode (PIN_DIGIT2, OUTPUT);
  pinMode (PIN_DIGIT3, OUTPUT);
  pinMode (PIN_DIGIT4, OUTPUT);
  pinMode (PIN_DECIMALPOINT, OUTPUT);
  pinMode (PIN_1, OUTPUT);
  pinMode (PIN_2, OUTPUT);
  pinMode (PIN_4, OUTPUT);
  pinMode (PIN_8, OUTPUT);
  turnIndicatorsOff();
 }
 void setDigit(uint8_t digitnum, uint8_t value){_digits[digitnum-1]=value;}
 void turnOnDecimalPoint()
 {
	if (!_currentdecimalpointvalue)
		{_decimalpoint=true; 
	   digitalWrite (PIN_DECIMALPOINT, 1);
		 _currentdecimalpointvalue = true; 
		}
 }
 void turnOffDecimalPoint()
 {
	if (_currentdecimalpointvalue)
		{_decimalpoint=false; 
	   digitalWrite (PIN_DECIMALPOINT, 0);
		 _currentdecimalpointvalue = false; 
		}
 }
 void turnOffDigit(uint8_t num){digitalWrite (_pindigits[num-1], LOW);}
 void turnOnDigit(uint8_t num){digitalWrite (_pindigits[num-1], HIGH);}
 void showDigits()
 {
  for (uint8_t i=1; i<=4; ++i)
   {	
	  if (i == 2)
	   {
		  if (_decimalpoint) turnOnDecimalPoint(); else turnOffDecimalPoint();
	   }
	  outDigit (i, _digits[i-1]);
	  delay (DYNAMICINDICATIONDELAYTIME);
	  turnOffDigit (i);
   }
 }
 void test()
 {
  setDigit (1, 2);
  setDigit (2, 3);
  setDigit (3, 0);
  setDigit (4, 5);
  turnOnDecimalPoint();
  showDigits();
 }
private:
  uint8_t _digits[4];
  uint8_t _currentdigitvalue;
  uint8_t _currentdecimalpointvalue;
  int _pindigits[4];
  bool _decimalpoint;
  void outLow(uint8_t pin){digitalWrite (pin, LOW);}
  void turnIndicatorsOff()
  { 
    outLow (PIN_DIGIT1);
    outLow (PIN_DIGIT2);
    outLow (PIN_DIGIT3);
    outLow (PIN_DIGIT4);
    outLow (PIN_DECIMALPOINT);
    _currentdigitvalue = 10;
    _currentdecimalpointvalue = false;
  }
  //вывод цифры на сегменты
  void outDigitValue(uint8_t digit)
  { 
    if (_currentdigitvalue!=digit)
     {
		  uint8_t b;
		  b = bitRead (digit, 0);
		  digitalWrite (PIN_1, b);
		  b = bitRead (digit, 1);
		  digitalWrite (PIN_2, b);
		  b = bitRead (digit, 2);
		  digitalWrite (PIN_4, b);
		  b = bitRead (digit, 3);
		  digitalWrite (PIN_8, b);
		  _currentdigitvalue=digit;
	   }
  }
  //вывод цифры с включением индикатора
  void outDigit (uint8_t num, uint8_t value)
  {
   if (value != 10)
   { 
     outDigitValue (value);      
     turnOnDigit(num);
   }
   else turnOffDigit(num);  
  }
};
