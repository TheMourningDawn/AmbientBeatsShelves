#ifndef PARTICLECLOUDFUNCTIONS_CPP
#define PARTICLECLOUDFUNCTIONS_CPP

#include "ParticleCloudFunctions.h"

LEDAnimations *ledAnimations;

ParticleCloudFunctions::ParticleCloudFunctions(LEDAnimations *animations) : ledAnimations(animations) {}

void ParticleCloudFunctions::setupCloudModeFunctions() {
    // Up to 15 cloud functions may be registered and each function name is limited to a maximum of 12 characters.
    Particle.function("toggle-audio-reactive",  std::bind(&ParticleCloudFunctions::toggleAudioReactive, this, std::placeholders::_1));

    Particle.function("next-animation", std::bind(&ParticleCloudFunctions::nextAnimation, this, std::placeholders::_1));
    Particle.function("previous-animation", std::bind(&ParticleCloudFunctions::previousAnimation, this, std::placeholders::_1));
    Particle.function("set-animation", std::bind(&ParticleCloudFunctions::setAnimation, this, std::placeholders::_1));

    Particle.function("cycle-frequency", std::bind(&ParticleCloudFunctions::nextFrequency, this, std::placeholders::_1));

    Particle.function("set-color", std::bind(&ParticleCloudFunctions::setColor, this, std::placeholders::_1));
    Particle.function("set-hue", std::bind(&ParticleCloudFunctions::setHue, this, std::placeholders::_1));
    Particle.function("set-saturation", std::bind(&ParticleCloudFunctions::setSaturation, this, std::placeholders::_1));
    Particle.function("set-brightness", std::bind(&ParticleCloudFunctions::setBrightness, this, std::placeholders::_1));
    Particle.function("set-sensitivity", std::bind(&ParticleCloudFunctions::setSensitivity, this, std::placeholders::_1));

    Particle.function("reset-device",std::bind(&ParticleCloudFunctions::resetDevice, this, std::placeholders::_1));

    Particle.function("enter-safe-mode", std::bind(&ParticleCloudFunctions::enterSafeMode, this, std::placeholders::_1));

    Particle.function("power-on", std::bind(&ParticleCloudFunctions::powerOn, this, std::placeholders::_1));
    Particle.function("power-off", std::bind(&ParticleCloudFunctions::powerOff, this, std::placeholders::_1));
    Particle.function("pause", std::bind(&ParticleCloudFunctions::pause, this, std::placeholders::_1));

//    Particle.variable("current-hue", &LEDAnimations::currentHue, INT);
//    Particle.variable("current-brightness", &LEDAnimations::currentBrightness, INT);
//    Particle.variable("current-saturation", &LEDAnimations::currentSaturation, INT);
//    Particle.variable("current-animation", &LEDAnimations::currentAnimation, INT);

//    Particle.subscribe("NEXT_MODE", &ParticleCloudFunctions::handleNextAnimation);
//    Particle.subscribe("PREVIOUS_MODE", &ParticleCloudFunctions::handlePreviousAnimation);
//    Particle.subscribe("RESET", &ParticleCloudFunctions::handleReset);
}

int ParticleCloudFunctions::resetDevice(String arg) {
    System.reset();

    return 0;
}

void ParticleCloudFunctions::handleReset(const char *eventName, const char *data) {
    resetDevice("whatever");
}

int ParticleCloudFunctions::enterSafeMode(String arg) {
    System.enterSafeMode();

    return 1;
}

int ParticleCloudFunctions::powerOn(String arg) {
//    poweredOn = true;

    return 1;
}

int ParticleCloudFunctions::powerOff(String arg) {
//    poweredOn = false;

//    animations->clearAllLeds();
//    FastLED.show();

    return 1;
}

int ParticleCloudFunctions::pause(String arg) {
//    poweredOn = !poweredOn;

    return 1;
}

int ParticleCloudFunctions::toggleAudioReactive(String arg) {
    return ledAnimations->toggleAudioReactive();
}

int ParticleCloudFunctions::nextAnimation(String arg) {
    int currentAnimation = ledAnimations->nextAnimation();
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

void ParticleCloudFunctions::handleNextAnimation(const char *eventName, const char *data) {
    nextAnimation("notSureWhyIHaveToDoItLikeThis");
}

int ParticleCloudFunctions::previousAnimation(String arg) {
    int currentAnimation = ledAnimations->previousAnimation();
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

void ParticleCloudFunctions::handlePreviousAnimation(const char *eventName, const char *data) {
    previousAnimation("seriouslyWhy?");
}

int ParticleCloudFunctions::setAnimation(String animationNumber) {
    int currentAnimation = ledAnimations->setAnimation(animationNumber.toInt());
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

int ParticleCloudFunctions::nextFrequency(String frequency) {
    int frequencyMode = ledAnimations->nextFrequencyMode();

    return frequencyMode;
}

int ParticleCloudFunctions::previousFrequency(String frequency) {
    int frequencyMode = ledAnimations->previousFrequencyMode();

    return frequencyMode;
}

//Expects rgb values to be in r,g,b format e.g. 140,200,90
int ParticleCloudFunctions::setColor(String rgbString) {
    char buffer[12];
    rgbString.toCharArray(buffer, 12);
    String r = "";
    String g = "";
    String b = "";

    int rgbItem = 0;
    for(int i=0;i<12;i++) {
        if(buffer[i] != ',') {
            if(rgbItem == 0) {
                r.concat(buffer[i]);
            } else if(rgbItem == 1) {
                g.concat(buffer[i]);
            } else if(rgbItem == 2) {
                b.concat(buffer[i]);
            }
        } else {
            rgbItem++;
        }
    }

    CRGB rgb = CRGB(r.toInt(),g.toInt(),b.toInt());
    CHSV hsv = rgb2hsv_approximate(rgb);

    ledAnimations->currentHue = hsv.hue;

    return hsv.hue;
}

int ParticleCloudFunctions::setHue(String newHue) {
    ledAnimations->currentHue = newHue.toInt();

    return ledAnimations->currentHue;
}

int ParticleCloudFunctions::setSaturation(String saturationString) {
    ledAnimations->setCurrentSaturation(saturationString.toInt());

    return 1;
}

int ParticleCloudFunctions::setBrightness(String brightnessString) {
    ledAnimations->setCurrentBrightness(brightnessString.toInt());

    return 1;
}

int ParticleCloudFunctions::setSensitivity(String sensitivity) {
    ledAnimations->globalSensitivity = sensitivity.toInt();

    return sensitivity.toInt();
}

#endif
