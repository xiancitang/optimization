#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{  //0-2; 3-5;6-8
	MAXLOOPCOUNTER, TEMPERATURE, FLOOR_SIZE, WD, WA, TV_COACH_TARGET_DISTANCE,TARGET_TV_TO_COACH,TARGET_COACH_TO_TV,
	COUCH_XPOS, COUCH_ZPOS, COUCH_ROTATION, 
	TV_XPOS, TV_ZPOS, TV_ROTATION,
	TB_XPOS, TB_ZPOS, TB_ROTATION,
	PL_XPOS, PL_ZPOS, PL_ROTATION,
	
	
	

	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_LIGHTBLUE 0.2f,0.87f,0.88f
#define COLOR_LIGHTGREEN 0.24f,1.0f,0.48f
#define COLOR_ORANGE 0.95f, 0.46f, 0.09f
#define COLOR_LIGHTGREY 0.686f, 0.811f, 0.933f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define GETVAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif