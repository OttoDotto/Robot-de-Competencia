#include <Arduino.h>
#include <buzzerT5.h>

double ledcWriteNote(uint8_t chan, uint8_t note, uint8_t octave){
  const uint16_t noteFrequencyBase[12] = {
  //   C        C#       D        Eb       E        F       F#        G       G#        A       Bb        B
      4186,    4435,    4699,    4978,    5274,    5588,    5920,    6272,    6645,    7040,    7459,    7902
  };

  if(octave > 8 || note >= NOTE_MAX){
      //return 0;
      return ledcWriteTone(chan, 0);
  }
  double noteFreq =  (double)noteFrequencyBase[note] / (double)(1 << (8-octave));
  return ledcWriteTone(chan, noteFreq);
}

void silencio(uint16_t time){
  ledcWriteNote(BUZZER_CHANNEL, NOTE_MAX, 5);
  delay(time);
}


void buzz1(void){
  // ESCALA
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 5);
  delay(500);
}


void buzz2(void){
  // Cumpleaños feliz
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(500);
}

void buzz3(void){
// SMOKE ON THE WATER
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 6);
  delay(700);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Cs, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 6);
  delay(700);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 6);
  delay(700);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Bb, 5);
  delay(600);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(600);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_MAX, 5);
  delay(500);
}

void buzz4(void){
// Final Fantasy
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Eb, 5);
  delay(200);
  silencio(20);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Eb, 5);
  delay(200);
  silencio(20);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Eb, 5);
  delay(200);
  silencio(20);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
  delay(700);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Cs, 5);
  delay(300);
  silencio(30);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Eb, 5);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Cs, 5);
  delay(100);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Eb, 5);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_MAX, 5);
  delay(2000);
}

void buzz5(void){
// LA CUCARACHA
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(700);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 5);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_MAX, 5);
  delay(500);
}

void buzz6(void){
// Terminator
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(800);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(800);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(800);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  delay(200);
  silencio(800);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 4);
  delay(800);
  silencio(400);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 5);
  delay(800);
  silencio(100);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 5);
  delay(800);
  silencio(400);

  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 5);
  delay(800);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 5);
  delay(400);
  silencio(10);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 4);
  delay(800);
  silencio(1200);
}

void buzz7(void){
// MARIO COIN
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 7);
  // delay(100);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 8);
  // delay(200);
  // silencio(200);
// Variante MArio Coin
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 8);
  delay(100);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 8);
  delay(200);
  silencio(200);
}

void buzz8(void){
//1-UP
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 7);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 7);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 7);
  delay(200);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 7);
  delay(200);
  silencio(400);
}

void buzzHarryPotter(void) {
  // Primer segmento (intro icónica)
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 5); 
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 6);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(800);
  silencio(200);

  // Segundo segmento (variación melódica)
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 5);
  delay(400);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(600);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(600);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 6);
  delay(400);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 7);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Cs, 7);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 7);
  delay(600);
  silencio(100);

  // Tercer segmento (clímax)
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(800);
  silencio(200);

  // Repetición de la intro para cerrar
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 5); 
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 6);
  delay(300);
  silencio(50);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_Fs, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 6);
  delay(300);
  ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 6);
  delay(1000); // Nota final más larga
  silencio(500);
}