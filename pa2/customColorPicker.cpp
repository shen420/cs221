#include "customColorPicker.h"

customColorPicker::customColorPicker()
{
/* Your code here! */
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{

/* Your code here! */
  HSLAPixel ret;
  ret.h = ((x+y) % 360) * 4;
  ret.s = 0.5;
  ret.l = 0.5;
  ret.a = 1.0;
  return ret;
}
