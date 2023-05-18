#include "modelerapp.h"
#include "modelerview.h"
#include "modelerui.h"


#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Box.H>
#include"util.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <thread>


// CLASS ModelerControl METHODS

ModelerControl::ModelerControl() : m_minimum(0.0f), m_maximum(1.0f), m_stepsize(0.1f), m_value(0.0f)
{
}

ModelerControl::ModelerControl(const char* name, float minimum, float maximum, float stepsize, float value)
{
	SetVals(name, minimum, maximum, stepsize, value);
}

ModelerControl::ModelerControl(const ModelerControl &o)
{ 
	operator=(o); 
}

ModelerControl&	ModelerControl::operator=(const ModelerControl &o)
{ 
	if (this != &o) 
		SetVals(o.m_name, o.m_minimum, o.m_maximum, o.m_stepsize, o.m_value); 
	return *this;
}

void ModelerControl::SetVals(const char* name, float minimum, float maximum, float stepsize, float value)
{
	strncpy(m_name,name, 128);
	m_minimum  = minimum;
	m_maximum  = maximum;
	m_stepsize = stepsize;
	m_value    = value;
}


// ****************************************************************************


// Set the singleton initially to a NULL instance
ModelerApplication* ModelerApplication::m_instance = NULL;

// CLASS ModelerApplication METHODS

ModelerApplication* ModelerApplication::Instance()
{
	// Make a new instance if none exists, otherwise, return
	// the existing instance of the ModelerApplication
	return (m_instance) ? (m_instance) : (m_instance = new ModelerApplication());
}

void ModelerApplication::Init(ModelerViewCreator_f createView, 
                              const ModelerControl controls[], unsigned numControls)
{
    int i;

	m_reset = false;
	m_startMCMC   = false;
	
	m_MCMC = new MCMC();
	outputString = new char[200];
	strcpy(outputString, "Sample Output Text");

	

	m_numControls = numControls;

    // ********************************************************
    // Create the FLTK user interface
    // ********************************************************
    
    m_ui = new ModelerUserInterface();
    
    // Store pointers to the controls for manipulation
    m_controlLabelBoxes   = new Fl_Box*[numControls];
    m_controlValueSliders = new Fl_Value_Slider*[numControls];
    
    // Constants for user interface setup
    const int textHeight    = 20;
    const int sliderHeight  = 20;
    const int packWidth     = m_ui->m_controlsPack->w();

	m_ui->m_controlsPack->begin();
    // For each control, add appropriate objects to the user interface
    for (i=0; i<m_numControls; i++)
    {
        // Add the entry to the selection box
        m_ui->m_controlsBrowser->add(controls[i].m_name);

        // Add the label (but make it invisible for now)
        Fl_Box *box = new Fl_Box(0, 0, packWidth, textHeight, controls[i].m_name);
		box->labelsize(10);
		box->hide();
		box->box(FL_FLAT_BOX); // otherwise, Fl_Scroll messes up (ehsu)
        m_controlLabelBoxes[i] = box;

        // Add the slider (but make it invisible for now)
        Fl_Value_Slider *slider = new Fl_Value_Slider(0, 0, packWidth, sliderHeight,0);
        slider->type(1);
        slider->range(controls[i].m_minimum, controls[i].m_maximum);
        slider->step(controls[i].m_stepsize);
        slider->value(controls[i].m_value);
        slider->hide(); 
        m_controlValueSliders[i] = slider;
        slider->callback((Fl_Callback*)ModelerApplication::SliderCallback);
    }
    m_ui->m_controlsPack->end();


	

	// Make sure that we remove the view from the
	// Fl_Group, otherwise, it'll blow up 
	// THIS BUG FIXED 04-18-01 ehsu
	m_ui->m_modelerWindow->remove(*(m_ui->m_modelerView));
	delete m_ui->m_modelerView;

	m_ui->m_modelerWindow->begin();
	m_ui->m_modelerView = createView(0, 0, m_ui->m_modelerWindow->w(), m_ui->m_modelerWindow->h() ,NULL);
	Fl_Group::current()->resizable(m_ui->m_modelerView);
	m_ui->m_modelerWindow->end();
}

ModelerApplication::~ModelerApplication()
{
    // FLTK handles widget deletion
    delete m_ui;
    delete [] m_controlLabelBoxes;
    delete [] m_controlValueSliders;
	m_MCMC ->~MCMC();
	delete outputString;
}

