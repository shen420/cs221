
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){

/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
}

int toqutree::size() {
/* your code here */
	return sizeHelper(root);
}

int toqutree::sizeHelper(Node * root){
	if(root == NULL) return 0;
	return 1
					+ sizeHelper(root->NW)
					+ sizeHelper(root->NE)
					+ sizeHelper(root->SE)
					+ sizeHelper(root->SW);
}


toqutree::Node * toqutree::buildTree(PNG * im, int k) {

/* your code here */

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.
	Node * res;
	unsigned int width = (unsigned int) pow(2, k);
	if(k==0){
		pair<int, int> ctr (0,0);
		HSLAPixel * other = im.getPixel(0,0);
		HSLAPixel * pixel = new HSLAPixel(other->h, other->s, other->l, other->a);
		res = new Node(ctr, k, * pixel);
	}else{
		pair<int, int> ctr = determineCenter(im, width);
		stats * stats = new stats(* im);
		HSLAPixel a = stats->getAvg(pair<int, int> (0,0), pair<int,int> ((int)width - 1, (int)width - 1));
		delete stats;
		res = new Node(ctr, k, a);

		PNG * se = new PNG(width / 2, width / 2);
		for(unsigned int i = 0; i < width / 2; i++){
		  for(unsigned int j = 0; j < width / 2; j++){
		    unsigned int x = (res->center.first + i) % width;
		    unsigned int y = (res->center.second + j) % width;
		    HSLAPixel * pixel = se->getPixel(i, j);
		    HSLAPixel * other = im->getPixel(x, y);
		    copyPixel(pixel, other);
		  }
		}
		res->SE = buildTree(se, k-1);

		PNG * sw = new PNG(width / 2, width / 2);
		for(unsigned int i = 0; i < width / 2; i++){
		  for(unsigned int j = 0; j < width / 2; j++){
		    unsigned int x = (res->center.first + i + width / 2) % width;
		    unsigned int y = (res->center.second + j) % width;
		    HSLAPixel * pixel = sw->getPixel(i, j);
		    HSLAPixel * other = im->getPixel(x, y);
		    copyPixel(pixel, other);
		  }
		}
		res->SW = buildTree(sw, k-1);

		PNG * ne = new PNG(width / 2, width / 2);
		for(unsigned int i = 0; i < width / 2; i++){
		  for(unsigned int j = 0; j < width / 2; j++){
		    unsigned int x = (res->center.first + i) % width;
		    unsigned int y = (res->center.second + j + width / 2) % width;
		    HSLAPixel * pixel = ne->getPixel(i, j);
		    HSLAPixel * other = im->getPixel(x, y);
		    copyPixel(pixel, other);
		  }
		}
		res->NE = buildTree(ne, k-1);

		PNG * nw = new PNG(width / 2, width / 2);
		for(unsigned int i = 0; i < width / 2; i++){
		  for(unsigned int j = 0; j < width / 2; j++){
		    unsigned int x = (res->center.first + i + width / 2) % width;
		    unsigned int y = (res->center.second + j + width / 2) % width;
		    HSLAPixel * pixel = ne->getPixel(i, j);
		    HSLAPixel * other = im->getPixel(x, y);
		    copyPixel(pixel, other);
		  }
		}
		res->NW = buildTree(nw, k-1);
	}
	delete im;
	return res;
}

pair<int, int> toqutree::determineCenter(PNG * im, unsigned int width){
	pair<int, int> res;
	double min = INT_MAX;
	for(unsigned int a = 0; a < width / 2; a++){
		for(unsigned int b = 0; b < width / 2; b++){
			auto c = a + width / 4;
			auto d = b + width / 4;

			double totalEntropy = 0;
			pair<int, int> ul (0, 0);
			pair<int, int> lr (width / 2 - 1, width / 2 - 1);

			PNG * se = new PNG(width / 2, width / 2);
			for(unsigned int i = 0; i < width / 2; i++){
			  for(unsigned int j = 0; j < width / 2; j++){
			    unsigned int x = (c + i) % width;
			    unsigned int y = (d + j) % width;
			    HSLAPixel * pixel = se->getPixel(i, j);
			    HSLAPixel * other = im->getPixel(x, y);
			    copyPixel(pixel, other);
			  }
			}
			stats * se_stats = new stats(* se);
			totalEntropy += se_stats->entropy(ul, lr);
			delete se_stats;

			PNG * sw = new PNG(width / 2, width / 2);
			for(unsigned int i = 0; i < width / 2; i++){
			  for(unsigned int j = 0; j < width / 2; j++){
			    unsigned int x = (c + i + width / 2) % width;
			    unsigned int y = (d + j) % width;
			    HSLAPixel * pixel = sw->getPixel(i, j);
			    HSLAPixel * other = im->getPixel(x, y);
			    copyPixel(pixel, other);
			  }
			}
			stats * sw_stats = new stats(* sw);
			totalEntropy += sw_stats->entropy(ul, lr);
			delete sw_stats;

			PNG * ne = new PNG(width / 2, width / 2);
			for(unsigned int i = 0; i < width / 2; i++){
			  for(unsigned int j = 0; j < width / 2; j++){
			    unsigned int x = (c + i) % width;
			    unsigned int y = (d + j + width / 2) % width;
			    HSLAPixel * pixel = ne->getPixel(i, j);
			    HSLAPixel * other = im->getPixel(x, y);
			    copyPixel(pixel, other);
			  }
			}
			stats * ne_stats = new stats(* ne);
			totalEntropy += ne_stats->entropy(ul, lr);
			delete ne_stats;

			PNG * nw = new PNG(width / 2, width / 2);
			for(unsigned int i = 0; i < width / 2; i++){
			  for(unsigned int j = 0; j < width / 2; j++){
			    unsigned int x = (c + i + width / 2) % width;
			    unsigned int y = (d + j + width / 2) % width;
			    HSLAPixel * pixel = ne->getPixel(i, j);
			    HSLAPixel * other = im->getPixel(x, y);
			    copyPixel(pixel, other);
			  }
			}
			stats * nw_stats = new stats(* nw);
			totalEntropy += nw_stats->entropy(ul, lr);
			delete nw_stats;

			if(totalEntropy < min){
				min = totalEntropy;
				res.first = a;
				res.second = b;
			}
		}
	}
	return res;
}

