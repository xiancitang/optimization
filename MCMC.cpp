#include "MCMC.h"
#include "modelerglobals.h"

#include <vector>
#include "util.h"
# include "count.h"

#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

vector<Item> fnts_best;

 MCMC::MCMC() {
	
}


  // bounding furtunire within room
 void MCMC::bordersT(vector<Item> &f, int index) {

	 if (f[index].loc[0] < min)
	 {
		 f[index].loc[0] = min;
	 }
	 else if (f[index].loc[0] > max)
	 {
		 f[index].loc[0] = max;
	 }

	 if (f[index].loc[1] < min)
	 {
		 f[index].loc[1] = min;
	 }
	 else if (f[index].loc[1] > max)
	 {
		 f[index].loc[1] = max;
	 }

 }

 // Bounding furniture prientation if close to wall
 void MCMC::bordersR(vector<Item> &f, int index) {

	 if (f[index].loc[0] <= min && (180< f[index].loc[2] && f[index].loc[2] <360))
	 {
		 if (f[index].loc[2] > 270) { f[index].loc[2] = 0; }
		 else if (f[index].loc[2] == 270) { f[index].loc[2] = 90; }
		 else { f[index].loc[2] = 180; }
	 }
	 else if (f[index].loc[0] >= max && (0< f[index].loc[2] && f[index].loc[2] < 180))
	 {

		 if (f[index].loc[2] > 90) { f[index].loc[2] = 180; }
		 else if (f[index].loc[2] == 90) { f[index].loc[2] = 270; }
		 else { f[index].loc[2] = 0; }

	 }

	 else if (f[index].loc[1] <= min &&  (90<f[index].loc[2] && f[index].loc[2]<270))
	 {

		 if (f[index].loc[2] > 180) { f[index].loc[2] = 270; }
		 else if (f[index].loc[2] == 180) { f[index].loc[2] = 0; }
		 else { f[index].loc[2] = 90; }
	 }
	 else if (f[index].loc[1] >= max && ( (270 < f[index].loc[2] && f[index].loc[2] <= 360) || (0 <= f[index].loc[2] && f[index].loc[2] < 90)))
	 {

		 if (f[index].loc[2] < 90) { f[index].loc[2] = 90; }
		 else if (f[index].loc[2] == 0) { f[index].loc[2] = 180; }
		 else { f[index].loc[2] = 270; }
	 }

 }

 void  MCMC::optimize(int n,bool update) {

	
	 random_search(n, update);
	 fnts = fnts_best;
 };

 // find out  set of (x,z,r) for furtunitures that meet pairwise constraints
 void MCMC::random_search(int n,  bool update) {

	 double cur_cost, new_cost;
	 
	 if (update) {

		 if (n == 50) {
			 run = 0;
		 }
		 else { run = n - 50; }
	 }
	 else {
		 run = 0;
	 }

	 //Clean up old file if exists; append each iteration cost to csv file
	 ofstream outfile;
	 outfile.open("test.csv", std::ios::app); 

	 // add header to csv file at beginning
	 if (run == 0)
	 {
		 outfile << " run " << "," << " best_total_cost " << "," << " cur_total_cost " << "," << " cur_distance_cost " << ","
			 << "  cur_rotation_cost " << "\n";
	 }

	 for (run; run < n; run++) {
		 
		 if (update) {
			 if (run % 100 == 0)
				 std::this_thread::sleep_for(std::chrono::milliseconds(500));
		 }
		
		 // T cooling schedule
		 int T ;
		 int m = loop/ 1000;
		 if (run <= 400*m) {
			 T = tempeture;
		 }
		 else if (run > 400*m && run <= 800*m) {
			 T = tempeture/10;
		 }
		 else {
			 T = tempeture/100;
		 };

		 vector<Item> fnts_copy = fnts;

		 // current cost total
		 double cd = Cd(fnts, 0, 1);
		 double ca = Ca(fnts, 0, 1);
		 cur_cost = distance_weight * cd + angle_weight * ca;
		 
		 int item_index = rand() % 2;

		 // random do a move , if 0, MoveT; if 1,MoveR
		 int move_pick = rand() % 4;
	 
		 if (move_pick == 0) {
			 MoveT(fnts_copy, item_index, 0.3);
		 }
		 else if (move_pick == 1) {
			 MoveT(fnts_copy, item_index, -0.3);
		 }
		 else if (move_pick == 2) {
			 MoveR(fnts_copy, item_index, 1);
		 }
		 else {
			 MoveR(fnts_copy, item_index, -1);
		 }

		 // new cost total
		 double cdf = Cd(fnts_copy, 0, 1);
		 double caf = Ca(fnts_copy, 0, 1);
		 new_cost = distance_weight * cdf + angle_weight * caf;

		 double delta = new_cost - cur_cost;
		
		 if (delta < 0) {
			 fnts = fnts_copy;
			 cur_total_cost = new_cost;
			 cur_distance_cost = cdf;
			 cur_rotation_cost = caf;
		 }
		 else {

			 double p = dRand(0, 1);
			 double k = 0.01;
			 // Caculate move probability
			 double m = exp(-delta/(k*T));
			 double p_a = min(m, 1);

			 if (p <= p_a) {
				 fnts = fnts_copy;
				 cur_total_cost = new_cost;
				 cur_distance_cost = cdf;
				 cur_rotation_cost = caf;
			 }
			 else {
				 cur_total_cost = cur_cost;
				 cur_distance_cost = cd;
				 cur_rotation_cost = ca;
			 }

		 }


		 if (best_total_cost > cur_total_cost) {
			 best_total_cost = cur_total_cost;
			 fnts_best = fnts;
		 }
		 cout << "current cost: " << cur_total_cost<< " \n";
		 cout << "current distance cost: " << cur_distance_cost << " \n";
		 cout << "current rotation cost: " << cur_rotation_cost << " \n";

		 outfile << run << "," << best_total_cost << "," << cur_total_cost << ","
			 << cur_distance_cost << "," << cur_rotation_cost<< "\n";

		 fnts_copy.clear();
	 }  
	 
	 outfile.close();

 };



 //	Calculate Distance between two vectors
 double MCMC::distance(Item &a, Item &b) {

	 Vec3<double> ap = a.loc;
	 Vec3<double> bp = b.loc;

	 double d = (ap[0] - bp[0]) * (ap[0] - bp[0]) + (ap[1] - bp[1]) * (ap[1] - bp[1]);
	
	 return sqrt(d);
 };

 //	Calculate Distance between two vectors
 double MCMC::dis_pos(double x1, double z1, double  x2, double z2) {

	 double d = (x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2);
	 return sqrt(d);
 };

 // Pairwise distance cost function
 double MCMC::Cd(vector<Item> &f, int index_c, int index_t) {

	 double diff = distance(f[index_c], f[index_t]);
	 
	 return   abs(tv_coach_target_distance - diff);
 }

 // Pairwise angle cost function
 double MCMC::Ca(vector<Item> &f, int index_c, int index_t) {

	 // Convert to (0,360) range if degree is negative
	 f[index_c].loc[2] = make_degree_positive(f[index_c].loc[2]);
	 f[index_t].loc[2] = make_degree_positive(f[index_t].loc[2]);

	 double d = abs(target_c_t - target_t_c);

	 // degree needed if rotate from coach to TV opposite
	 double CT = make_degree_positive(f[index_t].loc[2] - 180);
	 double d1 = min(abs(CT - f[index_c].loc[2]), 360- abs(CT - f[index_c].loc[2]));

	 // degree needed if rotate from  TV to Coach opposite
	 double TC = make_degree_positive(f[index_c].loc[2] - 180);
	 double d2 = min(abs(TC - f[index_t].loc[2]), 360 - abs(TC - f[index_t].loc[2]));


	 // Supposed_degree vs current_degree; ideally 0 , means current funiture is same as supposed degree
	 // This one guarantee angle correctness
	 double degree_gap = abs(d1-d)+abs(d2-d);


	 // Check whether Coach is on the side of TV fronter vector direction so Coach and TV face each other
	 double t_x_move = tv_coach_target_distance * sin(f[index_t].loc[2] * M_PI / 180);
	 double t_z_move = tv_coach_target_distance * cos(f[index_t].loc[2] * M_PI / 180);

	 double target_t_x = f[index_t].loc[0] + t_x_move;
     double target_t_z = f[index_t].loc[1] + t_z_move;

	 // n, normal of TV  
	 double n_x = -t_z_move ;
	 double n_z = t_x_move;


	 // cross product of (tv to coach vector) and normal of TV(n_x,0, n_z)
	 double m1 = f[index_c].loc[0]- f[index_t].loc[0]; double m2 = 0; double m3 = f[index_c].loc[1]- f[index_t].loc[1];
	 double n1 = n_x; double n2 = 0; double n3 = n_z;
	 double cp_x = m2 * n3 - m3 * n2;
	 double cp_y = m1 * n3 - m3 * n1;
	 double cp_z = m1 * n1 - m2 * n1;


	 // cross product of (tv fronter vector) and normal of TV (n_x,0, n_z)
	 double am1 = t_x_move; double am2 = 0; double am3 = t_z_move;
	 double an1 = n_x; double an2 = 0; double an3 = n_z;
	 double acp_x = am2 * an3 - am3 * an2;
	 double acp_y = am1 * an3 - am3 * an1;
	 double acp_z = am1 * an1 - am2 * an1;

	 // If one y is positive, the other y is negative, it means  Coach are not on the side of TV fronter vector direction
	 // This one guarantee face each other 
	 double face_c = 0;
	 if ((acp_y*cp_y) <0) {
		 face_c = 180;
	 }

	 double dot_product = t_x_move * m1 + t_z_move * m3;
	 double dif = distance(f[index_c], f[index_t]);

	 // Degree between TV->coach vector and TV to supposed_coach vector
	 // This guarantee furniture current position close to supposed point
	 double angle = acos(dot_product/(tv_coach_target_distance*dif))*180/M_PI;


	 return (degree_gap + face_c + angle*2);
 }



 void MCMC::MoveT(vector<Item> &f, int index, double dist) {

	 double x_move = dist * sin(f[index].loc[2]*M_PI/180);
	 double z_move = dist * cos(f[index].loc[2]*M_PI/180);
	 f[index].loc[0] = f[index].loc[0] + x_move;
	 f[index].loc[1] = f[index].loc[1] + z_move;
	 bordersT(f, index);
 }



 void MCMC::MoveR(vector<Item> &f, int index, double angle) {

	 f[index].loc[2]= f[index].loc[2] - angle;
	 f[index].loc[2]= make_degree_positive(f[index].loc[2]);
	 bordersR(f, index);
 }

 double MCMC::make_degree_positive(double degree) {
	 if (degree < 0) {
		 degree = degree + 360;
	 }
	 return fmod(degree, 360);
 }


 MCMC::~MCMC(){
	 cout << "\n Destructor executed";
 }
 
 