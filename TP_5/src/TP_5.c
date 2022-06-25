/*=============================================================================
 * Author: Pedro Valentin Nieva <valentin9702@gmail.com>
 * Date: 2022/06/20
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TP_5.h"
#include "sapi.h"

/*=====[Definición de funciones]==============================*/
char* itoa(int value, char* result, int base);

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();
   uartConfig(UART_USB, 115200 ); //inicializamos la UART del puerto usb de la placa
   adcConfig(ADC_ENABLE);
   uint16_t dato;
   char enviar[10];
   // ----- Repeat for ever -------------------------
   while( true ) {

	   uint16_t dato = adcRead(CH1); //Leemos el dato entregado por el ADC
	   itoa(dato,enviar,10);//Convertimos ese dato a caracter y lo guardamos en la variable "enviar"
	   uartWriteString( UART_USB, "ADC CH1 value: " );
	   uartWriteString( UART_USB, enviar );//Imprimimos el valor leído por el ADC
	   uartWriteString( UART_USB, ". Max sample value: 1024 \r\n" );
	   delay(1000);

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
