#include <typeinfo>
#include <iostream>
#include "stats.h"

stats::stats(PNG & im){

/* your code here! */
	sumHueX.resize(im.width(), (vector<double>) im.height());
  sumHueY.resize(im.width(), (vector<double>) im.height());
  sumSat.resize(im.width(), (vector<double>) im.height());
  sumLum.resize(im.width(), (vector<double>) im.height());
  hist.resize(im.width(), vector< vector<int> > (im.height(), vector<int>(36, 0)));

	for(int i = 0; i < (int) im.width(); i++){
    for(int j = 0; j < (int) im.height(); j++){
			HSLAPixel * pixel = im.getPixel(i, j);

			int bucket = pixel->h / 10;
			double hueX = cos(pixel->h * PI/180);
			double hueY = sin(pixel->h * PI/180);
			double sat = pixel->s;
			double lum = pixel->l;

			// left edge
			if (i == 0 && j > 0){
				sat += sumSat[i][j-1];
				lum += sumLum[i][j-1];
				hueX += sumHueX[i][j-1];
				hueY += sumHueY[i][j-1];
				for(int k = 0; k < 36; k++){
          hist[i][j][k] = hist[i][j-1][k];
        }
			}
			// top edge
			if (j == 0 && i > 0){
				sat += sumSat[i-1][j];
				lum += sumLum[i-1][j];
				hueX += sumHueX[i-1][j];
				hueY += sumHueY[i-1][j];
				for (int k = 0; k < 36; k++){
					hist[i][j][k] = hist[i-1][j][k];
				}
			}
			// general case
			if(j > 0 && i > 0){
				sat += sumSat[i-1][j] + sumSat[i][j-1] - sumSat[i-1][j-1];
				lum += sumLum[i-1][j] + sumLum[i][j-1] - sumLum[i-1][j-1];
				hueX += sumHueX[i-1][j] + sumHueX[i][j-1] - sumHueX[i-1][j-1];
				hueY += sumHueY[i-1][j] + sumHueY[i][j-1] - sumHueY[i-1][j-1];
				for (int k = 0; k < 36; k++){
					hist[i][j][k] = hist[i-1][j][k] + hist[i][j-1][k] - hist[i-1][j-1][k];
				}
			}

			hist[i][j][bucket]++;
			sumSat[i][j] = sat;
			sumHueX[i][j] = hueX;
			sumHueY[i][j] = hueY;
			sumLum[i][j] = lum;
		}
	}
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */
  return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	HSLAPixel* ret = new HSLAPixel;

	double alpha = 1.0;
  double totalSat = sumSat[lr.first][lr.second];
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


	double hue = (atan2(hueY, hueX) * 180/PI);
	if (hue < 0) {
    hue += 360;
  }
	ret->h = hue;
	ret->s = totalSat / area;
	ret->l = totalLum / area;
	ret->a = alpha;

	return *ret;

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
