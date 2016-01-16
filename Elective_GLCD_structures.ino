//Elective_GLCD_structures.ino

#include "LCD_driver.h"
#include "Battery_interface.h"
#include "Motor_interface.h"

char texto[4];

GLCD pan;
GLCD * pantalla;

void setup() {

    pantalla = &pan;

    Serial.begin(9600);

    // put your setup code here, to run once:
    LCD_Init(pantalla);
    delay(2000);    // esperar 5 segundos por la pantalla.

    Mot_initInterface(pantalla);
    delay(200);
    for(int i=0; i<10000; i++){
      Mot_updateRPM(pantalla,i);
      delay(200);
      Mot_updatePhase(pantalla,1,(float)( i%100), 100.0 - i%100);
      delay(200);
      Mot_updatePhase(pantalla,2, 100.0 - i%100, (float) (i%100));
      delay(200);
      Mot_updatePhase(pantalla,3,(float) (i%100), 100.0 - i%100);
      delay(200);
      Mot_updateRC(pantalla,(100.0 - i%100)/10.0);
      delay(50);
      Mot_updateRF(pantalla,(i%100)/10.0);
      delay(50);
      Mot_updateInDC(pantalla,(i%1000)/10.0);
   }
     // Bat_initInterface(pantalla);
     // delay(200);
     // for(int i=999; i>0; i--){
     //   Bat_updateTmax(pantalla, i/10.0);
     //   delay(30);
     //   Bat_updateAout(pantalla, i/10.0);
     //   delay(30);
     //   Bat_updateVbat(pantalla, i/10.0);
     //   delay(30);
     //   Bat_percentageBar(pantalla, i/10);   //division de enteros.
     //   delay(40);
     //   Bat_updateTprom(pantalla, 100.0 - i/10.0);
     //   delay(20);
     //   Bat_updateAin(pantalla, 100.0 - i/10.0);

     // }

}


void loop() {


    for(;;);
}
