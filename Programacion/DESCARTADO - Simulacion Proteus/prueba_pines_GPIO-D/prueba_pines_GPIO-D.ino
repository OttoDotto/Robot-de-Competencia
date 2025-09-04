// Lista de GPIOs a testear
/*
  int pines[] = {
    2, 4, 5, 12, 13, 14, 15, 16, 17,
    18, 19, 21, 22, 23, 25, 26, 27,
    32, 33
  };
*/

int pines[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

// sizeof() devuelve nro de Bytes, Y al tamano del arreglo lo dividimos por el nro de un INT = nro de pines
int totalPines = sizeof(pines) / sizeof(pines[0]);

void setup() {
  for (int i = 0; i < totalPines; i++) {
    pinMode(pines[i], OUTPUT);
    digitalWrite(pines[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < totalPines; i++) {
    digitalWrite(pines[i], HIGH);
    delay(50);      // lo hago en pasos
    digitalWrite(pines[i], LOW);
  }
}