int ModelerApplication::Run()
{
	if (m_numControls == -1)
	{
		fprintf(stderr, "ERROR: ModelerApplication must be initialized before Run()!\n");
		return -1;
	}

    // Just tell FLTK to go for it.
   	Fl::visual( FL_RGB | FL_DOUBLE );
	m_ui->show();

	Fl::add_timeout(0, ModelerApplication::RedrawLoop, NULL);


	return Fl::run();
}

double ModelerApplication::GetControlValue(int controlNumber)
{
    return m_controlValueSliders[controlNumber]->value();
}

void ModelerApplication::SetControlValue(int controlNumber, double value)
{
    m_controlValueSliders[controlNumber]->value(value);
}

void ModelerApplication::ShowControl(int controlNumber)
{
    m_controlLabelBoxes[controlNumber]->show();
	if (controlNumber != MAXLOOPCOUNTER) { //
		m_controlValueSliders[controlNumber]->precision(2); //set number precision to two decimal places
	}
	else {
		m_controlValueSliders[controlNumber]->precision(0); //no decimal place shown in max loop counter slider
	}
	
    m_controlValueSliders[controlNumber]->show();
    m_ui->m_controlsWindow->redraw();
}

void ModelerApplication::HideControl(int controlNumber)
{
    m_controlLabelBoxes[controlNumber]->hide();
    m_controlValueSliders[controlNumber]->hide();
    m_ui->m_controlsWindow->redraw();
}

void ModelerApplication::SliderCallback(Fl_Slider *, void *)
{
    ModelerApplication::Instance()->m_ui->m_modelerView->redraw();
}





void ModelerApplication::RedrawLoop(void*)
{

	// 1/50 second update is good enough
	Fl::add_timeout(0.025, ModelerApplication::RedrawLoop, NULL);
}

//respond to  optimize button pressed
void ModelerApplication::MCMC_Start() {

	int iters=50;
	int n = m_MCMC->loop;

	if (m_updateMCMC!=1) {

		std::remove("./test.csv");

		m_MCMC->optimize(n,false);

		Item a0 = fnts[0];
		Item a1 = fnts[1];
		Item a2 = fnts[2];
		Item a3 = fnts[3];
		ModelerApplication::Instance()->SetControlValue(COUCH_XPOS, a0.loc[0]);
		ModelerApplication::Instance()->SetControlValue(COUCH_ZPOS, a0.loc[1]);
		ModelerApplication::Instance()->SetControlValue(COUCH_ROTATION, a0.loc[2]);

		ModelerApplication::Instance()->SetControlValue(TV_XPOS, a1.loc[0]);
		ModelerApplication::Instance()->SetControlValue(TV_ZPOS, a1.loc[1]);
		ModelerApplication::Instance()->SetControlValue(TV_ROTATION, a1.loc[2]);

		ModelerApplication::Instance()->SetControlValue(TB_XPOS, a2.loc[0]);
		ModelerApplication::Instance()->SetControlValue(TB_ZPOS, a2.loc[1]);
		ModelerApplication::Instance()->SetControlValue(TB_ROTATION, a2.loc[2]);

		ModelerApplication::Instance()->SetControlValue(PL_XPOS, a3.loc[0]);
		ModelerApplication::Instance()->SetControlValue(PL_ZPOS, a3.loc[1]);
		ModelerApplication::Instance()->SetControlValue(PL_ROTATION, a3.loc[2]);

		float s1 = round(m_MCMC->best_total_cost * 10000) / 10000.0;
		float s2 = round(m_MCMC->cur_total_cost * 10000) / 10000.0;
		float s3 = round(m_MCMC->cur_distance_cost * 10000) / 10000.0;
		float s4 = round(m_MCMC->cur_rotation_cost * 10000) / 10000.0;

		outputString = new char[200];
		char str[200];
		sprintf(str, "Cur Iteration %d  Best Total cost %f   Cur Total cost %f  Cur Distance cost %f  Cur Rotation cost %f",
			n, s1, s2, s3, s4);

		strcpy(outputString, str);

	}
	else {

		std::remove("./test.csv");
		DisableProcessWindowsGhosting();
		while (m_updateMCMC && iters <= n) {

			m_MCMC->optimize(iters, true);
			ModelerApplication::Instance()->m_ui->m_modelerView->redraw();
			m_ui->show();
			

			Item a0 = fnts[0];
			Item a1 = fnts[1];
			Item a2 = fnts[2];
			Item a3 = fnts[3];
			ModelerApplication::Instance()->SetControlValue(COUCH_XPOS, a0.loc[0]);
			ModelerApplication::Instance()->SetControlValue(COUCH_ZPOS, a0.loc[1]);
			ModelerApplication::Instance()->SetControlValue(COUCH_ROTATION, a0.loc[2]);

			ModelerApplication::Instance()->SetControlValue(TV_XPOS, a1.loc[0]);
			ModelerApplication::Instance()->SetControlValue(TV_ZPOS, a1.loc[1]);
			ModelerApplication::Instance()->SetControlValue(TV_ROTATION, a1.loc[2]);

			ModelerApplication::Instance()->SetControlValue(TB_XPOS, a2.loc[0]);
			ModelerApplication::Instance()->SetControlValue(TB_ZPOS, a2.loc[1]);
			ModelerApplication::Instance()->SetControlValue(TB_ROTATION, a2.loc[2]);

			ModelerApplication::Instance()->SetControlValue(PL_XPOS, a3.loc[0]);
			ModelerApplication::Instance()->SetControlValue(PL_ZPOS, a3.loc[1]);
			ModelerApplication::Instance()->SetControlValue(PL_ROTATION, a3.loc[2]);

			int s0 = iters;
			float s1 = round(m_MCMC->best_total_cost * 10000) / 10000.0;
			float s2 = round(m_MCMC->cur_total_cost * 10000) / 10000.0;
			float s3 = round(m_MCMC->cur_distance_cost * 10000) / 10000.0;
			float s4 = round(m_MCMC->cur_rotation_cost * 10000) / 10000.0;

			outputString = new char[200];
			char str[200];
			sprintf(str, "Cur Iteration %d  Best Total cost %f   Cur Total cost %f  Cur Distance cost %f  Cur Rotation cost %f",
				s0, s1, s2, s3, s4);

			strcpy(outputString, str);

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			iters = iters + 50;
		}

	}

	ModelerApplication::MCMC_Clean();
}

