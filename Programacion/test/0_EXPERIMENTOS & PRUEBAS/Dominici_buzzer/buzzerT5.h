

// the number of the LED pin
#define BUZZER_PIN  13  // 16 corresponds to GPIO16

// setting PWM properties
#define freq  1000
#define BUZZER_CHANNEL  4
#define resolution  13

#define NOTE_C    0
#define NOTE_Cs   1
#define NOTE_D    2
#define NOTE_Eb   3
#define NOTE_E    4
#define NOTE_F    5
#define NOTE_Fs   6
#define NOTE_G    7
#define NOTE_Gs   8
#define NOTE_A    9
#define NOTE_Bb   10
#define NOTE_B    11
#define NOTE_MAX  12

double ledcWriteNote(uint8_t, uint8_t, uint8_t);
void silencio(uint16_t);

void buzz1(void);
void buzz2(void);
void buzz3(void);
void buzz4(void);
void buzz5(void);
void buzz6(void);
void buzz7(void);
void buzz8(void);