#include "Arduino.h"

#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H


// #########################################################################################
// ###                     CAPA DE ABSTRACCION DE HARDWARE                               ###
// #########################################################################################

// Modifique las siguientes funciones para que el resto de la libreria sea compatible
// Con el modulo serial del microcontrolador que desee usar.
// El resto de las funciones derivan de aqui, y no usan caracteristicas o funciones dependientes
// de Hardware.

// Funcion que inicializa la comunicacion serial a 115200 bit/seg
// con formato 1N8.
void serial_Init(void);

// Funcion que envia un byte por el puerto serial.
void serial_sendByte(uint8_t Chr);

//Converts a float to a 4 char strings with 1 decimal.
void read4charfloat(char * buffer, float valor);

////Converts a float to a 3 char strings with 1 decimal.
void read3charfloat(char * buffer, float valor);



// #########################################################################################
// ###                           STRUCTURAS DE PANTALLA                                  ###
// #########################################################################################

// Esta es la estructura que guarda la informacion de la pantalla para manejar las optimizaciones

typedef struct{

    // Informacion general de la pantalla.
    int bat_iface;
    int mot_iface;

    // Informacion sobre la interfaz de baterias.
    float tmax;
    float tprom;
    float ina;
    float outa;
    int   percent_bar;  //0-44 esta variable es en pixeles.
    char  v_0;
    char  v_1;
    char  v_2;

    // Informacion sobre la interfaz de motor.
    float rc;
    float rf;
    float indc;
    float   fase_1c;
    float   fase_1v;
    float   fase_2c;
    float   fase_2v;
    float   fase_3c;
    float   fase_3v;
    char    rpm_0;
    char    rpm_1;
    char    rpm_2;
    char    rpm_3;
} GLCD;


// #########################################################################################
// ###                           COMUNICACION CON EL LCD                                 ###
// #########################################################################################

// Estas funciones crean el API basico de dibujo del LCD, vienen incluidaas en el controlador
// de la pantalla.


// Inicializa la comunicacion con el LCD;
void LCD_Init(GLCD *glcd_p);

// Borra la pantalla completa del LCD;
void LCD_clearScreen(GLCD *glcd_p);

// Invierte el blanco y el negro en el LCD;
void LCD_invert(GLCD *glcd_p);

// Cambia la intensidad del Backlight del LCD;
void LCD_backlightIntensity(GLCD *glcd_p, uint8_t duty_cycle);

// Cambia el BaudRate del LCD a uno de 6 valores predeterminados;
void LCD_changeBaudRate(GLCD *glcd_p, uint8_t BR_option);

// Cambia la posicion de inicio de escritura del generador de texto;
void LCD_setXYCoordinates(GLCD *glcd_p, uint8_t x, uint8_t y);

// Colorea o borra un pixel en la coordenada (x,y);
void LCD_drawPixel(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t color);

// Dibuja o borra una linea entre (x1,y1) y (x2,y2);
void LCD_drawLine(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);

// Dibuja o borra un circulo de centro (x,y) y radio 'r';
void LCD_drawCircle(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t r, uint8_t color);

// Dibuja o borra un rectangulo de esquinas (x1,y1) y (x2,y2);
void LCD_drawRect(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);

// Borra un area rectangular de esquinas (x1,y1) y (x2,y2);
void LCD_eraseRect(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

// Envia un String a la pantalla para ue sea dibujado por el generador de texto;
void LCD_sendText(GLCD *glcd_p, uint16_t length, char * text);

//*******************************************************************************************//

//Apartir de este punto, las siguientes funciones son derivados de las funciones previas.


// Imprime en la pantalla un String en la coordenada (x,y)
void LCD_print(GLCD *glcd_p, uint8_t x, uint8_t y, uint16_t length, char * text);

// Imprime un pixel de 2x2 en la coordenada (x,y), esquina superior izuierda del pixel.
void LCD_fatPixel(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t color);

// Imprime un numero en Fuente aumentada.
void LCD_printFatDigit(GLCD *glcd_p, char last_digit, uint8_t x, uint8_t y, uint8_t num);

// Imprime una linea vertical de dos pixeles de ancho, con (x,y) siendo  la coordenada superios izquierda.
void LCD_fatVline(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t height);

// Imprime una linea horizontal de dos pixeles de ancho, con (x,y) siendo  la coordenada superios izquierda.
void LCD_fatHline(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t width)



#endif
