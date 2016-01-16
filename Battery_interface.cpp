#include "Arduino.h"
#include "LCD_driver.h"
#include "Battery_interface.h"



// #########################################################################################
// ###                             INTERFAZ DE BATERIAS                                  ###
// #########################################################################################

// Dibuja la interfaz en blanco de la informacion de la bateria.
void Bat_initInterface(GLCD *glcd_p){

    int i,j;

    //Actualizamos el apuntador
    glcd_p->bat_iface    = 1;
    glcd_p->mot_iface    = 0;

    // Imprimimos el titulo.
    LCD_print(glcd_p, 13, 0, 16, "Sistema Baterias");

    //Imprimimos la barra de porcentaje
    LCD_print(glcd_p, 113,7,1,"%");
    LCD_drawRect(glcd_p, percent_bar_top_x, percent_bar_top_y, percent_bar_bottom_x, percent_bar_bottom_y, 1);


    // Imprimimos Tmax y T prom
    delay(80);
    LCD_print(glcd_p, 0, 15, 4, "Tmax");
    LCD_print(glcd_p, tmax_pos_x, tmax_pos_y, 4, "88.8");
    delay(40);
    LCD_drawCircle(glcd_p, 26,25,1,1);  // circulito de los grados centigrados
    LCD_drawCircle(glcd_p, 26,48,1,1);
    delay(30);
    LCD_print(glcd_p, 0, 38, 5, "Tprom");
    LCD_print(glcd_p, tprom_pos_x, tprom_pos_y, 4, "88.8");
    LCD_print(glcd_p, 29, 25, 1, "C");
    LCD_print(glcd_p, 29, 48, 1, "C");

    //imprimimos los datos de corriente de abajo
    delay(45);
    LCD_print(glcd_p, 45,56,9,"Out 88.8A");
    delay(30);
    LCD_print(glcd_p, 45,46,9,"In  88.8A");

    //Imprimimos los digitos gordos del voltaje
    delay(80);
    LCD_printFatDigit(glcd_p, '\0', vbat_0_pos_x, vbat_0_pos_y, 8);
    LCD_printFatDigit(glcd_p, '\0', vbat_1_pos_x, vbat_1_pos_y, 8);
    LCD_fatPixel(glcd_p, 71, 33, 1);
    LCD_printFatDigit(glcd_p, '\0', vbat_2_pos_x, vbat_2_pos_y, 8);
    LCD_print(glcd_p, 87,28,1, "V");
    delay(30);
    // Dibujamos los indicadores de nivel al lado de la barra
    for (i = 0; i < 5; ++i) {

        j = percent_bar_top_y + i*(percent_bar_bottom_y - percent_bar_top_y )/4;
        LCD_drawLine(glcd_p, percent_bar_bottom_x + 2, j + (i == 0) - (i == 4), percent_bar_bottom_x + 3 + (i == 2 | i == 0 | i == 4), j + (i == 0) - (i == 4), 1);

    }
}


// Actualiza el valor de Tmax.
void Bat_updateTmax(GLCD *glcd_p, float tmax){

    // chequeo de limite
    if (tmax > 99.9) tmax = 99.9;

    // Pointer check
    if (glcd_p->tmax == tmax && glcd_p->tmax != 0) return;

    char str_tmax[5];

    //transformamos el valor de un flotante a un string
    read4charfloat(str_tmax, tmax);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, tmax_pos_x, tmax_pos_y, 4, str_tmax);
    glcd_p->tmax = tmax;
}

// Actualiza el valor de Tprom.
void Bat_updateTprom(GLCD *glcd_p, float tprom){

    // chequeo de limite
    if (tprom > 99.9) tprom = 99.9;

    // Pointer check
    if (glcd_p->tprom == tprom && glcd_p->tprom != 0) return;

    char str_tprom[5];

    //transformamos el valor de un flotante a un string
    read4charfloat(str_tprom, tprom);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, tprom_pos_x, tprom_pos_y, 4, str_tprom);
    glcd_p->tprom = tprom;

}

// Actualiza el valor de la Corriente de entrada.
void Bat_updateAin(GLCD *glcd_p, float ain){

    // chequeo de limite
    if (ain > 99.9) ain = 99.9;

    // Pointer check
    if (glcd_p->ina == ain && glcd_p->ina != 0) return;

    char str_ain[5];

    //transformamos el valor de un flotante a un string
    read4charfloat(str_ain, ain);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, ain_pos_x, ain_pos_y, 4, str_ain);
    glcd_p->ina = ain;
}

// Actualiza el valor de la Corriente de Salida.
void Bat_updateAout(GLCD *glcd_p, float aout){

    // chequeo de limite
    if (aout > 99.9) aout = 99.9;

    // Pointer check
    if (glcd_p->outa == aout && glcd_p->outa != 0) return;

    char str_aout[5];

    //transformamos el valor de un flotante a un string
    read4charfloat(str_aout, aout);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, aout_pos_x, aout_pos_y, 4, str_aout);
    glcd_p->outa = aout;
}

