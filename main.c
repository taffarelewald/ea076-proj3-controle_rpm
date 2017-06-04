/* ###################################################################
 **     Filename    : main.c
 **     Project     : proj3
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-05-22, 15:19, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
 
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "PWM1.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "EInt1.h"
#include "ExtIntLdd1.h"
#include "WAIT1.h"
#include "UTIL1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/*Declaracao de variaveis globais de controle*/
extern int dutyc_flag;			//flag que habilita escrita de novo duty_cycle definido pelo usuario
extern int cont;				//variavel contadora para definicao da velocidade do motor a partir da medida do sensor
extern uint8_t duty_cycle;		//variavel que armazena valor do duty_cycle

/* User includes (#include below this line is not maintained by Processor Expert) */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/
	
	PWM1_Enable(); //Habilita o sinal de PWM que entra na base do transistor
	
	int flag_check = 0;			//flag que habilita medicoes somente a partir do momento em que o duty cycle eh definido
	int speed_ref = 3200;		//variavel que armazena velocidade de referencia para um dutycycle definido pelo usuario
	int speed_measure = 0;		//variavel que armazena velocidade medida pelo sensor IR
	int error;					//variavel que armazena erro da velocidade medida em relacao a velocidade de referencia
	float kp = 0.65;			//constante para controle proporcional
	int P;						//variavel que armazena valor normalizado (0-255) do controle proporcional
	int new_speed;				//variavel que armazena nova velocidade de rotacao
		
		while (1) {
			/*condicao de novo valor de duty_cycle (0-100) definido pelo usuario na entrada*/
			if (dutyc_flag) {
				PWM1_SetRatio8((100 - duty_cycle) * 2.55); 	//dessa forma, a velocidade do motor varia de 0-255, sendo 0 = 100%
				dutyc_flag = 0;								
				flag_check = 1;								//flag habilita medicoes pelo sensor IR
			}
	
			if(flag_check){
				cont = 0;									//cont eh inicializado em 0
				WAIT1_Waitms(1000);							//aguarda 1 segundo para contagem das rotacoes do motor durante esse tempo
			
				speed_ref = 3200*duty_cycle/100;			//velocidade de referencia para um dutycycle eh calculada
				speed_measure = cont*60/2;					//velocidade medida pelo sensor IR eh calculada em rpm
		
				send_string("Referencia : ");
				send_int(speed_ref);
			
				send_string("Medicao : ");
				send_int(speed_measure);

				error = speed_ref - speed_measure;			//erro de velocidade eh computado
			
				P = kp*error*255/3200;						//valor normalizado (0-255) do controle proporcional eh calculado
			
				new_speed = ((100 - duty_cycle) * 2.55) - P;	//nova velocidade de rotacao eh calculada com controle proporcional
				PWM1_SetRatio8(new_speed);						//nova velocidade de rotacao eh setada no motor
				}
		}
		
		
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.09]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
