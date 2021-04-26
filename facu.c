#include<xc.h>
#include"facu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void SendSerial(unsigned char *data)
{
    unsigned char x=0;
    while(data[x]!='\0'){
        while(PIR1bits.TXIF==0){}   
        TXREG=data[x];
        x++;
    }
}

void RX_Byte(void){
    unsigned char i;
    ArrayRX[Indice] = RCREG;
    while(PIR1bits.TXIF == 0){}
    TXREG = ArrayRX[Indice];
    if (ArrayRX[Indice] == 'e'){
        // ArrayRX a ArrayProc
        for (i=0;i<Indice;i++){
            ArrayProc[i] = ArrayRX[i];
        }
        ProcesarArray();
        Indice = 0;
    }
    else{
        Indice++;
    }
}

void ProcesarArray(void){
//    if((ArrayProc[6]==RD0+'0')&&(ArrayProc[7]==RD1+'0')&&(ArrayProc[8]==RD2+'0')&&(ArrayProc[9]==RD3+'0')&&(ArrayProc[10]==RD4+'0')){
    //CONSULTA REF [RC,1,00001]e
        if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='R')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==']')){
            // En posiciones 6 a 10 tengo los caracteres que representan el número de dispositivo con quien se quiere comunicar 
            if(ArrayProc[4]=='1'){ // El maestro me solicita la REF1
                SendSerial(REF1);
            }
            if(ArrayProc[4]=='2'){ // El maestro me solicita la REF2
                SendSerial(REF2);
            }
        }
        //PEDIDO DE ACCION [PA,4,00001,xx,xx]e
        else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='P')&&(ArrayProc[2]=='A')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
            if(ArrayProc[4]=='1') 
            {
                SendSerial("Temperatura cambiada");
                max = ((ArrayProc[12]*10)+ ArrayProc[13]);
                min = ((ArrayProc[15]*10)+ ArrayProc[16]);
            }
        }
        //CONSULTA A DISPOSITIVO [CO,1,00001,XX,XX]e
        else if ((ArrayProc[0]=='[')&&(ArrayProc[1]=='C')&&(ArrayProc[2]=='O')&&(ArrayProc[3]==',')&&(ArrayProc[5]==',')&&(ArrayProc[11]==',')&&(ArrayProc[14]==',')&&(ArrayProc[17]==']')){
            if(ArrayProc[4]=='1')
            {
                unsigned char cosa[4];
                sprintf(cosa,"%.1f",valor);
                SendSerial(cosa);
            }
        }
//    }
}

void InicializarDisplay(void){
    //Display on/off
    E = 1;
    RS = 0;
    PORTB = 0b00001111;
    E = 0;
    __delay_ms(1);
    //Fuction set
    E = 1;
    RS = 0;
    PORTB = 0b00111000;
    E = 0;
    __delay_ms(1);
    //Entry Mode Set
    E = 1;
    RS = 0;
    PORTB = 0b00000110;
    E = 0;
    __delay_ms(1);
    //Return Home
    E = 1;
    RS = 0;
    PORTB = 0b00000010;
    E = 0;
    __delay_ms(2);
}

void BorrarDisplay(void){
    //Display Clear
    RS=0; E=1;
    PORTB=0b00000001;
    E=0;
    __delay_ms(10);
}

void Posicionar(unsigned char pos, unsigned char linea){
    //unsigned char i=0;
    if(linea==1) pos = pos | 0x80;
    else pos = pos | 0x40 | 0x80;
    RS=0; E=1;
    PORTB=pos;
    E=0;
    __delay_ms(1);
}

void putch(char c) {
    RS = 1;
    E = 1;
    __delay_ms(1);
    if (c < 200) PORTB = c;
    else PORTB = c - 200;
    E = 0;
    __delay_ms(1);
}
