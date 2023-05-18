
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glut.h>
#include "modelerglobals.h"
#include "furnitures.cpp"
#include "util.h"
#include "MCMC.h"

vector<Item> fnts;

class MyModel : public ModelerView 
{
public:

    MyModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { 
		init_pos();
	}

    virtual void draw();
};


ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new MyModel(x,y,w,h,label); 
}


void MyModel::draw()
{

	ModelerView::draw();

	ModelerApplication::Instance()->MCMC_Update(GETVAL(MAXLOOPCOUNTER),GETVAL(TEMPERATURE), GETVAL(FLOOR_SIZE),GETVAL(WD), GETVAL(WA), GETVAL(TARGET_COACH_TO_TV),
		GETVAL(TARGET_TV_TO_COACH), GETVAL(TV_COACH_TARGET_DISTANCE));

	// draw the floor
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(0.0f, 0.0f, 0.5f);
	glPushMatrix();
	//move the floor and make its side align to x-axis and z-axis
	glTranslated(GETVAL(FLOOR_SIZE) / 2, 0, GETVAL(FLOOR_SIZE) / 2);
	drawBox(GETVAL(FLOOR_SIZE) , 0.1f, GETVAL(FLOOR_SIZE));
	glTranslated(-GETVAL(FLOOR_SIZE) / 2, 0, -GETVAL(FLOOR_SIZE) / 2);
	glPopMatrix();
		
	// Draw Couch

	char c1[6] = "Couch";
	glPushMatrix();
		setAmbientColor(COLOR_ORANGE);
		setDiffuseColor(COLOR_ORANGE);
		//draw a box in such way to make rotation axis in obj center
		glTranslated(GETVAL(COUCH_XPOS), 0.5f, GETVAL(COUCH_ZPOS));
		glRotated(GETVAL(COUCH_ROTATION), 0.0, 1.0, 0.0);
		drawBox(2.0f, 1, 1.0f);

		//drawVector
		setDiffuseColor(COLOR_RED);
		glPushMatrix();
		drawVector(0.0f, 0, 0.0f, 3);
		glPopMatrix();

		//drawtext
		glPushMatrix();
		drawString(c1);
		glPopMatrix();
	glPopMatrix();


	// Draw TV 
	char c2[6] = "TV";
	glPushMatrix();
		setAmbientColor(COLOR_ORANGE);
		setDiffuseColor(COLOR_ORANGE);
		glTranslated(GETVAL(TV_XPOS), 0.5f, GETVAL(TV_ZPOS));
		glRotated(GETVAL(TV_ROTATION), 0.0, 1.0, 0.0);
		drawBox(2.0f, 1, 1.0f);

		//drawVector
		setDiffuseColor(COLOR_RED);
		glPushMatrix();
		drawVector(0.0f, 0, 0.0f, 3); 
		glPopMatrix();

		//drawtext
		glPushMatrix();
		drawString(c2);
		glPopMatrix();
	glPopMatrix();


	// Draw Table 

	char c3[6] = "Table";
	glPushMatrix();
	setAmbientColor(COLOR_ORANGE);
	setDiffuseColor(COLOR_ORANGE);
	glTranslated(GETVAL(TB_XPOS), 0.5f, GETVAL(TB_ZPOS));
	glRotated(GETVAL(TB_ROTATION), 0.0, 1.0, 0.0);
	drawBox(2.0f, 1, 1.0f);

	//drawVector
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	drawVector(0.0f, 0, 0.0f, 3);
	glPopMatrix();

	//drawtext
	glPushMatrix();
	drawString(c3);
	glPopMatrix();
	glPopMatrix();


	// Draw Plant

	char c4[6] = "Plant";
	glPushMatrix();
	setAmbientColor(COLOR_ORANGE);
	setDiffuseColor(COLOR_ORANGE);
	glTranslated(GETVAL(PL_XPOS), 0.5f, GETVAL(PL_ZPOS));
	glRotated(GETVAL(PL_ROTATION), 0.0, 1.0, 0.0);
	drawBox(2.0f, 1, 1.0f);

	//drawVector
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	drawVector(0.0f, 0, 0.0f, 3);
	glPopMatrix();

	//drawtext
	glPushMatrix();
	drawString(c4);
	glPopMatrix();
	glPopMatrix();

	//debug text
	drawStringOnScreen(ModelerApplication::Instance()->outputString);
}

int main()
{

    ModelerControl controls[NUMCONTROLS];
	
	controls[MAXLOOPCOUNTER] = ModelerControl("Max Loop Counter", 100, 5000, 1.00f, 2000);
	controls[TEMPERATURE] = ModelerControl("Temperature", 0, 1000, 1.00f, 1000);
	controls[FLOOR_SIZE] = ModelerControl("Size Of Floor", 10, 30, 0.1f, 20);
	controls[WD] = ModelerControl("Distance Weight", 0, 1, 0.1f, 0.5);
	controls[WA] = ModelerControl("Angle Weight", 0, 1, 0.1f, 0.5);
	controls[TV_COACH_TARGET_DISTANCE] = ModelerControl("TV_Coach_Target_Distance", 10, 20, 1, 7);
	controls[TARGET_TV_TO_COACH] = ModelerControl("Target_TV_To_Coach", 0, 180 ,1, 0);
	controls[TARGET_COACH_TO_TV] = ModelerControl("Target_Coach_To_TV", 0, 180, 1, 0);
	
	controls[COUCH_XPOS] = ModelerControl("Couch X Position", 0, 30, 0.1f, 2);
	controls[COUCH_ZPOS] = ModelerControl("Couch Z Position", 0, 30, 0.1f, 2);
	controls[COUCH_ROTATION] = ModelerControl("Couch Rotation", 0, 360, 1, 0);

	controls[TV_XPOS] = ModelerControl("TV X Position", 0, 30, 0.1f, 10);
	controls[TV_ZPOS] = ModelerControl("TV Z Position", 0, 30, 0.1f, 4);
	controls[TV_ROTATION] = ModelerControl("TV Rotation", 0, 360, 1, 0);

	controls[TB_XPOS] = ModelerControl("TB X Position", 0, 30, 0.1f, 3);
	controls[TB_ZPOS] = ModelerControl("TB Z Position", 0, 30, 0.1f, 13);
	controls[TB_ROTATION] = ModelerControl("TB Rotation", 0, 360, 1, 0);

	controls[PL_XPOS] = ModelerControl("PL X Position", 0, 30, 0.1f, 18);
	controls[PL_ZPOS] = ModelerControl("PL Z Position", 0, 30, 0.1f, 3);
	controls[PL_ROTATION] = ModelerControl("PL Rotation", 0, 360, 1, 0);



    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}


