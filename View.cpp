#include <stdint.h> 
#include "Consts.cpp"
#include "Arduino.h"

/*
The IndicatorView class controls four LED 7-segment indicators and the decimal
point of second indicator. The setDigit (num, value) method sets a digit to
indicator you need. If value equals 10, the indicator won't light. To control
lightning of decimal point, you can use methods turnOnDecimalPoint() and
turnOffDecimalPoint(). After setting all digit values and decimal point
lightning, the method showDigits() must be called. The test() method is
a simple example to show how to work with this class. This method must be
executed inside a loop. Also it can be corrected and called to measure 
hardware parameters or diagnose hardware faults of LEDs, transistors, wires,
connectors and so other.
*/

/* 
 * This array is containing bit sequences to control seven digit segments.
 * Bit 0 controls segment A, bit 1 - segment B and so other. Value of bit
 * 7 is not used. Array elements from 0 to 9 containing digits. Element 10 
 * is zero (all segments are dark). Elements 11, 12, 13 containing 
 * sequences to show letters H, E and L. You can add your own bit sequences 
 * to this array if you want to show other segment combinations what 7-segment 
 * indicator can show. */
static const uint8_t _digitset[] =
 {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 0, 118, 121, 56};
 
class IndicatorView
{
public:
 void init()
 {
  _pindigits[0] = PIN_DIGIT1;
  _pindigits[1] = PIN_DIGIT2;
  _pindigits[2] = PIN_DIGIT3;
  _pindigits[3] = PIN_DIGIT4;
  for (uint8_t i=0; i<12; ++i) pinMode (i, OUTPUT);
  turnIndicatorsOff();
 }
 void setDigit(uint8_t digitnum, uint8_t value){_digits[digitnum-1]=value;}
 void turnOnDecimalPoint()
  {
 	 if (!_currentdecimalpointvalue)
		{_decimalpoint=true; 
	   digitalWrite (PIN_DECIMALPOINT, HIGH);
		 _currentdecimalpointvalue = true; 
		}
  }
 void turnOffDecimalPoint()
  {
 	 if (_currentdecimalpointvalue)
		{_decimalpoint=false; 
	   digitalWrite (PIN_DECIMALPOINT, LOW);
		 _currentdecimalpointvalue = false; 
		}
  }
 
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
   setDigit (1, 11);
   setDigit (2, 12);
   setDigit (3, 13);
   setDigit (4, 0);
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
  void turnOffDigit(uint8_t num){outLow (_pindigits[num-1]);}
  void turnOnDigit(uint8_t num){digitalWrite (_pindigits[num-1], HIGH);}
  
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
  
  void outDigitValue(uint8_t digit)
   { 
     if (_currentdigitvalue!=digit)
      {
 		  uint8_t b;
 		  for (int i=0; i<7; ++i)
        {
 		     b = bitRead (_digitset[digit], i);
		     digitalWrite (i, b);
        } 
		  _currentdigitvalue=digit;
	   }
   }
  
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
