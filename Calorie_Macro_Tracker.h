#pragma once

#include <string>
#include <vector>
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


using namespace std; 

using namespace boost::archive;

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

class Day {

public:

	template<class Archive>
	
	void serialize(Archive& ar, const unsigned int version) {

		ar & proteinBreakfast;

		ar & carbBreakfast;

		ar & fatBreakfast;

		ar & caloricBreakfast;

		ar & proteinLunch;

		ar & carbLunch;

		ar & fatLunch;

		ar & caloricLunch;

		ar & proteinDinner;

		ar & carbDinner;

		ar & fatDinner;

		ar & caloricDinner;

		ar & weight;

		ar & foodsBreakfast;

		ar & foodsLunch;

		ar & foodsDinner;

		ar& date;

	}

	double proteinBreakfast, carbBreakfast, fatBreakfast, caloricBreakfast;

	double proteinLunch, carbLunch, fatLunch, caloricLunch;

	double proteinDinner, carbDinner, fatDinner, caloricDinner;

	double weight;

	string foodsBreakfast = "[Foods ==> ";
	
	string foodsLunch = "[Foods ==> ";
		
	string foodsDinner = "[Foods ==> ";

	string date;

	friend ostream& operator << (ostream& out, const Day& day);

};

class Human {

public:

	double proteinGoal, carbGoal, fatGoal, caloricGoal;

	double weight;



};

class Log {

public:

	int currentDay = 1;
		
	int	currentWeek = 1;

	double avgWeight;

	string to_log;

	bool appendNewWeek, appendEndofWeek;

};

class Tracker {

public:

	unordered_map<string, Ingredient> allIngredients;

	unordered_map<string, Meal> mealList;

	list<string> text;

	Human h;

	Day d;

	Log l;

	int lineCount;

	void readStats();

	void inputIngredient();

	void inputMeal();

	void debug();

	void track();


	void updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion, bool roundProportion);

	string displayFood(string name, double cal, double protein, double carbs, double fat, string proportion);

};