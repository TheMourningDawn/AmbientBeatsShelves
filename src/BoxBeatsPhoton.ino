#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizerClient.h"
#include "AmbientBeatsCloudFunctions.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

SpectrumEqualizerClient *audioEqualizer;
LEDAnimations *animations;
AmbientBeatsCloudFunctions *cloudFunctions;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);


void setup() {
//    connectToRemote();
    audioEqualizer = new SpectrumEqualizerClient();
    animations = new LEDAnimations(audioEqualizer);
    cloudFunctions = new AmbientBeatsCloudFunctions(animations);

    cloudFunctions->setupCloudModeFunctions();

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
    if(animations->poweredOn) {
//      readColorFromRemote();

        animations->runAnimation();
        FastLED.show();
    }
}

//void connectToRemote() {
//   udpMulticast.begin(udpPort);
//   udpMulticast.joinMulticast(udpIP);
//}
//
//void readColorFromRemote() {
//   if(udpMulticast.parsePacket() > 0) {
//       animations->hue = udpMulticast.read() << 8 | udpMulticast.read();
//   }
//}