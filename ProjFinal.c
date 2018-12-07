/* ************************************************************************** */
/** Descriptive File Name

  @Company
 CPEG 222 Team 1: Matias Saavedra & Alex Chacko

  @File Name
    Robot.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
#pragma config JTAGEN = OFF     // Turn off JTAG - required to use Pin RA0 as IO
#pragma config FNOSC = PRIPLL   //configure system clock 80 MHz
#pragma config FSOSCEN = OFF    // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT     // Primary Oscillator Configuration (XT osc mode)
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
#pragma config FPBDIV = DIV_2   //configure peripheral bus clock to 40 MHz

#ifndef _SUPPRESS_PLIB_WARNING
#define _SUPPRESS_PLIB_WARNING
#endif
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <xc.h>
#include <stdio.h>
#include <plib.h>
#include "config.h"
#include <math.h>
#include "adc.h"
#include "btn.h"
#include "lcd.h"
#include "led.h"
#include "mic.h"
#include "pmods.h"
#include "rgbled.h"
#include "srv.h"
#include "ssd.h"
#include "tone.h"
#include "ultr.h"
#include "utils.h"

#define SYS_FREQ (80000000L)
#define INTSEC 10
#define CORE_TICK_RATE (SYS_FREQ / 2 / INTSEC)

int clockwise = 1100;
int counterClockwise = 1900;

enum modes {
    TEST, READY
};
int mode = TEST;
int micInput;
int btnlock = 0;
char topRow[80];
int miclock = 0;
int counter = 0;
int running = 0;
int timing = 0;

enum directions {
    LEFT, RIGHT, CENTER
};
int direction = CENTER;

int ultDist;
float distance;
char msg[80];
int onLine = 0;

int main(void) {
    LCD_Init();
    delay_ms(100);
    MIC_Init();
    delay_ms(100);
    LED_Init();
    delay_ms(100);
    BTN_Init();
    delay_ms(100);
    SRV_Init();
    delay_ms(100);
    SSD_Init();
    delay_ms(100);
    TONE_Init();
    delay_ms(100);
    ULTR_Init(0,1,0,2);
    delay_ms(100);
    RGBLED_Init();
    delay_ms(100);


    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    OpenCoreTimer(CORE_TICK_RATE); //CoreTimer used for tenths of second capture
    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_5 | CT_INT_SUB_PRIOR_0));
    INTEnableSystemMultiVectoredInt(); //Enable Timer interrupts
    //LCD_WriteStringAtPos("Beebo 3:2", 1, 0);
    update_SSD(0);

    while(1) {
        //LCD_WriteStringAtPos("Beebo 3:2", 1, 0);
        readUltr();
        if (BTN_GetValue('U')) {
            moveForward();
            delay_ms(1500);
            running = 1;
        }

        if (mode == TEST) {
            sprintf(topRow, "TM:1 MODE:TEST ");
        } 
        else {
            sprintf(topRow, "TM:1 MODE:READY");
        }
        LCD_WriteStringAtPos(topRow, 0, 0);
        if (mode == TEST) {
            //pass
        }

        if (mode == READY) {
            micInput = MIC_Val();
            if (micInput > 707) {
                counter = 0;
                delay_ms(50);
                micInput = MIC_Val();
                if ((micInput > 707) && (counter < 1000000)) {
                    running = 1;
                    moveForward();
                    delay_ms(1500);
                }
            }
        }
        while (running) {
            readUltr();
            LCD_WriteStringAtPos(msg, 1, 0);
            timing = 1;
            
            if(onLine){
                
            }
            else{
                
            }
            if(distance < 12){
                turnRightSharp();
                delay_ms(2500);
                moveForward();
                delay_ms(2500);
                turnLeftSharp();
                delay_ms(1000);
                moveForward();
                direction = RIGHT;
                //delay_ms(4000);
            }

            if (PMODS_GetValue(1, 1) && PMODS_GetValue(1, 2) && PMODS_GetValue(1, 3) && PMODS_GetValue(1, 4)) {
                /*if (direction == LEFT) {
                    moveForward();
                    delay_ms(3500);
                    turnRightSharp();
                    delay_ms(1500);
                    //direction = CENTER;
                }
                if (direction == RIGHT) {
                    moveForward();
                    delay_ms(3500);
                    turnLeftSharp();
                    delay_ms(1500);
                    //direction = CENTER;
                }*/

                moveForward();
            }
            if(PMODS_GetValue(1, 1) && !PMODS_GetValue(1, 2) && !PMODS_GetValue(1, 3) && PMODS_GetValue(1, 4)){
                onLine = 1;
                readUltr();
                moveForward();
                if(distance < 12){
                    if(direction == RIGHT){
                        turnLeftSharp();
                        delay_ms(3000);
                        moveForward();
                        delay_ms(7000);
                        turnRightSharp();
                        delay_ms(2000);
                        moveForward();
                        onLine = 0;
                        direction = LEFT;
                    }
                    else if (direction == LEFT){
                        turnRightSharp();
                        delay_ms(3000);
                        moveForward();
                        delay_ms(7000);
                        turnLeftSharp();
                        delay_ms(2000);
                        moveForward();
                        onLine = 0;
                        direction = RIGHT;
                    }
                    
                    
                }
            }
            while (PMODS_GetValue(1, 1) && PMODS_GetValue(1, 2) && !PMODS_GetValue(1, 3) && !PMODS_GetValue(1, 4)) {
                //turnRightSharp();
                if(onLine){
                    turnRightSharp();
                }
                else{
                    //turnLeftSharp();
                }
                
                /*delay_ms(2700);
                moveForward();
                delay_ms(7000);
                turnRightSharp();
                delay_ms(1200);*/
                //direction = RIGHT;
            }
            while(!PMODS_GetValue(1, 1) && !PMODS_GetValue(1, 2) && PMODS_GetValue(1, 3) && PMODS_GetValue(1, 4)) {
                //turnLeftSharp();
                if(onLine){
                  turnLeftSharp();
                }
                else{
                    //turnRightSharp();
                }
                /*delay_ms(2700);
                moveForward();
                delay_ms(7000);
                turnLeftSharp();
                delay_ms(1200);*/
                //direction = LEFT;
            }
            if (!PMODS_GetValue(1, 1) && !PMODS_GetValue(1, 2) && !PMODS_GetValue(1, 3) && !PMODS_GetValue(1, 4)) {
                finish();
            }
        }
    }
}

