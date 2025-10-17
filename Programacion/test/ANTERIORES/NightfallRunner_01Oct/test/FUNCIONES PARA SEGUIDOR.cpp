
#include <Arduino.h>

#define     SENSOR1     0       // pin sel sensor 1.
#define     SENSOR2     1
#define     SENSOR3     2
#define     SENSOR4     3
#define     SENSOR5     4

const int pinSensor[] = {SENSOR1, SENSOR1, SENSOR2, SENSOR3, SENSOR4}; // vector de pines sensores.

int sensores[5];        // valores medidos por los sensores
int digital[5];         // toma el valor de "sensores[5]" y de "umbrales[5]" para convertirlo en digital.
int lectura_fondo[5];   // variables para calibrar el umbral
int lectura_linea[5];   // variables para calibrar el umbral
int umbrales[5];        // umbral de calibración digital.
int sumaPonderada = 0;  // suma de productos entre Sensor que detecta y su peso.
int sumaDetector = 0;   // suma de sensores que detectan.
int pos = 0;            // posición detectada.
int poslast = 0;        // última posición detectada.
int position;

// -------- Configuración de los colores ---------
// 0 - Línea Blanca con fondo de pista negro
// 1 - Línea Negra con fondo de pista blanco
int colores = 1;
// -----------------------------------------------

int lecturaSensores(){
    for(int i = 0; i < 5; i++){
        sensores[i]= analogRead(pinSensor[i]);
        if(colores == 0){
            if(sensores[i] <= umbrales[i])
                digital[i] = 0;
            else
                digital[i] = 1;
        }
        if(colores == 1){
            if(sensores[i] <= umbrales[i])
                digital[i] = 1;
            else
                digital[i] = 0;
        }
        //Serial.print(digital[i]);
        //Serial.print("\t");
    }

    //Serial.println(" ");
    sumaPonderada = 0 * digital[0] + 100 * digital[1] + 200 * digital[2] + 300 * digital[3] + 400 * digital[4];
    sumaDetector = digital[0] + digital[1] + digital[2] + digital[3] + digital[4];
    pos = sumaPonderada / sumaDetector;

    if(poslast <= 100 && pos == -1)
        pos = 0;
    if(poslast >= 300 && pos == -1)
        pos = 400;

    poslast = pos;
    return pos;
}


void fondos(){
    for(int i = 0; i<5; i++){
        lectura_fondo[i]= analogRead(pinSensor[i]);
        //Serial.print(lectura_fondo[i]);
        //Serial.print("\t");
    }
//Serial.println(" ");
}


void lineas(){
    for(int i = 0; i < 5; i++){
        lectura_linea[i]= analogRead(pinSensor[i]);
        //Serial.print(lectura_linea[i]);
        //Serial.print("\t");
    }
//Serial.println(" ");
}

void promedio(){
    for(int i = 0; i < 5; i++){
        umbrales[i] = (lectura_fondo[i] + lectura_linea[i]) / 2;
        //Serial.print(umbrales[i]);
        //Serial.print("\t");
    }
//Serial.println(" ");
}