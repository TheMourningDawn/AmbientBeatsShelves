#ifndef SEGMENT_H
#define SEGMENT_H

#include <vector>
#include "application.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;

struct Intersection;

class Segment {
  private:
    CRGB *segmentStrip;
    int firstLEDIndex;
    int lastLEDIndex;
    std::vector<Intersection*> intersections;
  public:
    Segment();
    Segment(CRGB *segment, int firstLEDIndex, int lastLEDIndex);

    void addIntersection(int localIntersectionIndex, Segment *intersectingSegment, int intersectionIndex);
    std::vector<Intersection*> getIntersections();
    bool isIntersectionIndex(int index);
    Segment* getIntersectedSegment(int intersectionIndex);

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

struct Intersection {
  int localIntersectionIndex;
  Segment *intersectingSegment;
  int intersectionIndex;
  Intersection(int localIntersectionIndex, Segment *intersectingSegment, int intersectionIndex) : localIntersectionIndex(localIntersectionIndex), intersectingSegment(intersectingSegment), intersectionIndex(intersectionIndex) {}
  bool operator==(const Intersection& compareTo)
  {
    return (this->localIntersectionIndex == compareTo.localIntersectionIndex) && (this->intersectingSegment == compareTo.intersectingSegment) && (this->intersectionIndex == compareTo.intersectionIndex);
  }
};


#endif
