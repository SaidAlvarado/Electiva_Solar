#include "Arduino.h"
#include "LCD_driver.h"
#include "Motor_interface.h"



// #########################################################################################
// ###                             INTERFAZ DE MOTORES                                   ###
// #########################################################################################

// Dibuja la interfaz en blanco de la informacion de la bateria.
void Mot_initInterface(GLCD *glcd_p){

    // Imprimimos el titulo.
    LCD_print(glcd_p, 13, 0, 15, "Sistema Motores");
    delay(30);
    //Imprimimos la informacion de las fases
    LCD_print(glcd_p, 0,32,6,"F C  V");
    delay(30);
    LCD_print(glcd_p, 0,40,7,"1 88 88");
    delay(30);
    LCD_print(glcd_p, 0,48,7,"2 88 88");
    delay(30);
    LCD_print(glcd_p, 0,56,7,"3 88 88");
    delay(50);

    //Imprimimos RC y RF.
    LCD_print(glcd_p, 50, 44, 2, "Rc");
    delay(30);
    LCD_print(glcd_p, RC_pos_x, RC_pos_y, 4, "8.8v");
    delay(20);
    LCD_print(glcd_p, 50, 55, 2, "Rf");
    delay(20);
    LCD_print(glcd_p, RF_pos_x, RF_pos_y, 4, "8.8v");
    delay(20);

    //imprimimos Indc
    LCD_print(glcd_p, 95, 44, 4, "InDC");
    delay(20);
    LCD_print(glcd_p, InDC_pos_x, InDC_pos_y, 5, "88.8A");
    delay(60);

    //Imprimimos las letras gigantes de RPM
    LCD_printFatDigit(glcd_p, '\0', RPM_0_pos_x, RPM_0_pos_y, 8);
    LCD_printFatDigit(glcd_p, '\0', RPM_1_pos_x, RPM_1_pos_y, 8);
    LCD_printFatDigit(glcd_p, '\0', RPM_2_pos_x, RPM_2_pos_y, 8);
    LCD_printFatDigit(glcd_p, '\0', RPM_3_pos_x, RPM_3_pos_y, 8);
    delay(20);
    LCD_print(glcd_p, 95, 25, 3, "RPM");

}

// Actualiza el valor de Tmax.
void Mot_updatePhase(GLCD *glcd_p, uint8_t phase, float current, float voltage){

    // chequeo de limite
    if (current > 99.9) current = 99.9;
    if (voltage > 99.9) voltage = 99.9;

    char str_current[5];
    char str_voltage[5];

    //transformamos el valor de un flotante a un string
    read4charfloat(str_current, current);
    read4charfloat(str_voltage, voltage);
    //lo imprimimos en la pantalla

    switch(phase){

        case 1:
            LCD_print(glcd_p, F1_C_pos_x, F1_C_pos_y, 2, str_current);
            delay(40);
            LCD_print(glcd_p, F1_V_pos_x, F1_V_pos_y, 2, str_voltage);
            break;

        case 2:
            LCD_print(glcd_p, F2_C_pos_x, F2_C_pos_y, 2, str_current);
            delay(40);
            LCD_print(glcd_p, F2_V_pos_x, F2_V_pos_y, 2, str_voltage);
            break;

        case 3:
            LCD_print(glcd_p, F3_C_pos_x, F3_C_pos_y, 2, str_current);
            delay(40);
            LCD_print(glcd_p, F3_V_pos_x, F3_V_pos_y, 2, str_voltage);
            break;

        default:
            break;
    }

}

// Actualiza el valor de Tprom.
void Mot_updateRC(GLCD *glcd_p, float RC){

    // chequeo de limite
    if (RC > 9.9) RC = 9.9;

    // Pointer check
    if (glcd_p->rc == RC && glcd_p->rc != 0) return;

    char str_RC[4];

    //transformamos el valor de un flotante a un string
    read3charfloat(str_RC, RC);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, RC_pos_x, RC_pos_y, 3, str_RC);
    glcd_p->rc = RC;
}

