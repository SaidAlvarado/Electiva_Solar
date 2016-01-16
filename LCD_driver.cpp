#include "Arduino.h"
#include "LCD_driver.h"


// #########################################################################################
// ###                     CAPA DE ABSTRACCION DE HARDWARE                               ###
// #########################################################################################

// Modifique las siguientes funciones para que el resto de la libreria sea compatible
// Con el modulo serial del microcontrolador que desee usar.
// El resto de las funciones derivan de aqui, y no usan caracteristicas o funciones dependientes
// de Hardware.
;

// Funcion que inicializa la comunicacion serial a 115200 bit/seg
// con formato 1N8.
void serial_init(void){

//    Serial1.begin(115200);
//    LCD_changeBaudRate(2);
    Serial1.begin(9600);
}


// Funcion que envia un byte por el puerto serial.
void serial_sendByte(uint8_t Chr){

    Serial1.write(Chr);
}



//Converts a float to a 4 char strings with 1 decimal.
void read4charfloat(char * buffer, float valor){

    // Arduino.
    dtostrf(valor,4,1,buffer);

    // Hercules RM42x.
    // sprintf(buffer,"%2.1f",valor)

}

//Converts a float to a 3 char strings with 1 decimal.
void read3charfloat(char * buffer, float valor){

    // Arduino.
    dtostrf(valor,3,1,buffer);

    // Hercules RM42x.
    // sprintf(buffer,"%1.1f",valor)

}


// #########################################################################################
// ###                           COMUNICACION CON EL LCD                                 ###
// #########################################################################################

// Estas funciones crean el API basico de dibujo del LCD, vienen incluidaas en el controlador
// de la pantalla.


// Inicializa la comunicacion con el LCD;
void LCD_Init(GLCD *glcd_p){

    serial_init();
    //Inicializamos el objeto puntero a pantalla.
    glcd_p->bat_iface    = 0;
    glcd_p->mot_iface    = 0;
    glcd_p->tmax         = 0;
    glcd_p->tprom        = 0;
    glcd_p->ina          = 0;
    glcd_p->outa         = 0;
    glcd_p->percent_bar  = 0;
    glcd_p->v_0          = ' '; //este es un caracter "espacio". que indica que el digito esta vacio.
    glcd_p->v_1          = ' ';
    glcd_p->v_2          = ' ';
    glcd_p->rc           = 0;
    glcd_p->rf           = 0;
    glcd_p->indc         = 0;
    glcd_p->fase_1c      = 0;
    glcd_p->fase_1v      = 0;
    glcd_p->fase_2c      = 0;
    glcd_p->fase_2v      = 0;
    glcd_p->fase_3c      = 0;
    glcd_p->fase_3v      = 0;
    glcd_p->rpm_0        = ' ';
    glcd_p->rpm_1        = ' ';
    glcd_p->rpm_2        = ' ';
    glcd_p->rpm_3        = ' ';
}

// Borra la pantalla completa del LCD;
void LCD_clearScreen(GLCD *glcd_p){

    serial_sendByte(0x7C);
    serial_sendByte(0x00);
    //Borramos el objeto pantalla
    glcd_p->bat_iface    = 0;
    glcd_p->mot_iface    = 0;
}


// Invierte el blanco y el negro en el LCD;
void LCD_invert(GLCD *glcd_p){

    serial_sendByte(0x7C);
    serial_sendByte(0x12);
}


// Cambia la intensidad del Backlight del LCD;
//  Parameters
//      @ duty_cycle:   numero de rango [0,100], significa
//                      el porcentaje de brillo de la pantalla.
void LCD_backlightIntensity(GLCD *glcd_p, uint8_t duty_cycle){

    //Chequeo de rango
    if (duty_cycle > 100) duty_cycle = 100;

    serial_sendByte(0x7C);
    serial_sendByte(0x02);
    serial_sendByte(duty_cycle);
}


