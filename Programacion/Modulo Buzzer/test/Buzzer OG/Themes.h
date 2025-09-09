#ifndef THEMES_H
#define THEMES_H

// Notas musicales (frecuencia en Hz)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 830
#define NOTE_A5  880
#define NOTE_DS4 311
#define NOTE_D4  294
#define NOTE_B3  247
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_C5  523
#define NOTE_E5  659

// ----------------- MARCHA IMPERIAL -----------------
int imperialMelody[] = {
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,
  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5,
  NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4
};

int imperialLength = sizeof(imperialMelody) / sizeof(imperialMelody[0]);

int imperialDurations[] = {
  4,4,4,4,8,
  4,4,8,2,
  4,4,4,4,8,
  4,4,8,2
};



#endif
