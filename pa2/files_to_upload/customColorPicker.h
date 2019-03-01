/**
 * @file customColorPicker.h
 * Definition of a border color picker.
 *
 */
#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include "colorPicker.h"


class customColorPicker : public colorPicker
{
  public:

    customColorPicker(int test);

    virtual HSLAPixel operator()(int x, int y);
};

#endif