// Actualiza el estado de la barrita de porcentaje.
void Bat_percentageBar(GLCD *glcd_p, uint8_t percent){

    if (percent > 100) percent = 100;

    uint8_t barra, barra_altura_abs, i;
    int diff_bar;

    //height of the bar 44 pixels.
    barra = percent * 44 / 100;

    // Pointer check, a ver si no hace falta dibujar nada.
    if (glcd_p->percent_bar == barra && glcd_p->percent_bar != 0) return;

    // Calculamos la diferencia entre la barra actual y la nueva
    diff_bar = barra - glcd_p->percent_bar;

    // Tambien calculamos la altura absoluta de la barra actual (porque es util para no repetir cuentas)
    barra_altura_abs = percent_bar_bottom_y - glcd_p->percent_bar;

    //Borramos y dibujamos un pedacito de la barra dependiendo de donde este la nueva barra con respecto a la anterior
    if (diff_bar < 0){
        LCD_eraseRect(glcd_p, percent_bar_top_x + 1, barra_altura_abs, percent_bar_bottom_x - 1, barra_altura_abs - diff_bar - 1);
        // LCD_eraseRect(glcd_p, percent_bar_top_x + 1, barra_altura_abs + 1, percent_bar_bottom_x - 1, barra_altura_abs - diff_bar - 1);
    }
    else{
        // Si el crecimiento de la barra es muy grande, dibujalo con barras verticales.
        if (diff_bar > 12){
            for (i = 0; i < 6; ++i){
                delay(30);  // no podemos mandar tantos datos por serial juntos.
                LCD_fatVline(glcd_p, percent_bar_top_x + i*2, percent_bar_bottom_y - barra, diff_bar);
            }
        }

        //Si la barra no crece mucho, dibujala horizontalmente.
        if (diff_bar <= 12){
            for (i = 1; i <= diff_bar; ++i){
                // Como las lineas horizonales tienen 2 pixeles de grosor, las dibujamos solo en los intervalos impares de 'i'.
                if (i%2 != 0){
                    delay(30);  // no podemos mandar tantos datos por serial juntos.
                    LCD_fatHline(glcd_p, percent_bar_top_x, barra_altura_abs - i, percent_bar_bottom_x - percent_bar_top_x);
                }
            }
        }
    }

    //Actualizamos la estructura de la pantalla.
    glcd_p->percent_bar = barra;

    // Esta era la implementacion vieja.

    // // Limpiar La barra actual, pero solo la parte de adentro, no el bordecito (para eso son los +- 1).
    // LCD_eraseRect(percent_bar_top_x + 1, percent_bar_top_y + 1, percent_bar_bottom_x - 1, percent_bar_bottom_y - 1);
    // //usamos las lineas gordas para llenar la barra al lugar correcto.
    // for (i = 0; i < 6; ++i){

    //     delay(30);  // no podemos mandar tantos datos por serial juntos.
    //     LCD_fatVline(percent_bar_top_x + i*2, barra_altura_abs, barra);
    // }
}


// Actualiza el estado del voltaje de la bateria.
void Bat_updateVbat(GLCD *glcd_p, float vbat){

    // chequeo de limites numericos.
    if (vbat > 100.0) vbat = 100.0;

    char str_vbat[5];

    // transformamos el valor de un flotante a un string
    read4charfloat(str_vbat, vbat);
    // lo imprimimos en la pantalla, analizamos cada letra por separado.

    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_vbat[0] != glcd_p->v_0){
        // Si es un espacio en blanco, lo borramos.
        if (str_vbat[0] == ' ') LCD_eraseRect(glcd_p, vbat_0_pos_x,vbat_0_pos_y,vbat_0_pos_x + 12, vbat_0_pos_y + 16);
        // Si es un numero, lo dibujamos.
        else LCD_printFatDigit(glcd_p,  '\0', vbat_0_pos_x, vbat_0_pos_y, (uint8_t) str_vbat[0] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->v_0 = str_vbat[0];


    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_vbat[1] != glcd_p->v_1){
        // Dibujamos el numero
        LCD_printFatDigit(glcd_p, glcd_p->v_1, vbat_1_pos_x, vbat_1_pos_y, (uint8_t) str_vbat[1] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->v_1 = str_vbat[1];

    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_vbat[3] != glcd_p->v_2){
        // Dibujamos el numero
        LCD_printFatDigit(glcd_p, glcd_p->v_2, vbat_2_pos_x, vbat_2_pos_y, (uint8_t) str_vbat[3] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->v_2 = str_vbat[3];


}





//0800-sos-zoom    5
