#include "Arduino.h"
#include "LCD_driver.h"
#include "Motor_interface.h"



// #########################################################################################
// ###                             INTERFAZ DE MOTORES                                   ###
// #########################################################################################

// Dibuja la interfaz en blanco de la informacion de la bateria.
void Mot_initInterface(GLCD *glcd_p){

    //Limpiamos la pantalla de lo que sea que tenga
    LCD_clearScreen(glcd_p);

    // Imprimimos el titulo.
    LCD_print(glcd_p, 13, 0, 15, "Sistema Motores");
    delay(30);
    //Imprimimos la informacion de las fases
    LCD_print(glcd_p, 0,32,6,"F C  V");
    delay(30);
    LCD_print(glcd_p, 0,40,1,"1");
    delay(30);
    LCD_print(glcd_p, 0,48,1,"2");
    delay(30);
    LCD_print(glcd_p, 0,56,1,"3");
    delay(50);

    //Imprimimos RC y RF.
    LCD_print(glcd_p, 50, 44, 2, "Rc");
    delay(30);
    LCD_print(glcd_p, RC_pos_x + 18, RC_pos_y, 1, "v");
    delay(20);
    LCD_print(glcd_p, 50, 55, 2, "Rf");
    delay(20);
    LCD_print(glcd_p, RF_pos_x + 18, RF_pos_y, 1, "v");
    delay(20);

    //imprimimos Indc
    LCD_print(glcd_p, 95, 44, 4, "InDC");
    delay(20);
    LCD_print(glcd_p, InDC_pos_x + 24, InDC_pos_y, 1, "A");
    delay(60);

    //Imprimimos las letras gigantes de RPM
    LCD_print(glcd_p, 95, 25, 3, "RPM");

}

// Actualiza el valor de Tmax.
void Mot_updatePhase(GLCD *glcd_p, uint8_t phase, float current, float voltage){

    // chequeo de limite
    if (current > 99.9) current = 99.9;
    if (voltage > 99.9) voltage = 99.9;

    char str_current[5];
    char str_voltage[5];
    char str_complete[6];

    // transformamos el valor de un flotante a un string
    read4charfloat(str_current, current);
    read4charfloat(str_voltage, voltage);

    // Copiamos las partes relevantes a un solo string.
    str_complete[0] = str_current[0];
    str_complete[1] = str_current[1];
    str_complete[2] = ' ';
    str_complete[3] = str_voltage[0];
    str_complete[4] = str_voltage[1];

    // lo imprimimos en la pantalla
    switch(phase){

        case 1:
            LCD_print(glcd_p, F1_C_pos_x, F1_C_pos_y, 5, str_complete);
            glcd_p->fase_1c = current;
            glcd_p->fase_1v = voltage;
            break;

        case 2:
            LCD_print(glcd_p, F2_C_pos_x, F2_C_pos_y, 5, str_complete);
            glcd_p->fase_2c = current;
            glcd_p->fase_2v = voltage;
            break;

        case 3:
            LCD_print(glcd_p, F3_C_pos_x, F3_C_pos_y, 5, str_complete);
            glcd_p->fase_3c = current;
            glcd_p->fase_3v = voltage;
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

    // Chequeo de limites numericos.
    if (rpm > 9999) rpm = 9999;

    char str_rpm[5];
    uint16_t mils,cent,dec;

    // Primero transformamos el entero 'rpm' en un string con los numeros que se tienen que imprimir, tomando en cuenta los espacios vacios
    // a la izquierda.
    if (rpm < 10) {
        str_rpm[0] = ' ';
        str_rpm[1] = ' ';
        str_rpm[2] = ' ';
        str_rpm[3] = (char) 0x30 + rpm;
    }

    if (rpm >= 10 && rpm <100) {
        dec = rpm / 10;

        str_rpm[0] = ' ';
        str_rpm[1] = ' ';
        str_rpm[2] = (char) (0x30 + dec);
        str_rpm[3] = (char) (0x30 + rpm - dec*10);
    }


    if (rpm >= 100 && rpm <1000) {
        cent = rpm / 100;
        dec  = (rpm - cent*100) / 10;

        str_rpm[0] = ' ';
        str_rpm[1] = (char) (0x30 + cent);
        str_rpm[2] = (char) (0x30 + dec);
        str_rpm[3] = (char) (0x30 + rpm - dec*10 - cent*100);
    }

    if (rpm >= 1000 && rpm <10000) {
        mils = rpm / 1000;
        cent = (rpm - mils*1000) / 100;
        dec  = (rpm - mils*1000 - cent*100) / 10;

        str_rpm[0] = (char) (0x30 + mils);
        str_rpm[1] = (char) (0x30 + cent);
        str_rpm[2] = (char) (0x30 + dec);
        str_rpm[3] = (char) (0x30 + rpm - dec*10 - cent*100 - mils*1000);
    }

    //Ahora que terminamos de calcular los valores que se tienen que imprimir, procedemos a imprimirlos.

    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_rpm[0] != glcd_p->rpm_0){
        // Si es un espacio en blanco, lo borramos.
        if (str_rpm[0] == ' ') LCD_eraseRect(glcd_p, RPM_0_pos_x,RPM_0_pos_y,RPM_0_pos_x + 12, RPM_0_pos_y + 16);
        // Si es un numero, lo dibujamos.
        else LCD_printFatDigit(glcd_p,  glcd_p->rpm_0, RPM_0_pos_x, RPM_0_pos_y, (uint8_t) str_rpm[0] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->rpm_0 = str_rpm[0];


    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_rpm[1] != glcd_p->rpm_1){
        // Si es un espacio en blanco, lo borramos.
        if (str_rpm[1] == ' ') LCD_eraseRect(glcd_p, RPM_1_pos_x,RPM_1_pos_y,RPM_1_pos_x + 12, RPM_1_pos_y + 16);
        // Dibujamos el numero
        LCD_printFatDigit(glcd_p, glcd_p->rpm_1, RPM_1_pos_x, RPM_1_pos_y, (uint8_t) str_rpm[1] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->rpm_1 = str_rpm[1];

    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_rpm[2] != glcd_p->rpm_2){
        // Si es un espacio en blanco, lo borramos.
        if (str_rpm[2] == ' ') LCD_eraseRect(glcd_p, RPM_2_pos_x,RPM_2_pos_y,RPM_2_pos_x + 12, RPM_2_pos_y + 16);
        // Dibujamos el numero
        LCD_printFatDigit(glcd_p, glcd_p->rpm_2, RPM_2_pos_x, RPM_2_pos_y, (uint8_t) str_rpm[2] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->rpm_2 = str_rpm[2];


    // dibujamos solo si el numero que queremos no esta ya dibujado
    if (str_rpm[3] != glcd_p->rpm_3){
        // Dibujamos el numero
        LCD_printFatDigit(glcd_p, glcd_p->rpm_3, RPM_3_pos_x, RPM_3_pos_y, (uint8_t) str_rpm[3] - 0x30);
    }
    // guardamos el nuevo valor
    glcd_p->rpm_3 = str_rpm[3];
}

