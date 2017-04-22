#ifndef SPECTRUMEQUALIZER_H
#define SPECTRUMEQUALIZER_H

#include "application.h"

class SpectrumEqualizer {
    public:
      int port = 32555;
      UDP multicastUDP;
      IPAddress remoteIP;

      int frequenciesLeft[7];
      int leftChannelFrequencies[7];
      int rightChannelFrequencies[7];

      int i, value;

      inline SpectrumEqualizer() {
        IPAddress remoteIP(239,1,1,234);

        multicastUDP.begin(port);
        multicastUDP.joinMulticast(remoteIP);
      };

      inline virtual void readAudioFrequencies() {
        multicastUDP.parsePacket();
        for(i=0;i<7;i++) {
          value = multicastUDP.read() << 8 | multicastUDP.read();
          frequenciesLeft[i] = value;
        }
      };

      inline virtual void readLeftChannel() {
        multicastUDP.parsePacket();
        for(i=0;i<7;i++) {
          value = multicastUDP.read() << 8 | multicastUDP.read();
          leftChannelFrequencies[i] = value;
        }
        //We can ignore the rest of the bytes in the packet, since we're not upding the right channel array
      };

      inline virtual void readRightChannel() {
        multicastUDP.parsePacket();
        for(i=0;i<7;i++) {
            //Throw out the leftChannel bytes
            multicastUDP.read();
            multicastUDP.read();
        }
        for(i=0;i<7;i++) {
          value = multicastUDP.read() << 8 | multicastUDP.read();
          rightChannelFrequencies[i] = value;
        }
      };

      inline virtual void readBothChannels() {
        multicastUDP.parsePacket();
        for(i=0;i<7;i++) {
          value = multicastUDP.read() << 8 | multicastUDP.read();
          leftChannelFrequencies[i] = value;
        }
        for(i=0;i<7;i++) {
          value = multicastUDP.read() << 8 | multicastUDP.read();
          rightChannelFrequencies[i] = value;
        }
      }

};

#endif