PNG toqutree::render(){

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the
// quadtree, instead.

/* your code here */
	return renderHelper(root);

}

PNG toqutree::renderHelper(const Node * root){
	auto width = (unsigned int) pow(2, root->dimension);
	PNG retVal = PNG(width, width);
	if(root->NW == NULL){
		for(unsigned int i = 0; i < width; i++){
			for(unsigned int j = 0; j < width; j++){
				HSLAPixel * pixel = retVal.getPixel(i, j);
				copyPixel(pixel, & root->avg);
			}
		}
	} else{
		// fill se
		PNG se = renderHelper(root->SE);
		for(unsigned int i = 0; i < width / 2; i++){
			for(unsigned int j = 0; j < width / 2; j++){
				unsigned int x = (root->center.first + i) % width;
				unsigned int y = (root->center.second + j) % width;
				HSLAPixel * pixel = retVal.getPixel(x, y);
				HSLAPixel * other = se.getPixel(i, j);
				copyPixel(pixel, other);
			}
		}

		// fill sw
		PNG sw = renderHelper(root->SW);
		for(unsigned int i = 0; i < width / 2; i++){
			for(unsigned int j = 0; j < width / 2; j++){
				unsigned int x = (root->center.first + i + width / 2) % width;
				unsigned int y = (root->center.second + j) % width;
				HSLAPixel * pixel = retVal.getPixel(x, y);
				HSLAPixel * other = sw.getPixel(i, j);
				copyPixel(pixel, other);
			}
		}

		// fill ne
		PNG ne = renderHelper(root->NE);
		for(unsigned int i = 0; i < width / 2; i++){
			for(unsigned int j = 0; j < width / 2; j++){
				unsigned int x = (root->center.first + i) % width;
				unsigned int y = (root->center.second + j + width / 2) % width;
				HSLAPixel * pixel = retVal.getPixel(x, y);
				HSLAPixel * other = ne.getPixel(i, j);
				copyPixel(pixel, other);
			}
		}

		PNG nw = renderHelper(root->NW);
		for(unsigned int i = 0; i < width / 2; i++){
			for(unsigned int j = 0; j < width / 2; j++){
				unsigned int x = (root->center.first + i + width / 2) % width;
				unsigned int y = (root->center.second + j + width / 2) % width;
				HSLAPixel * pixel = retVal.getPixel(x, y);
				HSLAPixel * other = nw.getPixel(i, j);
				copyPixel(pixel, other);
			}
		}
	}
	return retVal;
}

void toqutree::copyPixel(HSLAPixel * pixel, const HSLAPixel * other){
	pixel->l = other->l;
	pixel->a = other->a;
	pixel->h = other->h;
	pixel->s = other->s;
}


/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

	prune(root,tol);

}

void toqutree::pruneHelper(Node * & curr, double tol){
	if(curr == NULL || curr->NW == NULL) return;
	if(shouldPrune(tol, curr->NW, curr->avg.h)
		&& shouldPrune(tol, curr->NE, curr->avg.h)
		&& shouldPrune(tol, curr->SE, curr->avg.h)
		&& shouldPrune(tol, curr->SW, curr->avg.h)){
		clear(curr->NW);
		clear(curr->NE);
		clear(curr->SE);
		clear(curr->SW);
		return;
	}
	pruneHelper(curr->NW, tol);
	pruneHelper(curr->NE, tol);
	pruneHelper(curr->SE, tol);
	pruneHelper(curr->SW, tol);
}

boolean toqutree::shouldPrune(double tol, Node * & curr, double h){
	if(curr->NW == NULL){
		return abs(curr->avg.h - h) <= tol;
	}
	return shouldPrune(tol, curr->NW, h)
	 				&& shouldPrune(tol, curr->NE, h)
					&& shouldPrune(tol, curr->SE, h)
					&& shouldPrune(tol, curr->SW, h);
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
/* your code here */
	clearHelper(curr);
}

void toqutree::clearHelper(Node * & curr){
	if(curr == NULL) return;
	clearHelper(curr->NW);
	clearHelper(curr->NE);
	clearHelper(curr->SE);
	clearHelper(curr->SW);
	delete curr;
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {

/* your code here */
	return copyHelper(other);
}

toqutree::Node * toqutree::copyHelper(const Node * other){
	if(other == NULL) return NULL;
	Node * res = new Node(other->center, other->dimension, other->avg);
	res->NW = copyHelper(other->NW);
	res->NE = copyHelper(other->NE);
	res->SE = copyHelper(other->SE);
	res->SW = copyHelper(other->SW);
	return res;
}