//Initilize Furnitures' Position
void ModelerApplication::Init_Poistion() {

	init_pos();
	Item a0 = fnts[0];
	Item a1 = fnts[1];
	Item a2 = fnts[2];
	Item a3 = fnts[3];
	ModelerApplication::Instance()->SetControlValue(COUCH_XPOS, a0.loc[0]);
	ModelerApplication::Instance()->SetControlValue(COUCH_ZPOS, a0.loc[1]);
	ModelerApplication::Instance()->SetControlValue(COUCH_ROTATION, a0.loc[2]);

	ModelerApplication::Instance()->SetControlValue(TV_XPOS, a1.loc[0]);
	ModelerApplication::Instance()->SetControlValue(TV_ZPOS, a1.loc[1]);
	ModelerApplication::Instance()->SetControlValue(TV_ROTATION, a1.loc[2]);

	ModelerApplication::Instance()->SetControlValue(TB_XPOS, a2.loc[0]);
	ModelerApplication::Instance()->SetControlValue(TB_ZPOS, a2.loc[1]);
	ModelerApplication::Instance()->SetControlValue(TB_ROTATION, a2.loc[2]);

	ModelerApplication::Instance()->SetControlValue(PL_XPOS, a3.loc[0]);
	ModelerApplication::Instance()->SetControlValue(PL_ZPOS, a3.loc[1]);
	ModelerApplication::Instance()->SetControlValue(PL_ROTATION, a3.loc[2]);

	ModelerApplication::MCMC_Clean();

}

// Update MCMC class instance attributes value to control bar parameter value
void  ModelerApplication::MCMC_Update(double loop,double tempeture, double floor_size,double weight_distance,double weight_angle,double target_t_c,double target_c_t,double tv_coach_target_distance) {
	m_MCMC->loop = loop;
	m_MCMC->floor_size =floor_size;
	m_MCMC->min= 1;
	m_MCMC->max = floor_size - 1;
	m_MCMC->tempeture = tempeture;
	m_MCMC->distance_weight =weight_distance;
	m_MCMC->angle_weight = weight_angle;
	m_MCMC->target_t_c  = target_t_c;
	m_MCMC->target_c_t = target_c_t;
	m_MCMC->tv_coach_target_distance = tv_coach_target_distance;
}


// Clean up MCMC class instance attribute value
void  ModelerApplication::MCMC_Clean() {

	m_MCMC->cur_total_cost = 9999;
	m_MCMC->best_total_cost = 9999;
	m_MCMC->cur_distance_cost = 0;
	m_MCMC->cur_rotation_cost = 0;

}
	






