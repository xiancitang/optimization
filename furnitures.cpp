//#include <vector>
//#include "Item.h"
//#include "util.h"
//
//
//using namespace std;
//
//class furnitures {
//
//public:
//
//	 vector<Item> fnts;
//
//	 void addItem(Item b) {
//		 fnts.push_back(b);
//	 }
//
//	 void optimize(int iters) {
//
//		 // search in space for solution 
//		 random_search(iters);
//
//	 };
//
//	 // find out  set of (x,z,r) for furtunitures that meet pairwise constraints
//	 void random_search(int iters) {
//
//		 double cur_cost, new_cost;
//
//		 vector<Item> fnts_copy = fnts;
//		 
//
//		 for (int i= 0; i < iters;i++) {
//
//			 // exponential cooling schedule
//			 int T ;
//			 if (i <= 400) {
//				 T = 1000;
//			 }
//			 else if (i > 400 && i <= 800) {
//				 T = 100;
//			 }
//			 else {
//				 T = 10;
//			 }
//
//			 cur_cost = C(0.5,0.5,2,5,5);
//
//			 // random pick a item ; in vector, index range from 0 to 3 , total 4 items 
//			 int item_index = iRand(0,3);
//			 cout << "iter: " << i << " \n";
//			 cout << "pick item index: " << item_index << " \n";
//			 cout << "pick item loc: " << fnts_copy[item_index].loc << " \n";
//			 // random do a move , if 0, MoveT; if 1,MoveR
//			 int move_pick = iRand(0, 1);
//			 if (move_pick == 0) {
//				 MoveT(fnts_copy[item_index],1);
//			 }
//			 else {
//				 MoveR(fnts_copy[item_index],5);
//			 }
//
//			 new_cost = C(0.5, 0.5, 2, 5, 5);
//			 double delta = new_cost - cur_cost;
//			 if (delta< 0) {
//				 fnts = fnts_copy;
//			 }
//			 else if(dRand(0, 1) < exp(-delta / T)) {
//
//				 fnts = fnts_copy;
//			 }
//
//		 }
//
//	 };
//
//
//	 //	Calculate Distance between two vectors
//	 double distance(Item a, Item b) {
//
//		 Vec3<double> ap = a.loc;
//		 Vec3<double> bp = b.loc;
//	
//		double d= (ap[0] - bp[0]) * (ap[0] - bp[0]) + (ap[1] - bp[1]) * (ap[1] - bp[1]);
//	
//		double res = sqrt(d);
//		 return res;
//	 }
//
//	// pairwise distance cost function
//	double Cd(double target, Item a, Item b) {
//		double diff = distance(a, b);
//		double gap = abs(target -distance(a, b));
//		return gap;
//	}
//
//	// pairwise angle cost function
//	double Ca(double target1, double target2,Item a, Item b) {
//		
//		// rotate from b to a
//		double gap_b_a=0, gap_b_a_f=0;
//		double ba1 = a.loc[2] - 180;
//		double ba2 = a.loc[2] + 180;
//		if (ba1 < b.loc[2]) {
//			gap_b_a = b.loc[2] - ba1;
//			gap_b_a_f=abs(target1 - gap_b_a);
//		}
//		else if (ba2 < b.loc[2]) {
//			gap_b_a = b.loc[2] - ba2;
//			gap_b_a_f = abs(target1 - gap_b_a);
//		}
//
//		// from a to b
//		double gap_a_b=0, gap_a_b_f=0;
//		double ab1 = b.loc[2] - 180;
//		double ab2 = b.loc[2] + 180;
//		if (ab1 < a.loc[2]) {
//			gap_a_b = a.loc[2] - ab1;
//			gap_a_b_f = abs(target1 - gap_a_b);
//		}
//		else if (ab2 < a.loc[2]) {
//			gap_a_b = a.loc[2] - ab2;
//			gap_a_b_f = abs(target2 - gap_a_b);
//		}
//
//		return (gap_a_b_f+ gap_b_a_f);
//	}
//
//	double C(double d_w, double a_w, double target_d, double target_a1,double target_a2) {
//
//		double cd=0.0;
//		double ca=0.0;
//		for (auto& b1 : fnts) {
//			for (auto& b2 : fnts) {
//				if (b1.loc[0] != b2.loc[0] && b1.loc[1] != b2.loc[1]) {
//					cd = cd + Cd(target_d,b1, b2);
//					ca = ca + Ca(target_a1, target_a2, b1, b2);
//				}
//			
//			}
//		}
//
//		return (d_w*cd + a_w * ca);
//
//	}
//
//	void MoveT(Item a, double dist) {
//
//		double x_move = dist * sin(a.loc[2]);
//		double z_move = dist * cos(a.loc[2]);
//		a.loc[0] = a.loc[0] + x_move;
//		a.loc[1] = a.loc[1] + z_move;
//	}
//
//	void MoveR(Item a, double angle) {
//		a.loc[2] = a.loc[2] - angle;
//	}
//
//};