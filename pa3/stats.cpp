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
				sat += temp->s + sumSat[i][j-1];
				lum += temp->l + sumLum[i][j-1];
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
				hueY[i][j] += sumHueY[i-1][j];
				for (int k = 0; k < 36; k++){
					hist[i][j][k] = hist[i-1][j][k];
				}
			}
			// general case
			if(j > 0 && i > 0){
				sat += sumSat[i-1][j] + sumSat[i][j-1] - sumSat[i-1][j-1];
				lum += sumLum[i-1][j] + sumLum[i][j-1] - sumLum[i-1][j-1];
				hueX[i][j] += sumHueX[i-1][j] + sumHueX[i][j-1] - sumHueX[i-1][j-1]);
				hueY[i][j] += sumHueY[i-1][j] + sumHueY[i][j-1] - sumHueY[i-1][j-1]);
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

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */
  return (lr.first - ul.first + 1) * (lr.second - ul.second + 1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	HSLAPixel* ret = new HSLAPixel;

	double hueX = 0.0;
	double hueY = 0.0;
	double sat = 0.0;
	double lum = 0.0;

	sat = sumSat[lr.first][lr.second];
	lum = sumLum[lr.first][lr.second];
	hueX = sumHueX[lr.first][lr.second];
	hueY = sumHueY[lr.first][lr.second];

	if (ul.first > 0){
		sat -= sumSat[ul.first -1][lr.second];
		lum -= sumLum[ul.first -1][lr.second];
		hueX -= sumHueX[ul.first -1][lr.second];
		hueY -= sumHueY[ul.first -1][lr.second];
	}
	if (ul.second > 0){
		sat -= sumSat[lr.first][ul.second - 1];
		lum -= sumLum[lr.first][ul.second - 1];
		hueX -= sumHueX[lr.first][ul.second - 1];
		hueY -= sumHueY[lr.first][ul.second - 1];
	}
	if (ul.first > 0 && ul.second > 0){
		sat += sumSat[ul.first-1][ul.second-1];
		lum += sumLum[ul.first-1][ul.second-1];
		hueX += sumHueX[ul.first-1][ul.second-1];
		hueY += sumHueY[ul.first-1][ul.second-1];
	}


	double hue = (atan2(hueY, hueX) * 180/PI);
	(hue > 0) ? (ret->h = hue) : (ret->h = (hue + 360));
	ret->s = sat/rectArea(ul,lr);
	ret->l = lum/rectArea(ul,lr);
	ret->a = 1.0;

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
