// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON 
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = ON

// CONFIG2
#pragma config BOR4V = BOR40V 
#pragma config WRT = OFF        

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "facu.h"
#include <string.h>

void main(){
    ANSEL=0;
    ANSELH=0;
    OPTION_REG=0x04;
    TRISA=0xFF;
    TRISB=0x00;
    TRISC=0x00;
    TRISC7=1;
    TRISD=0xFF;
    
    //Configura UART a 9600 baudios
    TXSTAbits.TXEN=1;
    TXSTAbits.BRGH=1;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    BAUDCTLbits.BRG16=0;
    SPBRG=25;
    SPBRGH=0;
    
    //Configura puerto AD
    ANSEL=0;
    ANSELbits.ANS1=1;
    ADCON1bits.ADFM=1;
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    ADCON0bits.ADON=1;
    ADCON0bits.CHS=1;
    
    InicializarDisplay();
    unsigned int contador=0;
    while(1){
        //RECIBE UN COMANDO
        if(T0IF==1)
        {
            TMR0=TMR0+131;
            T0IF=0;
            contador++;
            if (contador==500){
                contador=0;
                //MIDE TEMPERATURA
                ADCON0bits.GO=1;
                while (ADCON0bits.GO==1)
                caca= (ADRESH<<8)+ADRESL;
                __delay_ms(2);
                valor= (((float)caca)*5/1023)*100;
//                ENCIENDE LEDS DEPENDIENDO DE LA TEMPERATURA
                BorrarDisplay();
                if(valor>min && valor<max)
				{
					RC2=0;
					RC3=0;
					Posicionar(0,1);
					printf("T Normal");
					Posicionar(5,2);
					printf("%.1f",valor);
				}
				else if(valor>max)
				{
					RC2=1;
					Posicionar(0,1);
					printf("ALERTA T ALTA");
					Posicionar(5,2);
					printf("%.1f",valor);
				}
				else if(valor<min)
				{
					RC3=1;
					Posicionar(0,1);
					printf("ALERTA T BAJA"); 
					Posicionar(5,2);
					printf("%.1f",valor);
				}
            }
        }
        if (RCIF == 1){
            RX_Byte();
        }
    }
}
