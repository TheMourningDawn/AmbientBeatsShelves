#ifndef SPECTRUMEQUALIZER_H
#define SPECTRUMEQUALIZER_H

#include "application.h"

class SpectrumEqualizer {
    public:
      UDP multicastUDP;
      int port;
      IPAddress remoteIP;

      int frequenciesLeft[7];
      SpectrumEqualizer();
      void readAudioFrequencies();
};

#endif