// Actualiza el valor de la Corriente de entrada.
void Mot_updateRF(GLCD *glcd_p, float RF){

    // chequeo de limite
    if (RF > 9.9) RF = 9.9;

    // Pointer check
    if (glcd_p->rf == RF && glcd_p->rf != 0) return;

    char str_RF[4];

    //transformamos el valor de un flotante a un string
    read3charfloat(str_RF, RF);
    //lo imprimimos en la pantalla
    LCD_print(glcd_p, RF_pos_x, RF_pos_y, 3, str_RF);
    glcd_p->rf = RF;
}

// Actualiza el valor de la Corriente de Salida.
void Mot_updateInDC(GLCD *glcd_p, float InDC){

   // chequeo de limite
   if (InDC > 99.9) InDC = 99.9;

   // Pointer check
   if (glcd_p->indc == InDC && glcd_p->indc != 0) return;

   char str_InDC[5];

   //transformamos el valor de un flotante a un string
   read4charfloat(str_InDC, InDC);
   //lo imprimimos en la pantalla
   LCD_print(glcd_p, InDC_pos_x, InDC_pos_y, 4, str_InDC);
   glcd_p->indc = InDC;
}

// Actualiza el estado de la barrita de porcentaje.
void Mot_updateRPM(GLCD *glcd_p, uint16_t rpm){

    uint16_t mils,cent,dec;
    //lo imprimimos en la pantalla
    if (rpm < 10) { //Si el numero es muy pequeno, imprime solo dos digitos.

        LCD_eraseRect(glcd_p, RPM_0_pos_x,RPM_0_pos_y,RPM_0_pos_x + 12, RPM_0_pos_y + 16);
        delay(30);
        LCD_eraseRect(glcd_p, RPM_1_pos_x,RPM_1_pos_y,RPM_1_pos_x + 12, RPM_1_pos_y + 16);
        delay(30);
        LCD_eraseRect(glcd_p, RPM_2_pos_x,RPM_2_pos_y,RPM_2_pos_x + 12, RPM_2_pos_y + 16);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_3_pos_x, RPM_3_pos_y, rpm);// escribi esta funcion para que recibiera enteros... asi que casting!
    }

    if (rpm >= 10 && rpm <100) {

        dec = rpm / 10;

        LCD_eraseRect(glcd_p, RPM_0_pos_x,RPM_0_pos_y,RPM_0_pos_x + 12, RPM_0_pos_y + 16);
        delay(30);
        LCD_eraseRect(glcd_p, RPM_1_pos_x,RPM_1_pos_y,RPM_1_pos_x + 12, RPM_1_pos_y + 16);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_2_pos_x, RPM_2_pos_y, dec);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_3_pos_x, RPM_3_pos_y, rpm - dec*10);
    }


    if (rpm >= 100 && rpm <1000) {

        cent = rpm / 100;
        dec  = (rpm - cent*100) / 10;

        LCD_eraseRect(glcd_p, RPM_0_pos_x,RPM_0_pos_y,RPM_0_pos_x + 12, RPM_0_pos_y + 16);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_1_pos_x, RPM_1_pos_y, cent);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_2_pos_x, RPM_2_pos_y, dec);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_3_pos_x, RPM_3_pos_y, rpm - dec*10 - cent*100);
    }

    if (rpm >= 1000 && rpm <10000) {

        mils = rpm / 1000;
        cent = (rpm - mils*1000) / 100;
        dec  = (rpm - mils*1000 - cent*100) / 10;

        LCD_printFatDigit(glcd_p, '\0', RPM_0_pos_x, RPM_0_pos_y, mils);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_1_pos_x, RPM_1_pos_y, cent);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_2_pos_x, RPM_2_pos_y, dec);
        delay(30);
        LCD_printFatDigit(glcd_p, '\0', RPM_3_pos_x, RPM_3_pos_y, rpm - dec*10 - cent*100 - mils*1000);
    }

}

