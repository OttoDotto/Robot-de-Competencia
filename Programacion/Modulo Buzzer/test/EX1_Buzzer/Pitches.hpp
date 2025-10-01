#ifndef PITCHES_H
#define PITCHES_H

// Notas fundamentales en octava 4
const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;

// Notas fundamentales en octava 5
const int NOTE_C5 = 523;
const int NOTE_D5 = 587;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;
const int NOTE_G5 = 784;
const int NOTE_A5 = 880;
const int NOTE_B5 = 988;

'''
// Notas fundamentales
const int baseNotes[7] = {262, 294, 330, 349, 392, 440, 494};

// Función para obtener cualquier octava
inline int getNote(int noteIndex, int octave = 4) {
    return baseNotes[noteIndex] * (1 << (octave - 4));
}

// Opcional: nombres para referencia
const char* noteNames[7] = {"C", "D", "E", "F", "G", "A", "B"};
'''

#endif
