/* This is an open source Arduino-based digital desktop or wall-mounting clock 
 * project. You can use the code, electric circuit and board for your own purposes
 * for free.
 * The source code files are available on https://github.com/slyubez/clock .
 * The electric circuit and the board are drawed in KiCad and uploaded to the 
 * same repository.
 * Author: Sergey Lyubeznyy, slyubez@gmail.com
*/


#include "Consts.cpp"
#include "Model.cpp"
         
ClockModel model;
         
void setup(){model.init();}   
void loop(){model.run();}
