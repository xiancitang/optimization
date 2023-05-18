#pragma once
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glut.h>
#include "modelerglobals.h"
#include "furnitures.cpp"
#include "util.h"
#include "MCMC.h"
#include "common.h"
class MyModel 
{
public:
	MyModel(int x, int y, int w, int h, char *label);

	void MyModel::draw();

};

ModelerView* createSampleModel(int x, int y, int w, int h, char *label);

