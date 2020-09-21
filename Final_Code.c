//Try 2
/*
* File:   newmainXC16.c
* Author: garre
*
* Created on November 1, 2019, 7:58 PM
*/

 

#define FCY 4000000UL
#include "xc.h"
#include <p24F16KA301.h>
#include <libpic30.h>
#pragma config FNOSC=FRC //8MHz Osc
#pragma config ICS = PGx3
#pragma config SOSCSRC = DIG

//Global Variables
int x=0;

//Set Up States
enum state {crap, SEARCH1, LOAD, LOAD2, MIDDLE, SEARCH2, TURNLEFT, SHOOTLEFT, TURNCENTERLEFT, SHOOT, SHOOTRIGHT, TURNCENTERRIGHT, TURNRIGHT, ONEEIGHTY, FLIPAROUND};
enum state stat;

void _ISR _OC1Interrupt(void);
void config_ad(void);

int main(void)

{
    //Set Up Analog Inputs

    _ANSA1=1;  //pin 3 //Front IR
    _ANSB2=1;  //pin 6 //Left IR
    _ANSA2=1;  //pin 7 //Right IR
    _ANSB4=1;  //pin 9 //Back IR
    _ANSB1=1;

    _ANSB13=0;  // pin 16   digital
    _ANSB15=0;

    //Set up input/output pins

    _TRISA6=0;  // pin 14   output Wheel step
    _TRISB1=1;
    _TRISB9=0;  // pin 13   wheel direction pin

    _TRISB8=0;  // pin 12   wheel direction pin 

    _TRISA1=1;  //pin 3 //Front IR //BUFF 1
    _TRISB2=1;  //pin 6 //Left IR //BUFF 4
    _TRISA2=1;  //pin 7 //Right IR //BUFF 13
    _TRISB4=1;  //pin 9 //Back IR //BUFF 15
    _TRISA0=0;  // pin 2
    _TRISB12=1; // pin 15   IR Sensor input
    _TRISB13=0; // pin 16
    _TRISB15=0; // pin 18 DC Motor
            
    _LATB13=0;  // pin 16 output GROUND
    _LATA0=1;   // pin 2 output 3.3V

    //Set up direction of wheels
    _LATB9=0;
    _LATB8=1;

   // Set up output compare interrupt
   // Pin 14

    _OC1IP = 4; // Select OCx interrupt priority
    _OC1IE = 1; // Enable OCx interrupt
    _OC1IF = 0; // Clear interrupt flag 

    //Set up PWM signal
    OC1CON1 = 0;
    OC1CON2 = 0;

    // Set period and duty cycle
    OC1R = 10;    
    OC1RS = 25000;            

    // Configure OC1
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0;
    OC1CON1bits.OCM = 0b110;

   

    // Configure A/D
    config_ad();

    __delay_ms(10);
    
    int y=0;
    stat = crap;

    x=0;

    while(1)
    {
        switch (stat)

        { 
            case crap:
                if (y==1)
                {
                  stat=SEARCH2;  
                }
                OC1R = 10; 
                OC1RS = 20999; 
                
                stat = SEARCH1;
                break;

            case SEARCH1:

                if (ADC1BUF1 >= 1500)
                {
                    __delay_ms(500);

                    OC1R = 0;   
                    OC1RS = 20999;
                    
                    __delay_ms(1000);
                    
                    x = 0;

                    stat= FLIPAROUND;
                }
                break;
                
            case LOAD:
                
                OC1R = 10;               
                OC1RS = 20999; 
                
                _LATB9=1;
                _LATB8=1;
                
                if(x>=900)
                {
                    int y=0;
                    
                    // Ball load counter
                    for (y=0; y<5; y=y+1)
                    {
                    OC1R = 0;   
                    OC1RS = 20999;
                    __delay_ms(100);
                    OC1R = 10;               
                    OC1RS = 20999;
                    _LATB9=1;
                    _LATB8=1;
                    __delay_ms(500);
                    _LATB9=0;
                    _LATB8=0;
                    __delay_ms(500);
                    }
                    
                    x=0;
                    stat=MIDDLE;
                    
                
                }
                
                break;
                
            case MIDDLE:
                if (x>=520) //950
                {
                    OC1R=0;
                    OC1RS=20999;
                    stat=SEARCH2;
                }
                
                break;

               

            case SEARCH2:
                OC1R=0;
                OC1RS=20999;
                __delay_ms(300);
                if (ADC1BUF3 >= 2500)           // pin 3
                {
                    stat=TURNRIGHT;

                    x=0;
                }
//
                 if (ADC1BUF15 >= 1500)     // pin 7
                {
                    stat=TURNLEFT;

                    x=0;
                }
//
                else if (ADC1BUF1 >= 1500)      // pin 6
                {
//                    _LATB15=1;
                    x=0;
                    stat=SHOOT;
                }
                
                else
                {
//                    __delay_ms(300);
//                    _LATB15=0;
                }
//                }
                break;

            case TURNRIGHT:
                _LATB9=1;
                _LATB8=0; 

                OC1R = 100; 
                OC1RS = 20999;

                if (x >= 180)
                {
                OC1R = 0;
                OC1RS = 20999; 
                x=0;

                stat=SHOOTRIGHT;
                }

                break;
                
            case SHOOTRIGHT:
//                if (ADC1BUF1>=1500)
//                {
//                    _LATB15=1;
//                    __delay_ms(100);
//                }
//                else
//                {
//                    _LATB15=0;
//                    x=0;
//                    stat=TURNCENTERRIGHT;
//                    
//                }
                _LATB15=1;
                if (x>=1000)
                {
                    x=0;
                    _LATB15=0;
                    __delay_ms(100);
                    stat=TURNCENTERRIGHT;
                }
                break;
            case TURNCENTERRIGHT:
                __delay_ms(100);
                    _LATB9=0;
                    _LATB8=1; 

                    OC1R = 100; 
                    OC1RS = 20999;

                    if (x >= 180)
                    {
                        x=0;
                    OC1R = 0;
                    OC1RS = 20999; 
                    stat=LOAD;
                    }
                    break;

        
            case SHOOT:
//                if (ADC1BUF1>=1500)
//                {
//                _LATB15=1;
//                __delay_ms(10);
//                }
//                else
//                {
//                    x=0;
//                    _LATB15=0;
//                    __delay_ms(300);
//                    stat=LOAD;
//                }
                _LATB15=1;
                if (x>=1000)
                {
                    x=0;
                    _LATB15=0;
                    stat=LOAD;
                }
                break;

               

            case TURNLEFT:
                _LATB9=0;
                _LATB8=1; 

                OC1R = 100; 
                OC1RS = 20999;

                if (x >= 180)
                {
                OC1R = 0;
                OC1RS = 20999; 
                x=0;

                stat=SHOOTLEFT;
                }
                break;
            case SHOOTLEFT:
//                if (ADC1BUF1>=1500)
//                {
//                    _LATB15=1;
//                    __delay_ms(100);
//                }
//                else
//                {
//                    _LATB15=0;
//                    x=0;
//                    stat=TURNCENTERLEFT;
//                    
//                }
                _LATB15=1;
                if (x>=1000)
                {
                    x=0;
                    _LATB15=0;
                    stat=TURNCENTERLEFT;
                }
                break;
            case TURNCENTERLEFT:
                _LATB9=1;
                _LATB8=0; 

                OC1R = 100; 
                OC1RS = 20999;

                if (x >= 180)
                {
                    x=0;
                OC1R = 0;
                OC1RS = 20999; 
                stat=LOAD;
                }
                break;

               

            case ONEEIGHTY:
                _LATB9=1;
                _LATB8=0;
                
                OC1R = 100;  
                OC1RS = 20999;
                
                if (x>=360)
                {
                    OC1R = 0;  
                    OC1RS = 20999;
                
                    if (ADC1BUF4 >= 1500)
                    {
                        _LATB15=1;
                    }

                    else
                    {
                        _LATB13=0;
//                        stat=;
                    }
                }

                 

                break;

               

            case FLIPAROUND:

                _LATB9=1;
                _LATB8=0; 
                OC1R = 100;               
                OC1RS = 20999;
                
                if (x >= 340)
                {
                OC1R = 0;               
                OC1RS = 20999; 
                stat=LOAD;
                }

                break;
            case LOAD2:
                OC1R = 10;               
                OC1RS = 20999; 
                
                _LATB9=1;
                _LATB8=1;
                
                if(x>=1200)
                {
                    int y=0;
                    
                    // Ball load counter
                    for (y=0; y<3; y=y+1)
                    {
                    OC1R = 0;   
                    OC1RS = 20999;
                    __delay_ms(100);
                    OC1R = 10;               
                    OC1RS = 20999;
                    _LATB9=1;
                    _LATB8=1;
                    __delay_ms(500);
                    _LATB9=0;
                    _LATB8=0;
                    __delay_ms(500);
                    }
                    
                    x=0;
                    stat=MIDDLE;
                    
                
                }            
        }
    }

    return 0;
}

 

 

void _ISR _OC1Interrupt(void)

{
    _OC1IF = 0;
    x++;
}

 

void config_ad(void)

{
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register

    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive

                  // ref voltage

   _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative

                  // ref voltage

    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer

                  // location corresponding to channel

    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified

                  // in AD1CSSx registers

    _SMPI = 3;         // AD1CON2<6:2> -- Every 4th conversion sent

                  // to buffer (if sampling 4 channels)

    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only

    // AD1CON3 register

    _ADRC = 0;    // AD1CON3<15> -- Use system clock

    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D

                  // period TAD

    _ADCS = 0x3F; // AD1CON3<7:0> -- A/D period TAD = 64*TCY

    // AD1CSS registers

    _CSS3=1;
    _CSS14=1;
    _CSS15=1;
    _CSS1=1;

    _ADON = 1;    // AD1CON1<15> -- Turn on A/D
}
