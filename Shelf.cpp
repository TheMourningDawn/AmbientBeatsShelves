#ifndef SHELF_CPP
#define SHELF_CPP

#include "application.h"
#include "Shelf.h"

CRGB *shelfStrip;

int left = 0;
int right = 0;

Shelf::Shelf() {}

// Shelf::Shelf(CRGB *shelves, int leftIndex, int rightIndex) : shelfStrip(shelves), left(leftIndex), right(rightIndex) {
Shelf::Shelf(CRGB *shelves, int leftIndex, int rightIndex) : shelfStrip(shelves) {
  left = leftIndex;
  right = rightIndex;
}

void Shelf::setRightPixel(CRGB color) {
  shelfStrip[right] = color;
}

void Shelf::setLeftPixel(CRGB color) {
  shelfStrip[left] = color;
  // shelfStrip[left] = CRGB(255,0,0);
}

// void Shelf::shiftRight() {}
// void Shelf::sheftRightBy(int shiftAmount) {}
// void Shelf::shiftLeft() {}
// void Shelf::sheftLeftBy(int shiftAmount) {}

// char currentPatternString[40];
// sprintf(currentPatternString, "%d", left);
// Particle.publish("settinstuff", currentPatternString);

#endif
