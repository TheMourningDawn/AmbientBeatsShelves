#ifndef HORIZONTALORIENTATION_H
#define HORIZONTALORIENTATION_H
/*
   enum that defines which direction the data is going with respect to the ground
   DATA_DOWN would be the source of data at the top, or the arrows pointed down. The data is traveling down.
*/
struct HorizontalOrientation {
  enum orientation { DATA_RIGHT, DATA_LEFT };
};

#endif
