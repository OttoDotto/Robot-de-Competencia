#include "interrupciones.hpp"

volatile bool RUN = false;

void IRAM_ATTR handleRun()  { RUN = true;  }
void IRAM_ATTR handleStop() { RUN = false; }
