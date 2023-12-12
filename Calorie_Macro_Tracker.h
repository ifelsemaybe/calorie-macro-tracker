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


using namespace std; 

using namespace boost::archive;

using namespace  boost::algorithm;

using namespace boost;


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

class Meal {

public:

	string name, proportion; //proportion only in grams

	double cal;

	double carbs, protein, fat;

	vector<Ingredient> ingredientList;

	Meal();

	Meal(string name, vector<Ingredient> ingredientList, string proportion);

	Meal(string name, vector<Ingredient> ingredientList, double cal, double protein, double carbs, double fat, string proportion);

	friend ostream& operator << (ostream& out, const Meal& meal);

};


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


	void updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion);

	void updateIngredientFromMeal(Meal& m, string proportion);

	string displayFood(string name, double cal, double protein, double carbs, double fat, string proportion, bool toConsole = false);

	bool checkIfIngredientExists(string name);

	bool checkIfMealExists(string name);
	
	string myRound(double d, int decimalPlace);

};