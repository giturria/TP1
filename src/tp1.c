/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-04-26
 */

#define COMPILAR_TP1-1  /*parpadeo del LEDs*/
#define COMPILAR_TP1-2  /*sensado de Push Buttons*/
#define COMPILAR_TP1-3  /*uso de tickHooks*/
#define COMPILAR_TP1-4  /*tickHooks portable*/
#define COMPILAR_TP1-5  /*UART2 como interfaz de DEBUG*/
#define COMPILAR_TP1-6  /*sensado de Push Buttons c/sAPI*/

#define COMPILAR COMPILAR_TP1-6

/*==================[inclusions]=============================================*/

/*#include "TP1-1.h"  */  // <= own header (optional)
#include "sapi.h"       // <= sAPI header

#if COMPILAR==COMPILAR_TP1-4
 /* FUNCION que se ejecuta cada vez que ocurre un Tick. */
/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ){

   static bool_t ledState = OFF;

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ){
      ledState = OFF;
   }
   else{
      ledState = ON;
   }
   gpioWrite( led, ledState );
}
#define TICKRATE_MS 100 // ms
#define LED_TOGGLE_MS 1000 // ms

#endif


#if COMPILAR==COMPILAR_TP1-3
    #include "TP1-3.h"
    /* FUNCION que se ejecuta cada vez que ocurre un Tick. */
/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ){

   static bool_t ledState = OFF;

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ){
      ledState = OFF;
   }
   else{
      ledState = ON;
   }
   gpioWrite( led, ledState );
}
#endif


#if COMPILAR==COMPILAR_TP1-5
/* The DEBUG* functions are sAPI debug print functions.
Code that uses the DEBUG* functions will have their I/O routed to
the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;
#endif




/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();


   /* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
   DEBUG_PRINT_ENABLE;
   /* UART for debug messages. */
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintString( "DEBUG c/sAPI\r\n" );
   debugPrintString( "LED Toggle\n" );


   /* ------------- uso de tickHooks ------------- */
#if COMPILAR==COMPILAR_TP1-3
   /* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
      periodicamente una interrupcion cada 50ms que incrementa un contador de
      Ticks obteniendose una base de tiempos). */
   tickConfig( 50 );

   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
      simplemente una funcion que se ejecutara peri�odicamente con cada
      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
      a una interrupcion.
      El segundo parametro es el parametro que recibe la funcion myTickHook
      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   */

   tickCallbackSet( myTickHook, (void*)LEDR );
   delay(1000);

#elif COMPILAR==COMPILAR_TP1-4
   /* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
      periodicamente una interrupcion cada 50ms que incrementa un contador de
      Ticks obteniendose una base de tiempos). */
   tickConfig( 50 );

   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
      simplemente una funcion que se ejecutara peri�odicamente con cada
      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
      a una interrupcion.
      El segundo parametro es el parametro que recibe la funcion myTickHook
      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   */

   tickCallbackSet( myTickHook, (void*)LEDR );
   delay(TICKRATE_MS);
   /* ------------- sensado de Push Buttons ------------- */

#elif COMPILAR==COMPILAR_TP1-2

   gpioConfig( GPIO0, GPIO_INPUT );

   gpioConfig( GPIO1, GPIO_OUTPUT );

      /* Variable para almacenar el valor de tecla leido */
      bool_t valor;

#endif

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

   /* -------------parpadeo del LEDs ------------- */
#if COMPILAR==COMPILAR_TP1-1

      /* Prendo el led azul */
      gpioWrite( LEDB, ON );

      delay(500);

      /* Apago el led azul */
      gpioWrite( LEDB, OFF );

      delay(500);

      /* ------------- sensado de Push Buttons ------------- */
#elif COMPILAR==COMPILAR_TP1-2

            valor = !gpioRead( TEC1 );
            gpioWrite( LEDB, valor );

            valor = !gpioRead( TEC2 );
            gpioWrite( LED1, valor );

            valor = !gpioRead( TEC3 );
            gpioWrite( LED2, valor );

            valor = !gpioRead( TEC4 );
            gpioWrite( LED3, valor );

            valor = !gpioRead( GPIO0 );
            gpioWrite( GPIO1, valor );

       /* ------------- uso de tickHooks ------------- */
 #elif COMPILAR==COMPILAR_TP1-3

            tickCallbackSet( myTickHook, (void*)LEDG );
            delay(1000);
            tickCallbackSet( myTickHook, (void*)LEDB );
            delay(1000);
            tickCallbackSet( myTickHook, (void*)LED1 );
            delay(1000);
            tickCallbackSet( myTickHook, (void*)LED2 );
            delay(1000);
            tickCallbackSet( myTickHook, (void*)LED3 );
            delay(1000);
            tickCallbackSet( myTickHook, (void*)LEDR );
            delay(1000);

#elif COMPILAR==COMPILAR_TP1-4

            tickCallbackSet( myTickHook, (void*)LEDG );
            delay(LED_TOGGLE_MS);
            tickCallbackSet( myTickHook, (void*)LEDB );
            delay(LED_TOGGLE_MS);
            tickCallbackSet( myTickHook, (void*)LED1 );
            delay(LED_TOGGLE_MS);
            tickCallbackSet( myTickHook, (void*)LED2 );
            delay(LED_TOGGLE_MS);
            tickCallbackSet( myTickHook, (void*)LED3 );
            delay(LED_TOGGLE_MS);
            tickCallbackSet( myTickHook, (void*)LEDR );
            delay(LED_TOGGLE_MS);


#endif

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
