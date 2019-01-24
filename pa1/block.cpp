#include "block.h"

void Block::build(PNG & im, int column, int width){
  for (int x = column; x < column + width; x++) {
    vector < HSLAPixel > col;
    for (int y = 0; y < (int) im.height(); y++) {
      col.push_back(*im.getPixel(x,y));
    }
    data.push_back(col);
  }
}

void Block::render(PNG & im, int column) const{
  for(int x = 0; x < width(); x++){
    for(int y = 0; y < height(); y++){
      *im.getPixel(x+column, y) = data.at(x).at(y);
    }
  }
}

void Block::greyscale(){
  for (int x = 0; x < width(); x++){
    for (int y = 0; y < height(); y++){
      data.at(x).at(y).s=0;
    }
  }
}

int Block::width() const{
  return data.size();
}

int Block::height() const{
  return data.at(0).size();
}