// Cambia el BaudRate del LCD a uno de 6 valores predeterminados;
void LCD_changeBaudRate(GLCD *glcd_p, uint8_t BR_option){

    serial_sendByte(0x7C);
    serial_sendByte(0x07);

    switch(BR_option){

        case 1:
            serial_sendByte(0x31);  // 4800 baud
            break;

        case 2:
            serial_sendByte(0x32);  // 9600 baud
            break;

        case 3:
            serial_sendByte(0x33);  // 19200 baud
            break;

        case 4:
            serial_sendByte(0x34);  // 38400 baud
            break;

        case 5:
            serial_sendByte(0x35);  // 57600 baud
            break;

        case 6:
            serial_sendByte(0x36);  // 115200 baud
            break;

        default:
            serial_sendByte(0x36);  // Default = 115200 baud
            break;
    }
}


// Cambia la posicion de inicio de escritura del generador de texto;
void LCD_setXYCoordinates(GLCD *glcd_p, uint8_t x, uint8_t y){

    // Set X_offset.
    serial_sendByte(0x7C);
    serial_sendByte(0x18);
    serial_sendByte(x);
    // Set Y_offset.
    serial_sendByte(0x7C);
    serial_sendByte(0x19);
    serial_sendByte(y);
}


// Colorea o borra un pixel en la coordenada (x,y);
void LCD_drawPixel(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t color){

    serial_sendByte(0x7C);
    serial_sendByte(0x10);
    serial_sendByte(x);
    serial_sendByte(y);
    if (color)  serial_sendByte(0x01);  // if color is different than 0, set it.
    else        serial_sendByte(0x00);  // clear it otherwise.
}


// Dibuja o borra una linea entre (x1,y1) y (x2,y2);
void LCD_drawLine(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color){

    serial_sendByte(0x7C);
    serial_sendByte(0x0C);
    serial_sendByte(x1);
    serial_sendByte(y1);
    serial_sendByte(x2);
    serial_sendByte(y2);
    if (color)  serial_sendByte(0x01);  // if color is different than 0, set it.
    else        serial_sendByte(0x00);  // clear it otherwise.
}


// Dibuja o borra un circulo de centro (x,y) y radio 'r';
void LCD_drawCircle(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t r, uint8_t color){

    serial_sendByte(0x7C);
    serial_sendByte(0x03);
    serial_sendByte(x);
    serial_sendByte(y);
    serial_sendByte(r);
    if (color)  serial_sendByte(0x01);  // if color is different than 0, set it.
    else        serial_sendByte(0x00);  // clear it otherwise.
}

// Dibuja o borra un rectangulo de esquinas (x1,y1) y (x2,y2);
void LCD_drawRect(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color){

    serial_sendByte(0x7C);
    serial_sendByte(0x0F);
    serial_sendByte(x1);
    serial_sendByte(y1);
    serial_sendByte(x2);
    serial_sendByte(y2);
    if (color)  serial_sendByte(0x01);  // if color is different than 0, set it.
    else        serial_sendByte(0x00);  // clear it otherwise.
}

// Borra un area rectangular de esquinas (x1,y1) y (x2,y2);
void LCD_eraseRect(GLCD *glcd_p, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){

    serial_sendByte(0x7C);
    serial_sendByte(0x05);
    serial_sendByte(x1);
    serial_sendByte(y1);
    serial_sendByte(x2);
    serial_sendByte(y2);
}

// Envia un String a la pantalla para ue sea dibujado por el generador de texto;
void LCD_sendText(GLCD *glcd_p, uint16_t length, char * text){

    int i;

    for (i = 0; i < length; ++i)
    {
        serial_sendByte( (uint8_t) *(i + text) );
    }

}

//*******************************************************************************************//

//Apartir de este punto, las siguientes funciones son derivados de las funciones previas.


// Imprime en la pantalla un String en la coordenada (x,y)
void LCD_print(GLCD *glcd_p, uint8_t x, uint8_t y, uint16_t length, char * text){

    // Movemos el cursor antes de escribir.
    LCD_setXYCoordinates(glcd_p, x, y);

    // Escribimos el texto.
    LCD_sendText(glcd_p, length, text);

}



