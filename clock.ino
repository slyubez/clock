/* This is an open source Arduino-based desktop or wall-mounting clock project.
 * The source code files are available on https://github.com/slyubez/clock .
 * The electric sheme will later be drawed in KiCad and uploaded to the 
 * same repository.
 * Author: Sergey Lyubeznyy, slyubez@gmail.com
 * Used hardware:
 * 1. AtMega 328P-PU microcontroller with external quartz oscillator 16MHz
 * and with Arduino Uno bootloader included.
 * 2. CD4511BE binary code to 7-segment signal decoder chip. It's inputs are
 * connected to four microcontroller's digital output pins. It's outputs
 * are connected to seven transistor keys
 * 3. Four control buttons, that are connected between analog input pins of 
 * microcontroller and ground. These analog pins are used as digital inputs 
 * with low active level, and these pins are connected to +5V power line via 
 * 10 kiloohm resistors.
 * 4. Troyka RTC board, based on DS1307 real-time clock chip. The Arduino library 
 * for this board is available at https://github.com/amperka/TroykaRTC . The
 * russian language description of this board is available at manufacture's site
 * on http://amperka.ru/product/troyka-rtc
 * 
*/


#include "Consts.cpp"
#include "Model.cpp"
         
ClockModel model;
         
void setup(){model.init();}   
void loop(){model.run();}
