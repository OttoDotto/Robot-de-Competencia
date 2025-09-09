#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
  private:
    int pin;

  public:
    Buzzer(int buzzerPin);

    void begin(unsigned int freq = 2000, uint8_t resolution = 8);

    void play(int freq);

    void stop();

    void playTheme(const int *melody, const int *durations, int length) 
  };

#endif
