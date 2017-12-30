#include "Arduino.h"
#include <stdint.h> 
#include "Consts.cpp"

class ButtonsController
{
 public:
  void init()
  {
    _buttonpins [0] = PIN_MODEBUTTON;
    _buttonpins [1] = PIN_UPBUTTON;
    _buttonpins [2] = PIN_DOWNBUTTON;
    _buttonpins [3] = PIN_RETURNBUTTON;   
    for (int i=0; i<=3; ++i) 
    {
      pinMode (_buttonpins[i], INPUT);
      _buttons[i]=false;
    }
  }
  bool readButtons()
  {
    uint8_t cnt = 0;
    for (uint8_t i=0; i<=3; ++i)
    {
     _buttons[i] = (digitalRead(_buttonpins[i]) != HIGH);
     if (_buttons[i]) cnt ++;
    }
    //если нажато более одной кнопки, отключать нажатие всем кнопкам
    if (cnt > 1)  for (int i=0; i<=3; ++i) _buttons[i] = false;
    bool isPressed = (cnt == 1);      
    if (isPressed) delay (BUTTONSDELAYTIME);    
    return isPressed;
  }
  bool isButtonPressed(uint8_t button){return _buttons[button];}
 private:
  bool _buttons[4];
  int _buttonpins[4];
};
