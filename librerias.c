#include <xc.h>
#include "librerias.h"

void ioc_init (char pin){
    OPTION_REGbits.nRBPU = 0;
    //wpub0 es pullup e iocb es para interrupcion on change
    switch (pin){
        case 0: 
            WPUBbits.WPUB0 = 1;
            IOCBbits.IOCB0 = 1;
            TRISBbits.TRISB0 = 1; 
        case 1: 
            WPUBbits.WPUB1 = 1;
            IOCBbits.IOCB1 = 1;
            
        case 2: 
            WPUBbits.WPUB2 = 1;
            IOCBbits.IOCB2 = 1;
            
        case 3: 
            WPUBbits.WPUB3 = 1;
            IOCBbits.IOCB3 = 1;
            
        case 4: 
            WPUBbits.WPUB4 = 1;
            IOCBbits.IOCB4 = 1;
            
        case 5: 
            WPUBbits.WPUB5 = 1;
            IOCBbits.IOCB5 = 1;
            
        case 6: 
            WPUBbits.WPUB6 = 1;
            IOCBbits.IOCB6 = 1;
            
        case 7: 
            WPUBbits.WPUB7 = 1;
            IOCBbits.IOCB7 = 1; 
            
        default:
            break;
    }
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
   
}

void adc_init(int channel){
   /*if(8 > channel)
       ANSEL = (unsigned char)channel;
   else
       ANSELH = (unsigned char)(channel - 8);*/
   ADCON1bits.ADFM = 0;
   ADCON0bits.ADCS = 0b01;
   ADCON1bits.VCFG0 = 0;
   ADCON1bits.VCFG1 = 0;
   ADCON0bits.CHS = (unsigned char)channel;
   ADCON0bits.ADON = 1;
   PIE1bits.ADIE = 1;
   PIR1bits.ADIF = 0;
}
int adc_read (){
    //__delay_us(1000); //1000
    ADCON0bits.GO = 1;
    return ADRESH;
}

void adc_change_channel(int channel){
    ADCON0bits.CHS = (unsigned char)channel;
}

int adc_get_channel(){
    return ADCON0bits.CHS;
}

