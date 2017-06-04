/* ===================================================================
**  Projeto 2: Controle de RPM realimentado
**  
**  Taffarel Cunha Ewald 147957
**  Daniel Rodrigues Silveira Freitas 145782
**        
**  Programa implementa um sistema de controle que garante que a 
** 	rotação do motor estabilize em uma velocidade pré-definida para 
**  um PWM ajustado.
** ===================================================================*/

/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
 ** ===================================================================
 **     Event       :  Cpu_OnNMIINT (module Events)
 **
 **     Component   :  Cpu [MKL25Z128LK4]
 */
/*!
 **     @brief
 **         This event is called when the Non maskable interrupt had
 **         occurred. This event is automatically enabled when the [NMI
 **         interrupt] property is set to 'Enabled'.
 */
/* ===================================================================*/
void Cpu_OnNMIINT(void) {
	/* Write your code here ... */
}

/*stdio.h contem rotinas para processamento de expressoes regulares*/
#include <stdio.h>

/*Processo de bufferizacao. Caracteres recebidos sao armazenados em um buffer. 
Quando um caractere de fim de linha ('\n') eh recebido, todos os caracteres do buffer sao processados simultaneamente.*/

/*Buffer de dados recebidos*/
#define MAX_BUFFER_SIZE 20
typedef struct {
	char data[MAX_BUFFER_SIZE];
	unsigned int tam_buffer;
} serial_buffer;

volatile serial_buffer Buffer;
uint8_t duty_cycle;
int dutyc_flag = 0;
int cont = 0;

/*Rotina auxiliar para comparacao de strings*/
int str_cmp(char *s1, char *s2, int len) {
	/*Compare two strings up to length len. Return 1 if they are equal, and 0 otherwise.*/
	int i;
	for (i = 0; i < len; i++) {
		if (s1[i] != s2[i])
			return 0;
		if (s1[i] == '\0')
			return 1;
	}
	return 1;
}

/*Limpa buffer*/
void buffer_clean() {
	Buffer.tam_buffer = 0;
}

/*Adiciona caractere ao buffer*/
int buffer_add(char c_in) {
	if (Buffer.tam_buffer < MAX_BUFFER_SIZE) {
		Buffer.data[Buffer.tam_buffer++] = c_in;
		return 1;
	}
	return 0;
}

void AS1_OnRxChar(void) {
	char c;
	while (AS1_RecvChar(&c) == ERR_OK) {
		if (c == '\r') {
			buffer_add('\0'); /*Se recebeu um fim de linha, coloca um terminador de string no buffer*/
			AS1_SendChar('\r');// Retorna o cursor para o comeco da linha 
			AS1_SendChar('\n');//Quebra de linha
			PWM_DC();
		} else {
			buffer_add(c);
			AS1_SendChar(c);// Mostra na tela o que está sendo escrito no teclado em tempo real
		}
	}
}

/*Funcao PWM_DC eh chamada para definicao do duty cycle de acordo com entrada do usuario armazenada no buffer*/
void PWM_DC() {
	/*Apos definicao do duty cycle o buffer eh limpo para que outra entrada possa ser recebida*/
	
	/*Valor do duty cycle inserido pelo usuario no programa*/
	if (str_cmp(Buffer.data, "DC", 2)){
		sscanf(Buffer.data, "%*s %d", &duty_cycle);  //valor de entrada do usuario eh atribuido a variavel duty_cycle
		dutyc_flag = 1;
		buffer_clean();
	}
}

/*Funcao que recebe variavel do tipo int e envia para terminal*/
void send_int(int val) {
	char str[16];
	if(val < 0) {
		val = val*(-1);
	}
	UTIL1_Num16uToStr((uint8_t*) str, 16, val);
	send_string(str);
	AS1_SendChar('\r');
	AS1_SendChar('\n');
}

/*Funcao que recebe variavel do tipo char* e envia para terminal*/
void send_string(char* s) {
	int err;
	while (*s != '\0') {
		err = 1;
		while (err) {
			err = AS1_SendChar(*s);
		}
		s++;
	}
}

/*
** ===================================================================
**     Event       :  EInt1_OnInterrupt (module Events)
**
**     Component   :  EInt1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*Funcao de interrupcao externa chamada a cada medicao do sensor IR*/
void EInt1_OnInterrupt(void)
{
	cont++;
}

/* END Events */

#ifdef __cplusplus
} /* extern "C" */
#endif 

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