//Imprime un pixel de 2x2 en la coordenada (x,y), esquina superior izuierda del pixel.
void LCD_fatPixel(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t color){

    if (color) LCD_drawRect(glcd_p, x, y, x+1, y+1, 1);
    else LCD_eraseRect(glcd_p, x, y, x+1, y+1);

}


//Imprime un numero en Fuente aumentada.
void LCD_printFatDigit(GLCD *glcd_p, char last_digit, uint8_t x, uint8_t y, uint8_t num){

    uint8_t font[11][5] =      {{0x3E, 0x51, 0x49, 0x45, 0x3E},     // 0
                                {0x00, 0x42, 0x7F, 0x40, 0x00},     // 1
                                {0x72, 0x49, 0x49, 0x49, 0x46},     // 2
                                {0x21, 0x41, 0x49, 0x4D, 0x33},     // 3
                                {0x18, 0x14, 0x12, 0x7F, 0x10},     // 4
                                {0x27, 0x45, 0x45, 0x45, 0x39},     // 5
                                {0x3C, 0x4A, 0x49, 0x49, 0x31},     // 6
                                {0x41, 0x21, 0x11, 0x09, 0x07},     // 7
                                {0x36, 0x49, 0x49, 0x49, 0x36},     // 8
                                {0x46, 0x49, 0x49, 0x29, 0x1E},     // 9
                                {0x00, 0x00, 0x00, 0x00, 0x00}};    // espacio

    int j, i;
    uint8_t line, old_line;

    // Revisamos el digito de la fuente pixel por pixel con dos bucles, para pintar los que hagan falta

    // Si el argumento 'last_digit' es igual a '\0', no intentes optimizar los pixeles.
    if (last_digit == '\0'){
        for (j = 0; j < 6; ++j)
        {
            for (i = 0; i < 8; ++i)
            {
                if (j == 5) line = 0x00;
                else line = font[num][j];

                if ( (line & (0x01 << i)) != 0 ) LCD_fatPixel(glcd_p, x+j*2,y+i*2,1);     // pintar pixel
                else LCD_fatPixel(glcd_p, x+j*2,y+i*2,0);                               // borrar pixel
            }
        }
    }
    // De otra forma, revisa la fuente del ultimo numero para ver si se parece al del nuevo, y solo cambia los pixeles necesarios
    else{
        for (j = 0; j < 6; ++j){
            for (i = 0; i < 8; ++i){
                if (j == 5) {line = 0x00; old_line = 0x00;}
                else {
                    line = font[num][j];
                    old_line = font[(int)last_digit - 0x30][j];
                    //si el ultimo digito fue vacio, usa el ultimo elemento de la fuente, que es todo vacio.
                    if (last_digit == ' ') old_line = font[10][j];
                    }

                    // Si el pixel de el numero pasado, corresponde con el de este numero. No pintes nada.
                    if ( (line & (0x01 << i)) == (old_line & (0x01 << i))) continue;


                if ( (line & (0x01 << i)) != 0 ) LCD_fatPixel(glcd_p, x+j*2,y+i*2,1);     // pintar pixel
                else LCD_fatPixel(glcd_p, x+j*2,y+i*2,0);                               // borrar pixel
            }
        }



    }


}


// Imprime una linea vertical de dos pixeles de ancho, con (x,y) siendo  la coordenada superios izquierda.
void LCD_fatVline(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t height){

    LCD_drawRect(glcd_p, x, y, x+1, y+height, 1);
}

// Imprime una linea horizontal de dos pixeles de ancho, con (x,y) siendo  la coordenada superios izquierda.
void LCD_fatHline(GLCD *glcd_p, uint8_t x, uint8_t y, uint8_t width){

    LCD_drawRect(glcd_p, x, y, x+width, y+1, 1);
}
