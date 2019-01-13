#include "block.h"

namespace cs221util{

Block::build(PNG & im, int column, int width){
  width_ = width;
  height_ = im.height();
  for (unsigned x = column; x < column + width; x++) {
    vector < HSLAPixel > col;
    for (unsigned y = 0; y < im.height(); y++) {
      col.push_back(im.getPixel(x,y));
    }
    data.push_back(col);
  }
}

Block::render(PNG & im, int column){
  for(unsigned x = 0; x < width_; x++){
    for(unsigned y = 0; y < height_; y++){
      im.getPixel(x+column, y) = data.at(x).at(y);
    }
  }
}

Block::greyscale(){
  for (unsigned x = 0; x < width_; x++){
    for (unsigned y = 0; y < height_; y++){
      data.at(x).at(y)->s=0;
    }
  }
}

Block::width(){
  return width_;
}

Block::height(){
  return height_;
}

}