void moveForward() {
    SRV_SetPulseMicroseconds0(clockwise);
    SRV_SetPulseMicroseconds1(counterClockwise);
}

void turnRight() {
    SRV_SetPulseMicroseconds0(1500);
    SRV_SetPulseMicroseconds1(counterClockwise);
}

void turnRightSharp() {
    SRV_SetPulseMicroseconds0(counterClockwise);
    SRV_SetPulseMicroseconds1(counterClockwise);

}

void turnLeft() {
    SRV_SetPulseMicroseconds0(clockwise);
    SRV_SetPulseMicroseconds1(1500);
}

void turnLeftSharp() {
    SRV_SetPulseMicroseconds0(clockwise);
    SRV_SetPulseMicroseconds1(clockwise);
}

void finish() {
    SRV_SetPulseMicroseconds0(1500);
    SRV_SetPulseMicroseconds1(1500);
    timing = 0;
    delay_ms(5000);
    LED_SetGroupValue(0xFF);
    turnRight();
    delay_ms(3000);
    turnLeft();
    delay_ms(3000);
    SRV_SetPulseMicroseconds0(1500);
    SRV_SetPulseMicroseconds1(1500);
    celebrationSong();
    running = 0;
}

void delay_ms(int ms) {
    int i, counter;
    for (counter = 0; counter < ms; counter++) {
        for (i = 0; i < 500; i++) {//300
        } //software delay ~1 millisec 
    }
}

void celebrationSong() {
    TONE_Start(523, 150);
    TONE_Start(523, 150);
    TONE_Start(523, 150);
    TONE_Start(523, 400);
    TONE_Start(415, 400);
    TONE_Start(466, 400);
    TONE_Start(523, 300);
    delay_ms(50);
    TONE_Start(466, 100);
    delay_ms(50);
    TONE_Start(523, 500);
}

void update_SSD(int value) {
    int hunds, tens, ones, tenths;
    int dec1, dec2;
    char SSD1 = 0b0000000; //SSD setting for 1st SSD (LSD)
    char SSD2 = 0b0000000; //SSD setting for 2nd SSD
    char SSD3 = 0b0000000; //SSD setting for 3rd SSD
    char SSD4 = 0b0000000; //SSD setting for 4th SSD (MSD)
    if (value < 0) {
        SSD4 = 17;
        value = -1 * value;
        dec1 = 0;
        dec2 = 1;
    } else {
        dec1 = 1;
        dec2 = 0;
        hunds = floor(value / 1000);
        if (hunds > 0)
            SSD4 = hunds; //SSD4 = display_char[thous];
        else
            SSD4 = 17;
    }
    tens = floor((value % 1000) / 100);
    if (hunds == 0 && tens == 0)
        SSD3 = 17;
    else
        SSD3 = tens;
    SSD2 = ones = floor(value % 100 / 10);
    SSD1 = tenths = floor(value % 10);
    SSD_WriteDigits(SSD1, SSD2, SSD3, SSD4, 0, 1, 0, 0);
}

void readUltr() {
    delay_ms(100);
    ultDist = ULTR_MeasureDist();
    distance = (ultDist * 34300)*0.0000005;
    if (distance >= 0 && distance < 5)
        RGBLED_SetValue(255, 0, 0);
    else if (distance >= 5 && distance < 10)
        RGBLED_SetValue(255, 255, 0);
    else if (distance >= 10 && distance < 50)
        RGBLED_SetValue(0, 255, 0);
    else
        RGBLED_SetValue(0, 0, 255);

    if (distance < 0)
        sprintf(msg, "Range:%.2f cm\r\n", distance);
    else
        sprintf(msg, "Range:%.2f cm\r\n", distance);
}


void __ISR(_CORE_TIMER_VECTOR, ipl5) _CoreTimerHandler(void) {
    mCTClearIntFlag();
    if (BTN_GetValue('C') && !btnlock) {
        if (mode == READY) {
            mode = TEST;
        } else {
            mode = READY;
        }
        btnlock = 1;
    }
    if (!BTN_GetValue('C')) {
        btnlock = 0;
    }


    if (timing == 1) {
        update_SSD(counter);
    }
    counter++;
    UpdateCoreTimer(CORE_TICK_RATE);
}
