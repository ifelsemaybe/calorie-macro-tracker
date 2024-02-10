#pragma once

#include <string>

using namespace std;

class Food {

public:

	string name;

	double cal, protein, carbs, fat;

	int id;

	double propValues[4]; //prop stands for proportion, 4th value indicates which of the three indices to use!
	string propTypes[3];

	virtual ~Food() {};

};