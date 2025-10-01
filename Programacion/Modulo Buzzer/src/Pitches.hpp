#ifndef PITCHES_H
#define PITCHES_H

// Notas fundamentales
const int baseNotes[7] = {262, 294, 330, 349, 392, 440, 494};

// Función para obtener cualquier octava
inline int getNote(int noteIndex, int octave = 4) {
    return baseNotes[noteIndex] * (1 << (octave - 4));
}

// Opcional: nombres para referencia
const char* noteNames[7] = {"C", "D", "E", "F", "G", "A", "B"};

#endif
