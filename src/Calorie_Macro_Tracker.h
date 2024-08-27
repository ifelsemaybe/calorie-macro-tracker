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

#include "Ingredient.h"

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

	unordered_map<int, Ingredient> allIngredients;

	unordered_map<int, Meal> allMeals;

	Human h;

	Day d;

	

	bool sql_mealIsLogged;
	int sql_mealID = -1;


	void readStats();

	void inputIngredient();

	void inputMeal();

	void debug();

	void track();

	void log();

	void reset();


	string mealAlreadyHasFood_Warning(string meal);


	void updateIngredientInMeal(Ingredient &ing, Meal &m, double proportionValue, string proportionType, bool countMeal = true);

	void updateIngredientFromMeal(Meal& m, double proportionValue, string proportionType);

	int checkIfIngredientExists(string name, string brand);

	void askForIngredient(int& id, string& name, string& brand);

	int checkIfMealExists(string name, int variation);

};
