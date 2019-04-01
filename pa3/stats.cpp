#include <typeinfo>
#include <iostream>
#include "stats.h"

stats::stats(PNG & im){

/* your code here! */

	sumSat.resize(im.width());
	sumLum.resize(im.width());
	sumHueX.resize(im.width());
	sumHueY.resize(im.width());
	hist.resize(im.width());
	
	for (int i = 0; i < (int) im.width(); i++){
/*         vector<double> satRow;
		vector<double> lumRow;			
		vector<double> hueXRow;
		vector<double> hueYRow;
		vector<vector<int>> histRow; */
		sumSat[i].resize(im.height());
		sumLum[i].resize(im.height());
		sumHueX[i].resize(im.height());
		sumHueY[i].resize(im.height());
		hist[i].resize(im.height());
		
		for (int j = 0; j < (int) im.height(); j++){
			HSLAPixel *temp = im.getPixel(i, j);
			vector<int> histSoFar(36, 0);
			
			int k = (int) temp->h / 10;
			
			// origin
			if (i == 0 && j == 0){
				sumSat[i][j] = temp->s;
				sumLum[i][j] = temp->l;
				sumHueX[i][j] = cos(temp->h * PI/180);
				sumHueY[i][j] = sin(temp->h * PI/180);
				
			}
			
			// top row
			else if (i == 0 && j > 0){
				sumSat[i][j] = temp->s + sumSat[i][j-1];
				sumLum[i][j] = temp->l + sumLum[i][j-1];
				sumHueX[i][j] = cos(temp->h * PI/180) + sumHueX[i][j-1];
				sumHueY[i][j] = sin(temp->h * PI/180) + sumHueY[i][j-1];
				
				for (int z = 0; z < 36; z++){
					histSoFar[z] = hist[i][j-1][z];							
				}
				
			}
			
			// first column
			else if (j == 0 && i > 0){
				sumSat[i][j] = temp->s + sumSat[i-1][j];
				sumLum[i][j] = temp->l + sumLum[i-1][j];
				sumHueX[i][j] = cos(temp->h * PI/180) + sumHueX[i-1][j];
				sumHueY[i][j] = sin(temp->h * PI/180) + sumHueY[i-1][j];
				
				for (int z = 0; z < 36; z++){
					histSoFar[z] = hist[i-1][j][z];			
				}
				
			}
			
			// everything in the middle
			else{
				sumSat[i][j] = temp->s + sumSat[i-1][j] + sumSat[i][j-1] - sumSat[i-1][j-1];
				sumLum[i][j] = temp->l + sumLum[i-1][j] + sumLum[i][j-1] - sumLum[i-1][j-1];
				sumHueX[i][j] = (
					cos(temp->h * PI/180) + sumHueX[i-1][j] + sumHueX[i][j-1] - sumHueX[i-1][j-1]);
				sumHueY[i][j] = (
					sin(temp->h * PI/180) + sumHueY[i-1][j] + sumHueY[i][j-1] - sumHueY[i-1][j-1]);
			
				for (int z = 0; z < 36; z++){
					histSoFar[z] = hist[i-1][j][z] + hist[i][j-1][z] - hist[i-1][j-1][z];		
				}
			
			}

			histSoFar[k]++;
			hist[i][j] = histSoFar;
        }

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
