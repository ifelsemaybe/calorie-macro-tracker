#pragma once
#include <string>
#include <iostream>

#include "Utility.h";

using namespace std;


class Ingredient {

public:

	double cal;

	double carbs, protein, fat;

	string name, proportion;

	Ingredient();

	Ingredient(string name, double cal, double protein, double carbs, double fat, string proportion);

	Ingredient(const Ingredient& copy);

	friend ostream& operator << (ostream& out, const Ingredient& ing);

};