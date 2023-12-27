#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <limits>
#include <cmath>
#include <ctime>


#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/algorithm/string.hpp>


#include "Day.h"

#include "API.h"

#include "Utility.h"

#include "Ingredient.h";

#include "Meal.h"


using namespace std; 

using namespace boost::archive;

using namespace  boost::algorithm;

using namespace boost;


class Human {

public:

	double proteinGoal, carbGoal, fatGoal, caloricGoal;

	double weight;



};

class Tracker {

public:

	unordered_map<string, Ingredient> allIngredients;

	unordered_map<string, Meal> mealList;

	list<string> text;

	Human h;

	Day d;

	int lineCount;

	void readStats();

	void inputIngredient();

	void inputMeal();

	void debug();

	void track();

	void log();

	void reset();


	string mealAlreadyHasFood_Warning(string meal);


	void updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion, bool roundProportion);

	void updateIngredientFromMeal(Meal& m, string proportion, bool roundProportion);

	bool checkIfIngredientExists(string name);

	bool checkIfMealExists(string name);

};