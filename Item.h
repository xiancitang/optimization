#pragma once
#include <vector>
#include "vec.h"


class Item{

public:

	Vec3<double> loc;

	Item(double a, double b, double c) {
		loc=Vec3<double>(a, b, c);
	}
};