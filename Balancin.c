#include "config.h"
#include "ADC.h"
#define _XTAL_FREQ 48000000
#include "LCD_Libreria.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pic18f4550.h>

int ADC_LECTURA();
float kp, ki, kd = 0.0;
int pbal, setpoint = 0;
char datos[30];
unsigned long grados = 0;
unsigned long pot =0;
int flag=0;

 //FUNCIONES 

void main(void) {
    TRISBbits.RB0 = 0;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE_GIEL = 0;
    RCONbits.IPEN = 1;
    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;
     //ASÍ TENEMOS QUE CADA 16 CICLOS DE RELOJ AUMENTA EN UNO EL CONTEO DEL TIMER0
    T0CON = 0b10000001; 
    //Se activan las interrupciónes y los periféricos
    INTCONbits.GIE = 1;
    INTCONbits.PEIE_GIEL = 1;
    //se resetea el registro del TMR0 y la bandera que indica el desbordamiento del mismo
    TMR0 = 0;
    TMR0IF = 0;
    
    //se carga el pin LD0 con el valor lógico de 0 o cero volts.
    ADC_config();
    LCD_Init();
    LCD_XY(0,0);
    LCD_Cadena("HOLA MUNDO");
    while(1){
        kp = (ADC_LECTURA(0)*100.0)/1023.0;
        ki = (ADC_LECTURA(1))*5.0/1023.0;
        kd = (ADC_LECTURA(2))*100.0/1023.0;
        pbal = ADC_LECTURA(3);
        setpoint = ADC_LECTURA(4);
        
        sprintf(datos,"%.2f %.3f %.2f",kp,ki,kd);
        LCD_XY(0,0);
        LCD_Cadena(datos);
        sprintf(datos,"%.4d %.4d",pbal,setpoint);
         LCD_XY(1,0);
        LCD_Cadena(datos);
        
    }
    return;
}
int ADC_LECTURA (int canal)
{
        ADCON0bits.CHS = canal; //Canal 0 o AN0 escogido como entrada analógica.
        ADCON0bits.GO_nDONE = 1;
        while(ADCON0bits.GO_nDONE == 1);
        return ADRES;
}
void __interrupt() my_isr(void)
{
     
    if (TMR0IF == 1)
    {        
    if (flag == 0){
          pot=setpoint;       
          grados =65535.0 - (1500.0+((pot*5500.0)/1023.0));
          TMR0 = grados;
          LATBbits.LB0 = 0;
          flag=1;
      }    
    else if(flag == 1)
      {
          TMR0 = 65535.0 -(60000.0- (1500.0+((pot*5500.0)/1023.0)));
           //la actualización de las banderas es necesario ponerlas al final, despues de cargar el registro TMR0
          //para evitar una acción en falso.
          LATBbits.LB0 = 1;
          flag=0;
      }                   
        TMR0IF = 0;
    }
  
}
