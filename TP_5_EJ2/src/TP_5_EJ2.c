/*=============================================================================
 * Author: Pedro Valentin Nieva <valentin9702@gmail.com>
 * Date: 2022/06/21
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TP_5_EJ2.h"
#include "sapi.h"
#include "RITimer.h"

/*=====[Definición de funciones]==============================*/
char* itoa(int value, char* result, int base); //Declaramos el prototipo de la función que convierte los valores de entero a caracter de base 10

/*=====[Definición de variables globales]=============================*/
volatile int frecuencia=10000; //Seteamos una frecuencia de 10khz para la onda senoidal
volatile double pi=3.141516,t=0.1;
volatile uint16_t Valor=0;

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();
   dacConfig( DAC_ENABLE ); //Se inicializa el DAC
   uartConfig( UART_USB, 115200 ); //Se inicializa la uart para monitorear los datos enviados por DAC
   RIT_init();//Inicializamos la interrupción RIT
   RIT_set(1);//Seteamos el tiempo en 1 ms
   // ----- Repeat for ever -------------------------
   while( true ) {

	   //Este apartado de código solo se usa para que se pueda ver si la placa está funcionando, haciendo parpadear el led.

      gpioToggle(LED);
      delay(500);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

char* itoa(int value, char* result, int base) { //Función que se encarga de transformar el dato entero que recibe a caracter de base 10
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

void RIT_IRQHandler(void){

	//Abajo de este comentario se encuentra el apartado de código que se encarga de enviar la onda senoidal por el DAC.
	//Si se descomenta el apartado de código de la función senoidal, se deberá comentar la línea 78 del código, que es el que se encarga de la onda dientes de sierra.

	/*
	t=t+0.1;
	double y;
	y=512*(sin(2*pi*frecuencia*t)+1);
	Valor=y;
	*/

	Valor=Valor+10; //Este es el paso de cada punto de la función, esto nos genera una función dientes de sierra de 10 Hz
	char uart[10];
	if (Valor==1023 || Valor<0){//Este if se encarga de evitar que haya valores negativos y sobrepasamiento, también hace que vuelva a 0 la onda de sierra
		Valor=0;
	}


	itoa(Valor,uart,10);//Enviamos el dato por la UART para poder ver desde la computadora el comportamiento del dato enviado por el DAC
	uartWriteString( UART_USB, uart );
	uartWriteString(UART_USB, "\n\r");
	dacWrite(DAC, Valor);
	RIT_clean();
}
