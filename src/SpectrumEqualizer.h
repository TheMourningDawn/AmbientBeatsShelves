#ifndef SPECTRUMEQUALIZER_H
#define SPECTRUMEQUALIZER_H

#include "application.h"

class SpectrumEqualizer {
    public:
      UDP multicastUDP;
      int port;
      IPAddress remoteIP;

      int frequenciesLeft[7];

      inline SpectrumEqualizer() {
        port = 32555;
        IPAddress remoteIP(239,1,1,234);

        multicastUDP.begin(port);
        multicastUDP.joinMulticast(remoteIP);
      };

      inline virtual void readAudioFrequencies() {
        multicastUDP.parsePacket();
        for(int i=0;i<7;i++) {
          int value = multicastUDP.read() << 8 | multicastUDP.read();
          frequenciesLeft[i] = value;
        }
      };

};

#endif
