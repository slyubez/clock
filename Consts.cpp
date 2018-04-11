/*Arduino pins*/

#define PIN_DIGIT1 8
#define PIN_DIGIT2 9
#define PIN_DIGIT3 10
#define PIN_DIGIT4 11

#define PIN_A 0
#define PIN_B 1
#define PIN_C 2
#define PIN_D 3
#define PIN_E 4
#define PIN_F 5
#define PIN_G 6
#define PIN_DECIMALPOINT 7

#define PIN_MODEBUTTON 14
#define PIN_UPBUTTON 15
#define PIN_DOWNBUTTON 16
#define PIN_RETURNBUTTON 17

/*Time intervals*/
#define DYNAMICINDICATIONDELAYTIME 3
#define BUTTONSDELAYTIME 300

/*Tact intervals*/

/*Button constants*/
#define MODEBUTTON 0
#define UPBUTTON 1
#define DOWNBUTTON 2
#define RETURNBUTTON 3

/*Clock mode constants*/
#define MODE_SHOWDATETIMEDAY 0
#define MODE_SHOWDATETIME 1
#define MODE_SHOWTIME 2
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT1 3
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT2 4
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT3 5
#define MODE_SETCORRECTINGCOEFFICIENT_DIGIT4 6
#define MODE_SETCORRECTINGDIRECTION 7
#define MODE_SETYEAR 8
#define MODE_SETMONTH 9
#define MODE_SETDAY 10
#define MODE_SETWEEKDAY 11
#define MODE_SETHOUR 12
#define MODE_SETMINUTE 13
#define MODE_SETSECOND 14

/*The MAXMODE constant is maximal mode index (excluding special *
 * testing mode indexes). When adding new clock modes, this *
 * constant must be corrected. */
#define MAXMODE 14

/*Special mode constants*/
#define MODE_CHECKBUTTONS 20

/*Memory addresses*/
#define SHOWMODEADDRESS 1
#define CORRECTINGCOEFFICIENTADDRESS 10
#define CORRECTINGDIRECTIONADDRESS 20
#define CORRECTINGTIMEADDRESS 30


