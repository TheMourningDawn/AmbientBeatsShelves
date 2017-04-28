#ifndef SEGMENT_CPP
#define SEGMENT_CPP

#include "Segment.h"

CRGB *segmentStrip;

int firstLEDIndex;
int lastLEDIndex;

std::vector<Intersection> intersections;

//TODO: Change the idea of a Shelf to a Segment
// Sements will be given a strip in which the segment is contained
// Segments will be given the led indexes indicating the begin and end
// Segments could be created via a factory
// Either:
//  Segment factory will need to know all the above,
//  as well as an orientation (Horizontal, Verticle),
//  and which direction the data came from
//      This part is difficult, because I don't know how to easily ask the question.
//      It will depend on where the microcontroller was placed
//  also, it would ideal to be able to know how and where segments intersect.
// Or:
//  Make seperate classes for a Horizontal and Verticle Segements, and decide the above in the constructor.
//
Segment::Segment() {}

/**
    Creates a new Segment

    @param segment Pointer to the CRGB strip in which the segment is contained
    @param firstIndex the index of the LED closest to the data input (smaller number)
    @@param lastIndex The index of the LED furthest away from the data input (later number)
*/
Segment::Segment(CRGB *segment, int firstLEDIndex, int lastLEDIndex) : segmentStrip(segment) {
  firstLEDIndex = firstLEDIndex;
  lastLEDIndex = lastLEDIndex;
}

void Segment::addIntersection(int localIntersectionIndex, Segment *intersectingSegment, int intersectionIndex) {
  Intersection *intersectionToAdd = new Intersection(localIntersectionIndex, intersectingSegment, intersectionIndex);
  intersections.push_back(intersectionToAdd);
}

std::vector<Intersection*> Segment::getIntersections() {
  return intersections;
}

bool Segment::isIntersectionIndex(int index) {
  // std::vector<Intersection> i;
  // for(i=intersections.begin();i!=intersections.end();++i) {
  //   // if(i == index) {
  //     return true;
  //   // }
  // }
  // return false;
  return false;
}

Segment* Segment::getIntersectedSegment(int intersectionIndex) {
  return intersections[intersectionIndex]->intersectingSegment;
}

int Segment::getFirstLEDIndex() {
   return firstLEDIndex;
}

int Segment::getLastLEDIndex() {
    return lastLEDIndex;
}

void Segment::setColor(CRGB color, int index) {
  segmentStrip[index] = color;
}

void Segment::setFirstLEDColor(CRGB color) {
  setColor(firstLEDIndex, color);
}

void Segment::setLastLEDColor(CRGB color) {
  setColor(lastLEDIndex, color);
}

CRGB Segment::getColor(int index) {
  return segmentStrip[index];
}

CRGB Segment::getFirstLEDColor() {
  return getColor(firstLEDIndex);
}

CRGB Segment::getLastLEDColor() {
  return getColor(lastLEDIndex);
}

/**
    Returns the length of the segment

    @returns number of LEDs in the strip
*/
int Segment::getSegmentLength() {
  return abs(firstLEDIndex - lastLEDIndex) + 1;
}

int Segment::length() {
  getSegmentLength();
}

/**
    Moves all LEDs on the strip in the direction of the last index (end) of
    the strip. The value in the last pixel is lost, and the value in the first
    index (begining) is UNCHANGED.
*/
//TODO: make sure this is going the correct direction...
CRGB Segment::shiftTowardLast() {
  Serial.printf("%s\n", "About to memmove");
  memmove(&segmentStrip[firstLEDIndex+1], &segmentStrip[firstLEDIndex], (getSegmentLength()-1) * sizeof(CRGB));
  Serial.printf("%s\n", "Finshed with memmove");
  return CRGB(255,76,98);
}
CRGB Segment::shiftTowardFirst() {
  // CRGB fallOffColor = getColor(firstLEDIndex);
  memmove(&segmentStrip[firstLEDIndex], &segmentStrip[firstLEDIndex+1], (getSegmentLength()-1) * sizeof(CRGB));
  return CRGB(255,76,98);
  // return fallOffColor;
}

/**
    Moves LEDs TO the first and last index FROM the index specified.
    The FROM index does not change, and the values of the first and the last
    LED are lost

    @param index Index from which the value will be shifted
*/
//TODO: Implement
void Segment::shiftFromIndex(int index) {

}
void Segment::shiftFromMiddle() {
  //TODO: either chose a side if there is not exact middle, or use both.
}
void Segment::shiftToIndex() {

}
void Segment::shiftToMiddle() {

}

void Segment::fillFromFirst(CRGB color, int numberOfLEDToFill) {
  //Ensure we're not filling past our bounds
  int cappedFill = numberOfLEDToFill > getSegmentLength()-1 ? getSegmentLength() - 1 : numberOfLEDToFill;
  for(int i=firstLEDIndex;i<firstLEDIndex+cappedFill;i++) {
    segmentStrip[i] = color;
  }
}

void Segment::fillFromLast(CRGB color, int numberOfLEDToFill) {
  //Ensure we're not filling past our bounds
  int cappedFill = numberOfLEDToFill > getSegmentLength()-1 ? getSegmentLength() - 1 : numberOfLEDToFill;
  for(int i=lastLEDIndex;i>lastLEDIndex-cappedFill;i--) {
    segmentStrip[i] = color;
  }
}

void Segment::fillRange(CRGB color, int fromLEDIndex, int toLEDIndex) {

}

int Segment::constrainIndexToSegmentLength(int index) {
  //TODO: finish implementation, constrain to the global strip index, or 0 based off the segment? Probably the former.
  int constrainedIndex = index > getSegmentLength()-1 ? getSegmentLength()-1 : index;
  return constrainedIndex;
}

#endif
