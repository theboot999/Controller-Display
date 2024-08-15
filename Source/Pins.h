#ifndef Pins_h
#define Pins_h
//vcc //ground //scl //sda
//Display Pins
const uint8_t pDisplayTearingEffect = 37;   
const uint8_t pDisplayTearingEffectIMX = 19;    
const uint8_t pDisplayBackLight =     29;    
//            DisplayTouchSDA2        25
//            DisplayTouchSCL2        24
//            pDisplayWrite           8
//            pDisplayCD              9           
//            pDisplayChipSelect      7
//            pDisplayDB0             19/
//            pDisplayDB1             18/
//            pDisplayDB2             14 
//            pDisplayDB3             15
//            pDisplayDB4             40
//            pDisplayDB5             41
//            pDisplayDB6             17
//            pDisplayDB7             16
//            pDisplayDB8             22
//            pDisplayDB9             23
//            pDisplayDB10            20
//            pDisplayDB11            21
//            pDisplayDB12            38 
//            pDisplayDB13            39 
//            pDisplayDB14            26 
//            pDisplayDB15            27 //This doubles as analogue in from multiplexer

//radio Pins
const uint8_t pRadioCE =              5;
const uint8_t pRadioCSN =             6;
const uint8_t pRF24Interrupt =        30;
//            Radio Mosi              11
//            Radio Miso              12
//            Radio SCK               13

//Magnometer
//                                    24    SCL2
//                                    25    SDA2


//Still to Use for GPS
//                                    0     Use for GPS RX1
//                                    1     Use for GPS TX1


//Still to use for buzzer
//                                    28    PWM OUTPUT
const uint8_t pBuzzer =               28;
//Still to use for vibrator
//                                    10    PWM OUPUT
const uint8_t pVibrator =             10;

//Multiplexer address lines
//use same address lines for both multiplexers. just different input pins. 
//need to make sure we disable analog multiplexer before doing data writes to lcd
//Need to make sure we allow enough time for the multiplexer to disable
//1 microsecond should be enough
const uint8_t pMultiPlex0 =               2;   //GPIO 9 Bit 4
const uint8_t pMultiPlex1 =               3;   //GPIO 9 Bit 5
const uint8_t pMultiPlex2 =               4;   //GPIO 9 Bit 6
const uint8_t pMultiPlex3 =               33;  //GPIO 9 Bit 7
const uint8_t pAnMultiPlexIn =            27;  //This doubles as data for LCD
const uint8_t pAnMultiPlexEnable =        31;
const uint8_t pDiMultiPlexIn =            32;  
const uint8_t pDiMultiPlexEnable =        34;  

//Spare////Spare////Spare////Spare////Spare////Spare////Spare////Spare//
//we will use 2 multiplexers for all buttons. need to do some tests on this with a breadboard
//we can use the same 3 select lines on the multiplexers. just have different data read in
//select both multiplexers to channel 0. read in multiplexer1, then read in multiplexer 2
//8 switches for trim buttons.  May need 16 bit shift register
//6 for 3 position switches?
//1 for 1 position switch?


//                                    35    
//                                    10 pwm







#endif
