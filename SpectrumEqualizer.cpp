#ifndef SPECTRUMEQUALIZER_CPP
#define SPECTRUMEQUALIZER_CPP

#include "SpectrumEqualizer.h"
#include "application.h"

UDP multicastUDP;
int port;

SpectrumEqualizer::SpectrumEqualizer() {
  port = 32555;
  IPAddress remoteIP(239,1,1,234);

  multicastUDP.begin(port);
  multicastUDP.joinMulticast(remoteIP);
}

void SpectrumEqualizer::readAudioFrequencies() {
    multicastUDP.parsePacket();
    for(int i=0;i<7;i++) {
      int value = multicastUDP.read() << 8 | multicastUDP.read();
      frequenciesLeft[i] = value;
    }
    // Serial.printlnf("frequenciesLeft: [%d, %d, %d, %d]", frequenciesLeft[0],frequenciesLeft[2],frequenciesLeft[4],frequenciesLeft[6]);
}

#endif
