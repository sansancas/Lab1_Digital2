/*
 * File:   lab1.c
 * Author: DELL
 *
 * Created on 13 de julio de 2023, 03:41 PM
 */
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include "librerias.h"

#define _XTAL_FREQ 4000000

int disp;
//int volt;
uint8_t volt;
uint8_t decenas;
uint8_t unidades;
uint8_t res;
uint8_t banderas = 0;
uint8_t mostrar [2];

uint8_t tabla [16] = {
    0b00111111,	   
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111,
    0b01110111,
    0b01111100,
    0b00111001,
    0b01011110,
    0b01111001,
    0b01110001
};

void setup(void);

void __interrupt() isr (void){
    if (PIR1bits.ADIF){
        if (adc_get_channel() == 0){
            disp = adc_read();
            //PORTD = disp;
        }
        PIR1bits.ADIF = 0;
    }
    if (INTCONbits.RBIF){
        if (!PORTBbits.RB0){
            PORTC++;
        }
        if(!PORTBbits.RB1){
            PORTC--;
        }
        if(!PORTBbits.RB7){
            PORTC = 128;
        }
        INTCONbits.RBIF = 0;
    }
    if (INTCONbits.T0IF){
       PORTE = 0;
       //volt = (int)(disp*((float)15/255)*16);
       volt = (uint8_t)(disp);
        if(banderas == 0){
            res = volt%16;
            unidades = res;
            PORTD = tabla[unidades];
            PORTE = 1;
            banderas = 1;
        }
        else if(banderas == 1){
            decenas = (uint8_t)(volt/16);
            PORTD = tabla[decenas];
            PORTE = 2;
            banderas = 0;
        }
       if (PORTC == (uint8_t)disp){
           PORTEbits.RE2 = 1;
       }
       else
           PORTEbits.RE2 = 0;
        TMR0 = 6;
        INTCONbits.T0IF = 0;
    }
    return;
}

void main(void) {
    setup();
    ADCON0bits.GO = 1;
    while(1){
        //volt = (int)(disp*((float)5/255)*16);
        //decenas = (uint8_t)(disp/16);
        //res = disp%16;
        //unidades = res;
        //mostrar[1] = tabla[unidades];
        //mostrar[0] = tabla[decenas];
        if(ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 0b0000)
                adc_change_channel(1);
            else        
                adc_change_channel(0);
            __delay_us(1000); //1000
            ADCON0bits.GO = 1;  
        }
        /*if (ADCON0bits.GO == 0){
            __delay_us(50);
            ADCON0bits.GO = 1;
        }*/
        /*if (!PORTBbits.RB1){
            SLEEP();
        }*/
    }
    return;
}

void setup(){
    //habria que auitar ansel y anselh
    ANSEL = 0b00000001;
    ANSELH = 0;
    TRISA = 0b00000001;
    PORTA = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    TRISB = 0b10000111;
    PORTB = 0;
    PORTE = 0;
    TRISE = 0;
    //OPTION_REGbits.nRBPU = 0;
    ioc_init(0);
    ioc_init(1);
    ioc_init(7);
    adc_init(0);
    /*WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
    INTCONbits.RBIE = 1;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB2 = 1;
    INTCONbits.RBIF = 0;*/
    OSCCONbits.IRCF = 0b0110;
    OSCCONbits.SCS = 1;
    /*ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b01;
    ADCON0bits.CHS = 0b0000;
    ADCON0bits.ADON = 1;*/
    __delay_us(50);
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    /*PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;*/
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b001;
    TMR0 = 6;
    INTCONbits.TMR0IE = 1;
    INTCONbits.T0IF = 0;
    
}

void display(int n){
    decenas = (uint8_t)(n/16);
    res = res%16;
    unidades = res;
}

