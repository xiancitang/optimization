#ifndef MCMC_H
#define MCMC_H

#include "modelerui.h"
#include "vec.h"
#include "mat.h"
#include "Item.h"


enum Moves{DISTANCE_MOVEMENT, ROTATION_MOVEMENT, TOTMOVE};

class MCMC
{

public:
	double min, max;
	double cur_total_cost=0;
	double best_total_cost=9999;
	double cur_distance_cost=0;
	double cur_rotation_cost=0;
	double loop, tempeture, floor_size,distance_weight,angle_weight,target_t_c, target_c_t, tv_coach_target_distance;
	MCMC();
	void bordersT(vector<Item> &f, int index);
	void bordersR(vector<Item> &f, int index);
	void optimize(int n,bool update);
	void random_search(int iters, bool update);
	double distance(Item &a, Item &b);
	double dis_pos(double x1, double z1, double  x2, double z2);
	double Cd( vector<Item> &f, int index_a, int index_b);
	double Ca(vector<Item> &f, int index_a, int index_b);
	void MoveT(vector<Item> &f, int index, double dist);
	void MoveR(vector<Item> &f, int index, double angle);
	double MCMC::make_degree_positive(double degree);
	
	~MCMC();
	

};

extern vector<Item> fnts;



#endif