#include "stripeColorPicker.h"
#include <math.h>

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing) : color(fillColor), spacing(stripeSpacing)
{
/* your code here! */
}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
/* your code here! */
  if(x%spacing==0) return color;
    HSLAPixel ret;
    ret.h = 100;
    ret.s = 0.0;
    ret.l = 1.0;
    ret.a = 1.0;
    return ret;
}
