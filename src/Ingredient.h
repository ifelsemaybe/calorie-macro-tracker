#pragma once
#include <string>
#include <iostream>

#include "Food.h"


using namespace std;



class Ingredient : public Food{

public:

	string brand;

	Ingredient();

	Ingredient (string name, double cal, double protein, double carbs, double fat, string brand, int id, double propValues[]);

	Ingredient(const Ingredient& copy);

	friend ostream& operator << (ostream& out, const Ingredient& ing);

};