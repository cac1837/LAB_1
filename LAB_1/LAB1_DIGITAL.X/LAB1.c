/*
 * File:   LAB1.c
 * Author: Katherine Caceros
 *
 * Created on 22 de enero de 2021, 08:32 AM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ  8000000
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>

//----------------------------------------------------------------------------//
//                      Declaracion de variables                              //
//----------------------------------------------------------------------------//

int flag = 0; 


//----------------------------------------------------------------------------//
//                      Declaracion de funciones                              //
//----------------------------------------------------------------------------//
void rst(void);
void setup(void);           // se declararon las funciones que mas adelante se definen
void semaforo(void);
void cont1(void);
void cont2(void);


//----------------------------------------------------------------------------//
//                              Configuracion                                 //
//----------------------------------------------------------------------------//

// Se realizaran todas las funciones utilizando un loop
void main(void){
    
    setup();
    
    while(1)
    {  
        if ((PORTBbits.RB0 == 1) && (!flag))
        {                                           // se comprueba si el RB0 esta presionado 
            semaforo();  
        }                                           // y si Flag esta desactivada y llama al semaforo   
        if ((PORTBbits.RB1 == 1) && (flag))
        {                                           // se presiona el RB1 y activamos Flags
            cont1();                                // llama a la funcion del jugador 1
            __delay_ms(100);
        }
        if ((PORTBbits.RB2 == 1) && (flag))
        {                                           // realiza lo mismo pero en juagdor 2
            cont2();
            __delay_ms(100);                        //y ponemos un delay para evitar rebote
        }
         if (PORTBbits.RB3 == 1){                   //Se presiona RB3 y reinicia el programa 
            PORTD = 0;
            PORTB = 0;
            PORTC = 0;
            PORTE = 0;
            flag = 0;
            __delay_ms(100);
             
        }
    } 
}


void setup(void){
    ANSEL = 0;          // Establecemos los puertos
    ANSELH = 0;
    
    TRISE = 0;          //Establecemos el puerto E como salida y se limpia
    PORTE = 0;          //SEMAFORO
    
    TRISC = 0;
    PORTC = 0;          //JUGADOR 1
    
    TRISD= 0;
    PORTD = 0;          //JUGAROR 2
    
    TRISB = 0B00001111;     // Solo se encendieron los tres primeron puertos 
    PORTB = 0;              // BOTONES DE JUGADOR Y RST
    
  
}


void semaforo(void) // se configura la configuracion del semaforo
{
    PORTE = 1;                  // Se enciende el primer led con un delay de 100
    __delay_ms(100);
    for(int i=0; i<=2; i++)     // Se utiliza un for para que repita
    {
        PORTE = PORTE <<1;      // Se correra un bit a la derecha con un delay de 100
        __delay_ms(100);
    }
    flag = 1;                   // Se activa flag y se limpia a los cont1 y 2
    PORTD = 0;
    PORTB = 0;
    PORTC = 0;
    
}


void cont1(void){               // Se configura al player 1
    
    if (PORTC == 0)             //Se utiliza un if para comenzar en 0 y al presionar el boton inicia el contador
    {
        PORTC = PORTC++;        // se le ira sumando uno
    }
    else
    {
        PORTC = PORTC << 1;     // Cuando ya este iniciado deja de sumar y desplaza un bit a la der
        if (PORTCbits.RC7 == 1) // Si el cont 1 llega a RC7 apaga la bandera y enciende RB6 indicando que gano
        {
            PORTBbits.RB6 = 1;
            flag = 0;
        }
        else
        {
            PORTBbits.RB6 = 0;  // si no llega no encendera que ha ganado 
        }
    }
}

void cont2(void){ // Para la configuracion del player 2 sera lo mismo que el player 1
    if (PORTD == 0)
    {
        PORTD = PORTD++;
    }
    else
    {
        PORTD = PORTD << 1;
        if (PORTDbits.RD7 == 1) // solo que en esta ocasion cuando llegue a RD7 apaga flag y enciede RB7
        {                       // indicando que ha ganado
            PORTBbits.RB7 = 1;
            flag = 0;
        }
        else
        {
            PORTBbits.RB7 = 0;
        }
    }
}

