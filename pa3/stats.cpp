#include <typeinfo>
#include <iostream>
#include "stats.h"

stats::stats(PNG & im){
  // initialize 2D arrays to empty
  sumHueX.resize(im.height(), (vector<double>) im.width());
  sumHueY.resize(im.height(), (vector<double>) im.width());
  sumSat.resize(im.height(), (vector<double>) im.width());
  sumLum.resize(im.height(), (vector<double>) im.width());
  hist.resize(im.height(), vector< vector<int> > (im.width(), vector<int>(36)));

  for(unsigned int i = 0; i < im.width(); i++){
    for(unsigned int j = 0; j < im.height(); j++){
      HSLAPixel * pixel = im.getPixel((unsigned int)i, (unsigned int)j);
      double hueX = pixel->s * cos(pixel->h*PI/180);
      double hueY = pixel->s * sin(pixel->h*PI/180);
      double sat = pixel->s;
      double lum = pixel->l;


      int bucket = pixel->h / 10;
      if (bucket == 36) { 
        bucket = 35; 
      }
      for (int k = 0; k < 36; k++) {
        if (k == bucket) { 
          hist.at(i).at(j).at(k) = 1; 
        }
        else { 
          hist.at(i).at(j).at(k) = 0; 
        }
      }
    
      // case 1 - left edge
      if (j > 0 && i==0) { 
        hueX += sumHueX.at(i).at(j-1); 
        hueY += sumHueY.at(i).at(j-1); 
        sat += sumSat.at(i).at(j-1); 
        lum += sumLum.at(i).at(j-1); 
        // add to hist
        for(int k = 0; k < 36; k++){
          hist.at(i).at(j).at(k) += hist.at(i).at(j-1).at(k);
        }
      }
      // case 2 - top edge
      if(j==0 && i > 0){
        hueX += sumHueX.at(i-1).at(j);
        hueY += sumHueY.at(i-1).at(j);
        sat += sumSat.at(i-1).at(j);
        lum += sumLum.at(i-1).at(j);
        // add to hist
        for(int k = 0; k < 36; k++){
          hist.at(i).at(j).at(k) += hist.at(i-1).at(j).at(k);
        }
      }
      // case 3 - general 
      if (j > 0 && i > 0) { 
        hueX += sumHueX.at(i-1).at(j) + sumHueX.at(i).at(j-1) - sumHueX.at(i-1).at(j-1); 
        hueY += sumHueY.at(i-1).at(j) + sumHueY.at(i).at(j-1) - sumHueY.at(i-1).at(j-1);
        sat += sumSat.at(i-1).at(j) + sumSat.at(i).at(j-1) - sumSat.at(i-1).at(j-1);
        lum += sumLum.at(i-1).at(j) + sumLum.at(i).at(j-1) - sumLum.at(i-1).at(j-1);
        // add to hist 
        for (int k = 0; k < 36; k++) { 
          hist.at(i).at(j).at(k) += hist.at(i-1).at(j).at(k) + hist.at(i).at(j-1).at(k) - hist.at(i-1).at(j-1).at(k);
        }
      }
      
      sumHueX.at(i).at(j) = hueX;
      sumHueY.at(i).at(j) = hueY;
      sumSat.at(i).at(j) = sat;
      sumLum.at(i).at(j) = lum;
    }
  }
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */
  return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */
  double alpha = 1.0;
  double totalSat = sumSat[lr.first][lr.second];
  double totalLum = sumLum[lr.first][lr.second];
  double totalX = sumHueX[lr.first][lr.second];
  double totalY = sumHueY[lr.first][lr.second];
  long area = rectArea(ul, lr);

  if(ul.second - 1 >= 0 && ul.first == 0){
    totalSat -= sumSat[lr.first][ul.second-1];
    totalLum -= sumLum[lr.first][ul.second-1];
    totalX -= sumHueX[lr.first][ul.second-1];
    totalY -= sumHueY[lr.first][ul.second-1];
  }
  if(ul.first - 1 >= 0 && ul.second == 0){
    totalSat -= sumSat[ul.first-1][lr.second];
    totalLum -= sumLum[ul.first-1][lr.second];
    totalX -= sumHueX[ul.first-1][lr.second];
    totalY -= sumHueY[ul.first-1][lr.second];
  }
  if(ul.first - 1 >= 0 && ul.second - 1 >= 0){
    totalSat += sumSat[ul.first-1][ul.second-1];
    totalLum += sumLum[ul.first-1][ul.second-1];
    totalX += sumHueX[ul.first-1][ul.second-1];
    totalY += sumHueY[ul.first-1][ul.second-1];
  }

  double saturation = totalSat / area;
  double luminance = totalLum / area;
  double hue = atan2(totalY, totalX) * 180 / PI; // piazza post
  if (hue < 0) { 
    hue += 2*PI; 
  }
  return HSLAPixel (hue, saturation, luminance, alpha);
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

/* your code here */
  vector<int> res (hist[lr.first][lr.second]);
  vector<int> aux;

  if(ul.second - 1 >= 0){
    aux = hist[lr.first][ul.second-1];
    for(int i = 0; i < 36; i++){
      res[i] -= aux[i];
    }
  }
  if(ul.first - 1 >= 0){
    aux = hist[ul.first-1][lr.second];
    for(int i = 0; i < 36; i++){
      res[i] -= aux[i];
    }
  }
  if(ul.first - 1 >= 0 && ul.second - 1 >= 0){
    aux = hist[ul.first-1][ul.second-1];
    for(int i = 0; i < 36; i++){
      res[i] += aux[i];
    }
  }

  return res;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 )
            entropy += ((double) distn[i]/(double) area)
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

  /* your code here */
  // cout << typeid(buildHist(ul, lr)).name() << endl; 
  // cout << typeid((int)rectArea(ul, lr)).name() << endl; 
  vector<int> histo = buildHist(ul, lr); 
  int area = rectArea(ul, lr); 
  return entropy(histo, area);
}
