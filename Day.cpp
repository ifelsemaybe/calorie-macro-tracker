#include "Day.h";

mealTime::mealTime(){}

mealTime::mealTime(string name, int order) {

	this->foods = "[Foods ==> ";
	this->name = name;
	this->order = order;
	this->stored = false;

	this->protein = 0;
	this->carb = 0;
	this->fat = 0;
	this->calories = 0;

}


Day::Day() {};

ostream& operator << (ostream& out, Day& day) {

	out << day.displayDay();

	return out;
}

bool Day::checkIfDayIsNotSetUp() {

	return listOfMealTimes.empty();

};

void Day::dayInit() {

	cout << "Hey, your Day doesn't have any \"Meal Times\" associated with it yet.\n\n" << "How many meal times do you wish to create?: ";

	int answer;

	cin >> answer;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "\n\n" << "Please provide your meal times in chronological order.\n\n";

	for (int i = 0; i < answer; i++) {

		string name;

		cout << "Meal Time " << (i + 1) << " name: ";

		getline(cin, name);

		listOfMealTimes.push_back(mealTime(name, i + 1));

		cout << "\n";

	}

	weight = 0;
	date = "";

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << *this;

	}

	cout << endl;

}

string Day::displayDay(bool skip) {

	double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;

	string toReturn;

	for (mealTime mT : listOfMealTimes) {

		toReturn += "\t" + displayFood(mT.name, mT.calories, mT.protein, mT.protein, mT.fat, "null") + "\n\n" + mT.foods.substr(0, mT.foods.size() - 3) + "]\n\n\n";

		totalCalories += mT.calories;
		totalProtein += mT.protein;
		totalCarbs += mT.carb;
		totalFat += mT.fat;

	}

	if (weight == 0 && !skip) {

		return toReturn + "\n" + displayFood("Total", totalCalories, totalProtein, totalCarbs, totalFat, "null") + "\n\nWeight: NO INPUT";

	}

	return toReturn + "\n" + displayFood("Total", totalCalories, totalProtein, totalCarbs, totalFat, "null") + "\n\nWeight: " + myRound(weight, 2) + " kg";

}

mealTime recursiveHelp(int& counter, vector<mealTime>& listOfMealTimes) {

	if (counter == listOfMealTimes.size()) {

		counter--;

		return listOfMealTimes[counter-1];

	}

	listOfMealTimes[counter] = recursiveHelp(counter = counter + 1, listOfMealTimes);

	listOfMealTimes[counter].order++;

	counter--;

	return listOfMealTimes[(counter == 0) ? 0 : counter - 1]; //Clamp to always return positive integer, i.e. when running add ~, before breakfast

}

void Day::add(string name) {

	if (checkIfDayIsNotSetUp()) {

		listOfMealTimes.push_back(mealTime(name, 1));

		cout << name << " has been added succesfully!";

		return;

	}

	vector<string> answerSplit;
	string toVerify;
	mealTime selected;

	while (true) {

		cout << "Where do you want your new Meal Time added? ";

		string answer;

		getline(cin, answer);

		cout << "\n\n";

		to_lower(answer);

		if (answer == "e" || answer == "exit") {

			cout << "New Meal Time " << name << " not added.\n\n";

			return;
		}

		split(answerSplit, answer, is_any_of(" "));

		if ((answerSplit[0] != "before" && answerSplit[0] != "after") || answerSplit.size() < 2) {

			cout << "Sorry! Format expected is: Before/After [Meal Time Name]\n\nPlease try again.\n\n";

			continue;

		}

		for (int x = 1; x < answerSplit.size(); x++) {

			toVerify += answerSplit[x];

		}

		bool verified = false;

		for (mealTime mT : listOfMealTimes) {

			to_lower(mT.name);

			if (mT.name == toVerify) {

				verified = true;

				selected = mT;

				break;

			}
		}

		if (!verified) {

			cout << "Sorry! " << toVerify << " was not found.\n\nPlease try again.\n\n";

			continue;

		}

		break;

	}

	listOfMealTimes.resize(listOfMealTimes.size() + 1);


	int orderForNewMT = (answerSplit[0] == "before") ? selected.order : selected.order + 1; //else option is for the choice "after"... instead of "before".


	int counter = orderForNewMT;

	recursiveHelp(counter, listOfMealTimes); //counter gets passed by reference, don't want it to mess up the value of orderForNewMT.


	listOfMealTimes[orderForNewMT - 1] = mealTime(name, orderForNewMT);

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << *this;

	}

	cout << "The new Meal Time has been added Succesfully!\n\n\n\n";



}

void Day::remove(mealTime& mT) {

	for (int x = mT.order; x < listOfMealTimes.size(); x++) {

		listOfMealTimes[x].order--;

	}

	listOfMealTimes.erase(listOfMealTimes.begin() + mT.order - 1);

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << *this;

	}

	cout << "The Meal Time has been removed Succesfully!\n\n\n\n";
}

void Day::switchOrder(mealTime& mT1, mealTime& mT2) {

	listOfMealTimes[mT1.order - 1] = mT2;

	listOfMealTimes[mT1.order - 1].order = mT1.order;

	listOfMealTimes[mT2.order - 1] = mT1;

	listOfMealTimes[mT2.order - 1].order = mT2.order;

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << *this;

	}

	cout << "The Meal Time have been switched Succesfully!\n\n\n\n";

}