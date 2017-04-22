#ifndef SEGMENT_H
#define SEGMENT_H

#include "application.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;


class Segment {
private:
  CRGB *segmentStrip;
  int firstLEDIndex;
  int lastLEDIndex;
public:
  Segment();
  Segment(CRGB *segment, int firstLEDIndex, int lastLEDIndex);
  int getFirstLEDIndex();
  int getLastLEDIndex();
  void setColor(CRGB color, int index);
  void setFirstLEDColor(CRGB color);
  void setLastLEDColor(CRGB color);
  CRGB getColor(int index);
  CRGB getFirstLEDColor();
  CRGB getLastLEDColor();
  int getSegmentLength();
  int length();
  CRGB shiftTowardLast();
  CRGB shiftTowardFirst();
  void shiftFromIndex(int index);
  void shiftFromMiddle();
  void shiftToIndex();
  void shiftToMiddle();
  void fillFromFirst(CRGB color, int numberOfLEDToFill);
  void fillFromLast(CRGB color, int numberOfLEDToFill);
  void fillRange(CRGB color, int fromLEDIndex, int toLEDIndex);
  int constrainIndexToSegmentLength(int index);
};

#endif
