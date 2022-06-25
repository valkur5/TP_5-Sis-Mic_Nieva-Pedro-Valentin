/*=============================================================================
 * Author: Pedro Valentin Nieva <valentin9702@gmail.com>
 * Date: 2022/06/23
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TP_5_EJ3.h"
#include "sapi.h"
#include "RITimer.h"

/*=====[Definición de funciones]==============================*/
char* itoa(int value, char* result, int base);

/*=====[Definición de variables globales]=============================*/
volatile uint16_t Valor=0;
volatile bool_t tecla1=true,tecla2=false,tecla3=false,tecla4=false;

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();
   dacConfig( DAC_ENABLE );
   uartConfig( UART_USB, 115200 );
   RIT_init();
   RIT_set(1);
   gpioWrite(LEDB,true);//Esto solo para indicar el estado inicial del programa, que es de 10Hz y 3.3V
   // ----- Repeat for ever -------------------------
   while( true ) {

	   //Se agregó al código que se encienda un led según la tecla pulsada, para determinar de manera visible el estado en el que se encuentra


	   //Leemos las teclas y cambiamos el estado de las variables y los leds según corresponde:
      if(gpioRead(TEC1)!= true){
    	  tecla1=true;
    	  tecla2=false;
    	  tecla3=false;
    	  tecla4=false;
    	  gpioWrite(LEDB,true);
    	  gpioWrite(LED1,false);
    	  gpioWrite(LED2,false);
    	  gpioWrite(LED3,false);
      }
      if(gpioRead(TEC2)!= true) {
    	  tecla1=false;
    	  tecla2=true;
    	  tecla3=false;
    	  tecla4=false;
    	  gpioWrite(LEDB,false);
    	  gpioWrite(LED1,true);
    	  gpioWrite(LED2,false);
    	  gpioWrite(LED3,false);
      }
      if(gpioRead(TEC3)!= true) {
    	  tecla1=false;
    	  tecla2=false;
    	  tecla3=true;
    	  tecla4=false;
    	  gpioWrite(LEDB,false);
    	  gpioWrite(LED1,false);
    	  gpioWrite(LED2,true);
    	  gpioWrite(LED3,false);
      }
      if(gpioRead(TEC4)!= true) {
    	  tecla1=false;
    	  tecla2=false;
    	  tecla3=false;
    	  tecla4=true;
    	  gpioWrite(LEDB,false);
    	  gpioWrite(LED1,false);
    	  gpioWrite(LED2,false);
    	  gpioWrite(LED3,true);
      }
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
char* itoa(int value, char* result, int base) {
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
	//Según la tecla que se haya pulsado, la salida cambiará de frecuencia y de amplitud

	if(tecla1==true){
		RIT_set(1);
		Valor=Valor+10;//Frecuencia de 10Hz aproximadamente

	}

	if(tecla2==true){
		RIT_set(1);
		Valor=Valor+1;//Frecuencia de 1Hz aproximadamente
	}

	//Con el objetivo de mantener la frecuencia con la misma relación (10Hz y 1Hz), en los casos donde
	//La salida deba ser de 1.65V, el tiempo del RIT será el doble de lento, para compensar el que
	//La onda dientes de sierra será el doble de rápida ya que se redujo la amplitud a la mitad.

	if(tecla3==true){
		RIT_set(2);
		Valor=Valor+1;
		if(Valor>=512){
			Valor=0;
		}
	}

	if(tecla4==true){
		RIT_set(2);
		Valor=Valor+10;//frecuencia de 10Hz aproximadamente
		if(Valor>=512){
			Valor=0;
		}
	}

	char uart[10];

	if (Valor>=1023){
		Valor=0;
	}

	itoa(Valor,uart,10);
	uartWriteString( UART_USB, uart );
	uartWriteString(UART_USB, "\n\r");
	dacWrite(DAC, Valor);
	RIT_clean();
}
