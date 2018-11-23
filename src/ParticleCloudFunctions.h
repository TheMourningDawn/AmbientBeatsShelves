#ifndef BOXBEATSPHOTON_PARTICLECLOUDFUNCTIONS_H
#define BOXBEATSPHOTON_PARTICLECLOUDFUNCTIONS_H

#include "application.h"
#include "LEDAnimations.h"

class ParticleCloudFunctions
{
    private:
        LEDAnimations *ledAnimations;
    public:
        ParticleCloudFunctions(LEDAnimations *animations);

        void setupCloudModeFunctions();

        // Handlers for particle cloud functions
        int resetDevice(String arg);
        int enterSafeMode(String arg);
        int powerOn(String arg);
        int powerOff(String arg);
        int pause(String arg);

        int toggleAudioReactive(String arg);
        int nextAnimation(String arg);
        int previousAnimation(String arg);
        int setAnimation(String animationNumber);
        int nextFrequency(String frequency);
        int previousFrequency(String frequency);

        int setColor(String rgbString);
        int setHue(String newHue);
        int setSaturation(String saturationString);
        int setBrightness(String brightnessString);
        int setSensitivity(String sensitivity);

        // Handlers for particle cloud events
        void handleReset(const char *eventName, const char *data);
        void handleNextAnimation(const char *eventName, const char *data);
        void handlePreviousAnimation(const char *eventName, const char *data);
};

#endif //BOXBEATSPHOTON_PARTICLECLOUDFUNCTIONS_H
