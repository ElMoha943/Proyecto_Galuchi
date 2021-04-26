#ifndef FACU_H
#define	FACU_H

#define _XTAL_FREQ 4000000

#define RS PORTCbits.RC1
#define E PORTCbits.RC0

//VARIABLES
unsigned char REF1[10]=" proyecto", REF2[55]=" Sensor de Temperatura";
unsigned char ArrayRX[20], Indice, ArrayProc[20];
unsigned int caca;
float valor, max=50, min=20;
int resultado;

//FUNCIONES
void SendSerial(unsigned char *data);
void RX_Byte(void);
void ProcesarArray(void);
unsigned char Codigo(void);
void InicializarDisplay(void);
void Posicionar(unsigned char pos, unsigned char linea);
void BorrarDisplay();
#endif	/* FACU_H */
