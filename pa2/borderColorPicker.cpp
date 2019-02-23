#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance, HSLAPixel center) : color(fillColor), im(img), tol(tolerance), ctr(center)
{
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
    // Loop from x-3 < i < x+3 to y-3 < j < y+3 (square centered around given x, y)

    for (int i = x-3; i < x+3; i++) { 
        for (int j = y-3; j < y+3; j++) { 
            // only operate on pixels within a unit circle of radius 3 ((x-a)^2+(y-b)^2 < 9)
        
            if ((i-x)*(i-x) + (j-y)*(j-y) <= 9) { 

                // skip the centre pixel 
                if ((i-x) == 0 && (j-y) == 0) { 
                    return ctr; 
                }

                // next check colour tolerance 
                double tolerance = im.getPixel(i, j)->dist(ctr); 

                if (tolerance > tol) { 
                    // return border colour
                    return color; 
                }
                else { 
                    // return original pixel
                    return (*im.getPixel(i, j)); 
                }
            }
            // check for outside bounds 
            else if ((i-x) < 0 || (i+x) > im.width()) { 
                return color; 
            }
            else if ((j-y) < 0 || (j+y) > im.height()) { 
                return color; 
            }
            else { 
                return (*im.getPixel(i, j)); 
            }
        }
    }
}
