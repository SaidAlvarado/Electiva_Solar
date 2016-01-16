#include "Arduino.h"


#ifndef __MOTOR_INTERFACE_H
#define __MOTOR_INTERFACE_H



// #########################################################################################
// ###                            POSICIONES DE LOS NUMEROS                              ###
// #########################################################################################

//Definiciones de la posicion (en pixeles) de los numeros que se actualizan en la pantalla.

#define RC_pos_x 64
#define RC_pos_y 44

#define RF_pos_x 64
#define RF_pos_y 55

#define InDC_pos_x 95
#define InDC_pos_y 54

#define F1_C_pos_x 12
#define F1_C_pos_y 40
#define F1_V_pos_x 30
#define F1_V_pos_y 40

#define F2_C_pos_x 12
#define F2_C_pos_y 48
#define F2_V_pos_x 30
#define F2_V_pos_y 48

#define F3_C_pos_x 12
#define F3_C_pos_y 56
#define F3_V_pos_x 30
#define F3_V_pos_y 56


#define RPM_0_pos_x 45
#define RPM_0_pos_y 18

#define RPM_1_pos_x 57
#define RPM_1_pos_y 18

#define RPM_2_pos_x 69
#define RPM_2_pos_y 18

#define RPM_3_pos_x 81
#define RPM_3_pos_y 18

;
// #########################################################################################
// ###                             INTERFAZ DE MOTORES                                   ###
// #########################################################################################

// Dibuja la interfaz en blanco de la informacion de la bateria.
void Mot_initInterface(GLCD *glcd_p);

// Actualiza el valor de Tmax.
void Mot_updatePhase(GLCD *glcd_p, uint8_t phase, float current, float voltage);

// Actualiza el valor de Tprom.
void Mot_updateRC(GLCD *glcd_p, float RC);

// Actualiza el valor de la Corriente de entrada.
void Mot_updateRF(GLCD *glcd_p, float RF);

// Actualiza el valor de la Corriente de Salida.
void Mot_updateInDC(GLCD *glcd_p, float InDC);

// Actualiza el estado de la barrita de porcentaje.
void Mot_updateRPM(GLCD *glcd_p, uint16_t rpm);



#endif
