
#include "stats.h"

stats::stats(PNG & im){

/* your code here */
  // initialize 2D arrays to empty
  for(int i = 0; i < im.width(); i++){
    vector<double> colHueX (im.height());
    vector<double> colHueY (im.height());
    vector<double> colSat (im.height());
    vector<double> colLum (im.height());
    vector<vector<int>> colHist (im.height());

    sumHueX.emplace_back(colHueX);
    sumHueY.emplace_back(colHueY);
    sumSat.emplace_back(colSat);
    sumLum.emplace_back(colLum);
    hist.emplace_back(colHist);
  }

  for(int i = 0; i < im.width(); i++){
    for(int j = 0; j < im.height(); j++){
      HSLAPixel * pixel = im.getPixel((unsigned int)i, (unsigned int)j);
      double hueX = pixel->s * cos(pixel->h);
      double hueY = pixel->s * sin(pixel->h);
      double sat = pixel->s;
      double lum = pixel->l;

      if(i != 0){
        hueX += colHueX.at(i-1).at(j);
        hueY += colHueY.at(i-1).at(j);
        sat += colSat.at(i-1).at(j);
        lum += colLum.at(i-1).at(j);
      }

      if(j != 0){
        hueX += colHueX.at(i).at(j-1);
        hueY += colHueY.at(i).at(j-1);
        sat += colSat.at(i).at(j-1);
        lum += colLum.at(i).at(j-1);
      }

      colHueX[i][j] = hueX;
      colHueY[i][j] = hueY;
      colSat[i][j] = sat;
      colLum[i][j] = lum;

      vector<int> freqs (36);
      if(i != 0){
        for(int k = 0; k < 36; k++){
          freqs[k] = hist.at(i-1).at(j)[k];
        }
      }
      for(int k = 0; k <= j; k++){
        HSLAPixel * otherPixel = im.getPixel((unsigned int)i, (unsigned int)k);
        int bin = otherPixel->h % 36;
        freqs[bin]++;
      }

      hist[i][j] = freqs;
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
  doube totalSat = sumSat[lr.first][lr.second];
  double totalLum = sumLum[lr.first][lr.second];
  double totalX = sumHueX[lr.first][lr.second];
  double totalY = sumHueY[lr.first][lr.second];
  long area = rectArea(ul, lr);

  if(ul.second - 1 >= 0){
    totalSat -= sumSat[lr.first][ul.second-1];
    totalLum -= sumLum[lr.first][ul.second-1];
    totalX -= sumHueX[lr.first][ul.second-1];
    totalY -= sumHueY[lr.first][ul.second-1];
  }
  if(ul.first - 1 >= 0){
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
  double hue = atan2(totalY/area, totalX/area) * 180 / PI; // piazza post

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
  return entropy(buildHist(ul, lr), (int) rectArea(ul, lr));
}
