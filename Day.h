#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Utility.h"

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/algorithm/string.hpp>


using namespace std;
using namespace boost::archive;
using namespace boost;


class mealTime {

public:

	template<class Archive>

	void serialize(Archive& ar, const unsigned int version) {

		ar & protein;

		ar & carb;

		ar & fat;

		ar & calories;

		ar & foods;

		ar & name;

		ar & order;

		ar& stored;

	}

	mealTime();
	mealTime(string name, int order);

	double protein, carb, fat, calories;

	string foods;

	string name;

	bool stored;

	int order;

};

class Day {

public:

	template<class Archive>

	void serialize(Archive& ar, const unsigned int version) {

		ar & listOfMealTimes;

		ar & weight;

		ar & date;

	}

	Day();

	friend ostream& operator << (ostream& out, Day& day);

	double weight;
	string date;

	vector<mealTime> listOfMealTimes;

	string displayDay(bool skip = false);
	
	bool checkIfDayIsNotSetUp();

	void dayInit();

	void add(string name);

	void remove(mealTime& mT);

	void switchOrder(mealTime& mT1, mealTime& mT2);





	friend ostream& operator << (ostream& out, const Day& day);

};