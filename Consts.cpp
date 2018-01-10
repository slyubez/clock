/*Arduino pins*/

#define PIN_DIGIT1 6
#define PIN_DIGIT2 7
#define PIN_DIGIT3 8
#define PIN_DIGIT4 5

#define PIN_DECIMALPOINT 9

#define PIN_1 10
#define PIN_2 13
#define PIN_4 12
#define PIN_8 11

#define PIN_MODEBUTTON 14
#define PIN_UPBUTTON 15
#define PIN_DOWNBUTTON 16
#define PIN_RETURNBUTTON 17

/*Time intervals in microseconds*/
#define DYNAMICINDICATIONDELAYTIME 3
#define BUTTONSDELAYTIME 500

/*Button constants*/
#define MODEBUTTON 0
#define UPBUTTON 1
#define DOWNBUTTON 2
#define RETURNBUTTON 3

/*Clock mode constants*/
#define MODE_SHOWDATETIME 0
#define MODE_SHOWTIME 1
#define MODE_SETYEAR 2
#define MODE_SETMONTH 3
#define MODE_SETDAY 4
#define MODE_SETWEEKDAY 5
#define MODE_SETHOUR 6
#define MODE_SETMINUTE 7
#define MODE_SETSECOND 8
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT1 9
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT2 10
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT3 11
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT4 12
#define MODE_SETCORRECTINGCOEFFICIENTDIRECTION 13
/*The MAXMODE constant is maximal mode index (excluding special *
 * testing mode indexes). When adding new clock modes, this *
 * constant must be corrected. */
#define MAXMODE 13

/*Special mode constants*/
#define MODE_CHECKBUTTONS 20
