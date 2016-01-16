#include "Arduino.h"
#include "LCD_driver.h"


#ifndef __BATTERY_INTERFACE_H
#define __BATTERY_INTERFACE_H



// #########################################################################################
// ###                            POSICIONES DE LOS NUMEROS                              ###
// #########################################################################################

//Definiciones de la posicion (en pixeles) de los numeros que se actualizan en la pantalla.

#define tmax_pos_x 0
#define tmax_pos_y 25

#define tprom_pos_x 0
#define tprom_pos_y 48

#define ain_pos_x 69
#define ain_pos_y 46

#define aout_pos_x 69
#define aout_pos_y 56

#define vbat_0_pos_x 47
#define vbat_0_pos_y 21

#define vbat_1_pos_x 59
#define vbat_1_pos_y 21

#define vbat_2_pos_x 75
#define vbat_2_pos_y 21

#define percent_bar_top_x    110
#define percent_bar_top_y    16
#define percent_bar_bottom_x 122
#define percent_bar_bottom_y 60

// #########################################################################################
// ###                             INTERFAZ DE BATERIAS                                  ###
// #########################################################################################
;
// Dibuja la interfaz en blanco de la informacion de la bateria.
void Bat_initInterface(GLCD *glcd_p);

// Actualiza el valor de Tmax.
void Bat_updateTmax(GLCD *glcd_p, float tmax);

// Actualiza el valor de Tprom.
void Bat_updateTprom(GLCD *glcd_p, float tprom);

// Actualiza el valor de la Corriente de entrada.
void Bat_updateAin(GLCD *glcd_p, float ain);

// Actualiza el valor de la Corriente de Salida.
void Bat_updateAout(GLCD *glcd_p, float aout);

// Actualiza el estado de la barrita de porcentaje.
void Bat_percentageBar(GLCD *glcd_p, uint8_t percent);

// Actualiza el estado del voltaje de la bateria.
void Bat_updateVbat(GLCD *glcd_p, float vbat);



#endif
