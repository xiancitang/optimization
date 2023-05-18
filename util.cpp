#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glut.h>
#include "modelerglobals.h"

#include <chrono>
#include <thread>

using namespace std;

// random double generater between certain range
double dRand(double dMin, double dMax)
{
	double d = (double)rand() / RAND_MAX;
	return dMin + d * (dMax - dMin);
}

// random generate furniture positions
void init_pos() {

	fnts.clear();
	Item* b1 = new Item(dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(0, 360));
	Item* b2 = new Item(dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(0, 360));
	Item* b3 = new Item(dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(0, 360));
	Item* b4 = new Item(dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(1, GETVAL(FLOOR_SIZE)-1), dRand(0, 360));
	fnts.push_back(*b1);
	fnts.push_back(*b2);
	fnts.push_back(*b3);
	fnts.push_back(*b4);
}

