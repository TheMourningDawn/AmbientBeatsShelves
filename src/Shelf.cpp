#ifndef SHELF_CPP
#define SHELF_CPP

#include "application.h"
#include "Shelf.h"

CRGB *shelfStrip;

int left;
int right;
int previousPushIndex;

Shelf::Shelf() {}

// Shelf::Shelf(CRGB *shelves, int leftIndex, int rightIndex) : shelfStrip(shelves), left(leftIndex), right(rightIndex) {
Shelf::Shelf(CRGB *shelves, int leftIndex, int rightIndex) : shelfStrip(shelves) {
  left = leftIndex;
  right = rightIndex;
  previousPushIndex = leftIndex;
}

int Shelf::getRightPixelIndex() {
   return right;
}

int Shelf::getLeftPixelIndex() {
    return left;
}

void Shelf::setPixel(int index, CRGB color) {
  shelfStrip[index] = color;
}

void Shelf::setRightPixel(CRGB color) {
  shelfStrip[right] = color;
}

void Shelf::setLeftPixel(CRGB color) {
  shelfStrip[left] = color;
}

CRGB Shelf::getRightPixel() {
  return shelfStrip[right];
}

CRGB Shelf::getLeftPixel() {
  return shelfStrip[left];
}

int Shelf::length() {
  return abs(left - right) + 1;
}

void Shelf::shiftRight() {
  if(left-right < 0) {
    memmove(&shelfStrip[left+1], &shelfStrip[left], (length()-1) * sizeof(CRGB));
  } else  {
    memmove(&shelfStrip[right], &shelfStrip[right+1], (length()-1) * sizeof(CRGB));
  }
}
void Shelf::shiftLeft() {
  if(left-right < 0) {
    memmove(&shelfStrip[left], &shelfStrip[left+1], (length()-1) * sizeof(CRGB));
  } else  {
    memmove(&shelfStrip[right+1], &shelfStrip[right], (length()-1) * sizeof(CRGB));
  }
}

void Shelf::shiftMiddle() {
    if(left < right) {
      memmove(&shelfStrip[left], &shelfStrip[left+1], (length() / 2) * sizeof(CRGB));
      memmove(&shelfStrip[(left + right)/2 + 1], &shelfStrip[(left + right)/2], (length() / 2) * sizeof(CRGB));
    } else {
      memmove(&shelfStrip[right], &shelfStrip[right+1], length() / 2 * sizeof(CRGB));
      memmove(&shelfStrip[(left + right)/2 + 1], &shelfStrip[(left + right)/2], (length() / 2) * sizeof(CRGB));
    }
}

void Shelf::pushRight(CRGB color) {
  if(left-right < 0) {
    previousPushIndex++;
    if(previousPushIndex < left) {
      previousPushIndex = left;
    }
    if(previousPushIndex > right) {
      previousPushIndex = right;
    }
  } else {
    previousPushIndex--;
    if(previousPushIndex > left) {
      previousPushIndex = left;
    }
    if(previousPushIndex < right) {
      previousPushIndex = right;
    }
  }
  shelfStrip[previousPushIndex] = color;
}

void Shelf::pushLeft(CRGB color) {
  if(left-right < 0) {
    previousPushIndex--;
    if(previousPushIndex < left) {
      previousPushIndex = left;
    }
    if(previousPushIndex > right) {
      previousPushIndex = right;
    }
  } else {
    previousPushIndex++;
    if(previousPushIndex > left) {
      previousPushIndex = left;
    }
    if(previousPushIndex < right) {
      previousPushIndex = right;
    }
  }
  shelfStrip[previousPushIndex] = color;
}

void Shelf::fillLeft(CRGB color, int numToFill) {
  if(left-right < 0) {
    int cappedFill = numToFill > length()-1 ? length() - 1 : numToFill;
    for(int i=left;i<left+cappedFill;i++) {
      shelfStrip[i] = color;
    }
  } else {
    int cappedFill = numToFill > length()-1 ? length() - 1 : numToFill;
    for(int i=left;i>left-cappedFill;i--) {
      shelfStrip[i] = color;
    }
  }
}

void Shelf::fillRight(CRGB color, int numToFill) {
  if(left-right < 0) {
    int cappedFill = numToFill > length()-1 ? length() - 1 : numToFill;
    for(int i=right;i>right-cappedFill;i--) {
      shelfStrip[i] = color;
    }
  } else {
    int cappedFill = numToFill > length()-1 ? length() - 1 : numToFill;
    for(int i=right;i<right+cappedFill;i++) {
      shelfStrip[i] = color;
    }
  }
}

#endif
