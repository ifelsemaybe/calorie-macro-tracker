#include "Calorie_Macro_Tracker.h"

Tracker t = Tracker();

string getTime() {

	time_t t = time(NULL);

	struct tm time;
	
	localtime_s(&time, &t); // for windows use

	return to_string(time.tm_mon + 1) + "/" + to_string(time.tm_mday) + "/" + to_string(1900 + time.tm_year);

}


string Tracker::myRound(double d, int decimalPlace) {

	decimalPlace = pow(10, decimalPlace);

	d = round(d * decimalPlace) / decimalPlace;

	string d_str = to_string(d);

	d_str = d_str.substr(0, d_str.find_last_not_of('0') + 1);

	if (d_str.find('.') == d_str.size() - 1) {

		d_str = d_str.substr(0, d_str.size() - 1);

	}

	return d_str;

}

string toMl(double proportion, string mesure) {

	if (mesure == "tsp") {

		return to_string(proportion * 4.928922) + "ml";
	}

	else if (mesure == "tbsp") {

		return to_string(proportion * 14.786765) + "ml";
	}

	else if (mesure == "cup") {

		return to_string(proportion * 236.588236) + "ml";

	}

}

string findVolumetricMesure(string proportion) {

	if (proportion.find("tsp") != -1) {

		proportion = toMl(stod(proportion.substr(0, proportion.find("tsp"))), "tsp");

	}

	else if (proportion.find("tbsp") != -1) {

		proportion = toMl(stod(proportion.substr(0, proportion.find("tbsp"))), "tbsp");

	}

	else if (proportion.find("cup") != -1) {

		proportion = toMl(stod(proportion.substr(0, proportion.find("cup"))), "cup");

	}

	return proportion;

}

list<string> readFoodStats(list<string> text) {

	ifstream myFile("Food Stats.txt");

	string line;

	while (getline(myFile, line)) {

		if (line.empty()) {

			text.push_back("\n");

		}

		else {

			text.push_back(line + "\n");

		}

	}

	text.back() = line; //so that we remove the "\n" on the last line of the text

	myFile.close();

	return text;

}

Ingredient::Ingredient() {}

Ingredient::Ingredient(string name, double cal, double protein, double carbs, double fat, string proportion) {

	this->name = name;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->proportion = proportion;

}

Ingredient::Ingredient(const Ingredient& copy) {

	cal = copy.cal;
	carbs = copy.carbs;
	protein = copy.protein;
	fat = copy.fat;
	name = copy.name;
	proportion = copy.proportion;

}

ostream& operator << (ostream& out, const Ingredient& ing) {

	out << t.displayFood(ing.name, ing.cal, ing.protein, ing.carbs, ing.fat, ing.proportion);

	return out;

}

Meal::Meal() {
	
	cal = 0;
	carbs = 0;
	protein = 0;
	fat = 0;

}

Meal::Meal(string name, vector<Ingredient> ingredientList, string proportion) {

	this->name = name;
	this->ingredientList = ingredientList;
	this->proportion = proportion;

}

Meal::Meal(string name, vector<Ingredient> ingredientList, double cal, double protein, double carbs, double fat, string proportion) {

	this->name = name;
	this->ingredientList = ingredientList;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->proportion = proportion;

}

ostream& operator << (ostream& out, const Meal& meal) {

	out << t.displayFood(meal.name, meal.cal, meal.protein, meal.carbs, meal.fat, meal.proportion) + "\n\n";
	out << "List of Ingredients: \n\n\n";

	for (auto& el : meal.ingredientList) {

		out << el << "\n\n";

	}

	return out;

}

void Tracker::readStats() {

	ifstream myFile;
	myFile.open("Food Stats.txt");

	string line;
	bool switch_ = false;
	bool emptyx2 = false;

	while (getline(myFile, line)) {

		if (line.empty() || line == "(protein/carbs/fat) (proportion)" || line == "Ingredients:") continue;

		if (line == "Meals:") {

			switch_ = true;

			continue;

		}
		
		if (!switch_) {

			string name = line.substr(0, line.find(":"));

			line.erase(0, line.find(":") + 2);

			int cal = stoi(line.substr(0, line.find(" ")));

			line.erase(0, line.find("cal") + 5);

			double protein = stod(line.substr(0, line.find("g/")));

			line.erase(0, line.find("g/") + 2);

			double carb = stod(line.substr(0, line.find("g/")));

			line.erase(0, line.find("g/") + 2);

			double fat = stod(line.substr(0, line.find("g/")));

			line.erase(0, line.find("g") + 2);

			string proportion;

			if (!line.empty()) {

				line.erase(0, 2);

				proportion = line.substr(0, line.find(")"));

				proportion = findVolumetricMesure(proportion);

			} else proportion = "";

			allIngredients[name] = Ingredient(name, cal, protein, carb, fat, proportion);

		}

		if (switch_) {

			Meal temp_meal = Meal();

			string temp = line.substr(0, line.find(":"));

			if (temp.find("(") != -1) {

				temp_meal.name = temp.substr(0, temp.find("(") - 1);
				temp_meal.proportion = temp.substr(temp.find("(") + 1, temp.find(")"));


				temp_meal.proportion = findVolumetricMesure(temp_meal.proportion);

			}

			else {

				temp_meal.name = temp;
				temp_meal.proportion = "";

			}

			line.erase(0, line.find(":") + 2);

			while (!line.empty()) {

				string temp;

				if (line.find(",") != -1) {

					temp = line.substr(0, line.find(",") + 1);

				}

				else {

					temp = line;

				}

				if (temp.find("(") != -1) {

					string ingredient_name = line.substr(0, line.find("(") - 1);

					line.erase(0, line.find("(") + 1);

					string ingredient_newProportionStr = line.substr(0, line.find(")"));

					ingredient_newProportionStr = findVolumetricMesure(ingredient_newProportionStr);

					if (line.find(",") != -1) {

						line.erase(0, line.find(",") + 2);

					}
					else line.erase(0);

					Ingredient ingredient = allIngredients[ingredient_name];


					updateIngredientInMeal(ingredient, temp_meal, ingredient_newProportionStr);

					temp_meal.ingredientList.push_back(ingredient);


				}

				else { //no new proportion

					string ingredient_name;

					if (line.find(",") != -1) {

						ingredient_name = line.substr(0, line.find(","));

						line.erase(0, line.find(",") + 2);

					}

					else {

						ingredient_name = line;

						line.erase(0);

					}

					Ingredient ingredient = allIngredients[ingredient_name];

					updateIngredientInMeal(ingredient, temp_meal, "");

					temp_meal.ingredientList.push_back(ingredient);

				}

			}
			
			mealList[temp_meal.name] = temp_meal;

		}


	}

	myFile.close();

}

void Tracker::inputIngredient() {

	ifstream readFile;
	readFile.open("Food Stats.txt");

	string line;

	lineCount = 0;

	while (getline(readFile, line)) {

		if (line.empty() || line == "(protein/carbs/fat) (proportion)" || line == "Ingredients:") continue;

		else if (line == "Meals:") break;

		lineCount++;

	}

	readFile.close();

	string input;

	string name;
	double calories;

	double protein, carbs, fat;
		
	string proportion;


	cout << "name? ";

	getline(cin, name);

	cout << endl;

	cout << "calories? ";

	cin >> calories;

	cout << endl;

	cout << "protein? ";

	cin >> protein;

	cout << endl;

	cout << "carbs? ";

	cin >> carbs;

	cout << endl;

	cout << "fat? ";

	cin >> fat;

	cout << endl;

	cout << "proportion? ";

	cin >> proportion;

	cout << endl;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');


	proportion = findVolumetricMesure(proportion);

	input = displayFood(name, calories, protein, carbs, fat, proportion);

	allIngredients[name] = (proportion == "null" || proportion == "n") ? Ingredient(name, calories, protein, carbs, fat, "") : Ingredient(name, calories, protein, carbs, fat, proportion);

	text.clear();

	text = readFoodStats(text);

	list<string>::iterator it = text.begin();

	std::advance(it, lineCount * 2 + 3);

	lineCount += 1;

	text.insert(it, "\n");

	text.insert(it, input + "\n");


	ofstream myFile = ofstream();

	myFile.open("Food Stats.txt");

	for (it = text.begin(); it != text.end(); it++) {

		myFile << *it;

	}

	myFile.close();

}

void Tracker::inputMeal() {

	string name, name_;

	string proportion; //proportion for ingredients

	string proportion_Meal;

	Meal m = Meal();

	cout << "Meal name? ";

	getline(cin, name);

	m.name = name;

	cout << endl;

	cout << "Meal Proportion? ";

	cin >> proportion_Meal;

	//If not in grams throw an exception

	m.proportion = proportion_Meal;


	m.proportion = findVolumetricMesure(m.proportion);


	cout << endl;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	bool end = false;

	int itr = 1;

	while (!end) {

		do{
			
			cout << "Ingredient " + to_string(itr) + " name? ";

			getline(cin, name_);

			if (name_ == "input ingredient" || name_ == "input i") {

				cout << "\n";

				inputIngredient();

				cout << "Ingredient " + to_string(itr) + " name? ";

				getline(cin, name_);

			}

		} while (!checkIfIngredientExists(name_));


		if (name_ == "e" || name_ == "exit") {

			end = true;
			continue;

		}

		cout << "Ingredient " + to_string(itr) + " proportion? ";

		getline(cin, proportion);

		if (proportion == "e" || proportion == "exit") {

			end = true;
			continue;

		}

		proportion = findVolumetricMesure(proportion);
		
		itr++;

		cout << endl;

		Ingredient ing = allIngredients[name_];

		updateIngredientInMeal(ing, m, proportion);

		m.ingredientList.push_back(ing);

	}

	cout << "\n" << endl;

	mealList[name] = m;

	string input;

	//Very Ugly written code below, might need to rewrite and remove repeating code by making helper methods :/

	if (m.ingredientList[0].proportion == allIngredients[m.ingredientList[0].name].proportion) { //note: if proportion are equal then null was used!

			if (m.proportion == "null" || m.proportion == "n") {

				input = name + ": " + m.ingredientList[0].name;

			}

			else if (m.proportion.find("g") != -1) {

				input = name + " (" + myRound(stod(m.proportion.substr(0,m.proportion.find("g"))), 1) + "g): " + m.ingredientList[0].name;

			}

			else if (m.proportion.find("ml") != -1) {

				input = name + " (" + myRound(stod(m.proportion.substr(0, m.proportion.find("ml"))), 1) + "ml): " + m.ingredientList[0].name;

			}

			else if (m.proportion[0] == 'x') {

				input = name + " (x" + myRound(stod(m.proportion.substr(1)), 1) + "): " + m.ingredientList[0].name;

			}


	}

	else {

		//Rounding first ingredient proportion!

		if (m.ingredientList[0].proportion.find("g") != -1) {

			m.ingredientList[0].proportion = myRound(stod(m.ingredientList[0].proportion.substr(0, m.ingredientList[0].proportion.find("g"))), 1) + "g";
		}

		else if (m.ingredientList[0].proportion.find("ml") != -1) {

			m.ingredientList[0].proportion = myRound(stod(m.ingredientList[0].proportion.substr(0, m.ingredientList[0].proportion.find("ml"))), 1) + "ml";

		}

		else if (m.ingredientList[0].proportion[0] = 'x') {

			m.ingredientList[0].proportion = "x" + myRound(stod(m.ingredientList[0].proportion.substr(1)), 1);

		}

		//Rounding meal proportion and adding first ingredient and its rounded proportion!

		if (m.proportion == "null" || m.proportion == "n") {

			input = name + ": " + m.ingredientList[0].name + " (" + m.ingredientList[0].proportion + ")";

		}

		else if (m.proportion.find("g") != -1) {

			input = name + " (" + myRound(stod(m.proportion.substr(0, m.proportion.find("g"))), 1) + "g): " + m.ingredientList[0].name + " (" + m.ingredientList[0].proportion + ")";

		}

		else if (m.proportion.find("ml") != -1) {

			input = name + " (" + myRound(stod(m.proportion.substr(0, m.proportion.find("ml"))), 1) + "ml): " + m.ingredientList[0].name + " (" + m.ingredientList[0].proportion + ")";

		}

		else if (m.proportion[0] = 'x') {

			input = name + " (x" + myRound(stod(m.proportion.substr(1)), 1) + "): " + m.ingredientList[0].name + " (" + m.ingredientList[0].proportion + ")";

		}

	}
	
	for (int i = 1; i < m.ingredientList.size(); i++) {

		if (m.ingredientList[i].proportion == allIngredients[m.ingredientList[i].name].proportion) { //note: if proportion are equal then null was used!

			input += ", " + m.ingredientList[i].name;
		}

		else {

			
			if (m.ingredientList[i].proportion.find("g") != -1) {

				input += ", " + m.ingredientList[i].name + " (" + myRound(stod(m.ingredientList[i].proportion.substr(0, m.ingredientList[i].proportion.find("g"))), 1) + "g)";

			}

			else if (m.ingredientList[i].proportion.find("ml") != -1) {

				input += ", " + m.ingredientList[i].name + " (" + myRound(stod(m.ingredientList[i].proportion.substr(0, m.ingredientList[i].proportion.find("ml"))), 1) + "ml)";

			}

			else if (m.ingredientList[i].proportion[0] = 'x') {

				input += ", " + m.ingredientList[i].name + " (x" + myRound(stod(m.ingredientList[i].proportion.substr(1)), 1) + ")";

			}

		}

	}

	ofstream myFile = ofstream();

	myFile.open("Food Stats.txt", ios::app);

	myFile << "\n\n" << input;

	myFile.close();

}

void Tracker::track() {

	string answer, name, proportion;

	string foods;

	double cal = 0;

	double protein = 0, carbs = 0, fat = 0;

	bool exit = false;

	mealTime selected = d.listOfMealTimes[0];

	int numberOfMealTimes = d.listOfMealTimes.size();

	for (mealTime& mealT : d.listOfMealTimes) {

		if (mealT.stored == false) {

			selected = mealT;

			break;

		}

	}

	cout << "Values stored in " + selected.name + " [" + to_string(selected.order) + "/" + to_string(numberOfMealTimes) + "]\n\n";

	while (!exit) {


		cout << "ingredient or meal? ";

		getline(cin, answer);

		cout << "\n" << endl;

		to_lower(answer);

		if (answer == "ingredient" || answer == "i") {

			do {

				cout << "name? ";

				getline(cin, name);

				cout << "\n" << endl;

			} while (!checkIfIngredientExists(name));


			cout << "proportion? ";

			cin >> proportion;

			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "\n" << endl;

			proportion = findVolumetricMesure(proportion);

			if (proportion == "null" || proportion == "n"){

				cal += allIngredients[name].cal;

				protein += allIngredients[name].protein;
				carbs += allIngredients[name].carbs;
				fat += allIngredients[name].fat;

				foods += displayFood(name, allIngredients[name].cal, allIngredients[name].protein, allIngredients[name].carbs, allIngredients[name].fat, proportion) + " - ";

				cout << displayFood(name, allIngredients[name].cal, allIngredients[name].protein, allIngredients[name].carbs, allIngredients[name].fat, proportion) << "\n\n\n";

			}

			else {

				double ratio;

				if (proportion[0] == 'x') {

					if (allIngredients[name].proportion[0] == 'x') {

						ratio = stod(proportion.substr(1)) / stod(allIngredients[name].proportion.substr(1));

						cal += (allIngredients[name].cal * ratio);
						protein += (allIngredients[name].protein * ratio);
						carbs += (allIngredients[name].carbs * ratio);
						fat += (allIngredients[name].fat * ratio);

						//A property boolean for displayFood to cout would not be a bad idea!

						foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion, true) + " - ";

						cout << "\n\n\n";

					}

					else {

						ratio = stod(proportion.substr(1));

						cal += (allIngredients[name].cal * ratio);
						protein += (allIngredients[name].protein * ratio);
						carbs += (allIngredients[name].carbs * ratio);
						fat += (allIngredients[name].fat * ratio);


						if (allIngredients[name].proportion.find("g") != -1) {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, to_string(stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("g"))) * ratio) + "g", true) + " - ";

							cout << "\n\n\n";

						}

						else if (allIngredients[name].proportion.find("ml") != -1) {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, to_string(stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("ml"))) * ratio) + "ml", true) + " - ";

							cout << "\n\n\n";

						}

						else {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion, true) + " - ";
							cout << "\n\n\n";

						}

					}
				}

				else if (proportion.find("g") != -1) {
	
					//if there's a mismatch in proportion mesures throw error 
	
					ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("g")));
	
					cal += (allIngredients[name].cal * ratio);
					protein += (allIngredients[name].protein * ratio);
					carbs += (allIngredients[name].carbs * ratio);
					fat += (allIngredients[name].fat * ratio);
	
					foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion, true) + " - ";
					cout << "\n\n\n";

				}
	
				else {
	
					ratio = stod(proportion.substr(0, proportion.find("ml"))) / stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("ml")));
	
					cal += (allIngredients[name].cal * ratio);
					protein += (allIngredients[name].protein * ratio);
					carbs += (allIngredients[name].carbs * ratio);
					fat += (allIngredients[name].fat * ratio);
	
					foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion, true) + " - ";
					cout << "\n\n\n";


				}

			}

		}

		else if (answer == "meal" || answer == "m") {

		do {

			cout << "name? ";

			getline(cin, name);

			cout << "\n" << endl;

		} while (!checkIfMealExists(name));



			Meal m = mealList[name];

			string answer;
			bool exit = false;

			while (!exit) {

				cout << "What do you want to do? ";

				getline(cin, answer);

				cout << "\n" << endl;

				if (answer == "Change Ingredient(s) Proportion" || answer == "cip") {

					bool exit = false;
					string name_;
					Ingredient ing;
					bool found = false;

					while (!exit) {

						do {

							cout << "Name of Ingredient? ";

							getline(cin, name_);

							cout << "\n" << endl;

						} while (!checkIfIngredientExists(name_));



						if (name_ == "exit" || name_ == "e") {

							exit = true;

							continue;

						}

						else {

							for (Ingredient& i : m.ingredientList) {

								if (i.name == name_) {

									found = true;
									ing = i;
									break;
								}

							}

							if (!found) {

								cout << "Sorry your ingredient is not a part of the meal's ingredient list! Try again!\n\n";
								continue;

							}

							string proportion;

							cout << "proportion? ";

							cin >> proportion;

							cin.ignore(numeric_limits<streamsize>::max(), '\n');

							cout << "\n" << endl;


							if (proportion == "exit" || proportion == "e") {

								exit = true;
								continue;

							}

							proportion = findVolumetricMesure(proportion);

							m.cal -= ing.cal;

							m.protein -= ing.protein;
							m.carbs -= ing.carbs;
							m.fat -= ing.fat;

							updateIngredientInMeal(ing, m, proportion);


						}

					}

				}

				else if (answer == "Change Meal Proportion" || answer == "cmp") {

					string proportion;

					cout << "What is the new proportion for the meal? ";

					cin >> proportion;

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					cout << "\n" << endl;
					
					proportion = findVolumetricMesure(proportion);

					updateIngredientFromMeal(m, proportion);

				}

				else if (answer == "exit" || "e") {

					foods += displayFood(m.name, m.cal, m.protein, m.carbs, m.fat, m.proportion) + " - ";

					exit = true;
					continue;
				}


			}



			cal += m.cal;

			protein += m.protein;
			carbs += m.carbs;
			fat += m.fat;

			cout << displayFood(m.name, m.cal, m.protein, m.carbs, m.fat, m.proportion) << "\n\n\n";

		}

		if (answer == "input ingredient" || answer == "input i") {

			inputIngredient();

		}

		else if (answer == "input meal" || answer == "input m") {

			inputMeal();
		}

		else if	(answer == "exit" || answer == "e") {

			exit = true;
		}

		else if (answer.find(" ") != -1) {

			vector<string> answerSplit;

			split(answerSplit, answer, is_any_of(" "));

			if (answerSplit[0] + " " + answerSplit[1] == "switch to" || answerSplit[0] == "sw") {

				string mealTimeName = "";

				if (answerSplit[0] == "sw") {

					for (int x = 1; x < answerSplit.size(); x++) {

						mealTimeName += answerSplit[x];
					}

				}

				else {

					for (int x = 2; x < answerSplit.size(); x++) {

						mealTimeName += answerSplit[x];
					}

				}

				bool found = false;

				for (mealTime mT : d.listOfMealTimes) {

					to_lower(mT.name);

					if (mealTimeName == mT.name) {

						selected = d.listOfMealTimes[mT.order-1];
						found = true;

						break;

					}

				}

				if (!found) {

					cout << "Sorry! " + mealTimeName + " was not found. Current meal time remains the same. \n\n";

				}

				else {

					cout << "Values stored in " + selected.name + " [" + to_string(selected.order) + "/" + to_string(numberOfMealTimes) + "]\n\n";

				}

			}

		}

	}

	cout << displayFood("Total", cal, protein, carbs, fat, "") << "\n\n\n";

	if (d.weight == 0) {

		cout << "What is your weight? ";

		cin >> d.weight;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n" << endl;

		d.date = getTime();

	}

	if (selected.stored) {

		string yesOrNo = mealAlreadyHasFood_Warning(selected.name);

		to_lower(yesOrNo);

		if (yesOrNo == "n" || yesOrNo == "no") {

			return;

		}

	}

	selected.calories += cal;
	selected.protein += protein;
	selected.carb += carbs;
	selected.fat += fat;
	selected.foods += foods;

	selected.stored = true;

	d.listOfMealTimes[selected.order - 1] = selected;

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << d;

	}

	ofs.close();

}

void Tracker::log() {

	ifstream ifile("log.txt");

	ofstream ofile("log.txt", ios::app);

	string line;

	int currentDay = 1;
	int	currentWeek = 1;
	string to_log;
	bool appendNewWeek = false, appendEndofWeek = false;

	double cal = 0;
	double protein = 0;
	double carbs = 0;
	double fat = 0;

	double weight = 0;

	while (getline(ifile, line)) {

		if (line.find("Week") != -1 && line.find("average") == -1) {

			currentWeek++;
			weight = 0;

		}

		if (line.find("average") != -1) {

			currentDay = 1;

			cal = 0;
			protein = 0;
			carbs = 0;
			fat = 0;

		}

		if (line.find("Day") != -1) {

			currentDay++;

		}

		if (line.find("Total") != -1) {

			line.erase(0, line.find(":") + 2);

			cal += stod(line.substr(0, line.find(" ")));

			line.erase(0, line.find("(") + 1);

			protein += stod(line.substr(0, line.find("g")));

			line.erase(0, line.find("/") + 1);

			carbs += stod(line.substr(0, line.find("g")));

			line.erase(0, line.find("/") + 1);

			fat += stod(line.substr(0, line.find("g")));

		}

		if (line.find("Weight") != -1) {

			line.erase(0, line.find(" ") + 1);

			weight += stod(line.substr(0, line.find(" kg")));

		}

	}

	if (currentDay == 7) {

		appendEndofWeek = true;

	}

	else if (currentDay == 1) {

		appendNewWeek = true;
	}

	if (appendNewWeek) {


		to_log += "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\nWeek " + to_string(currentWeek) + "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n";

	}

	to_log += "Day " + to_string(currentDay) + " (" + d.date + "):\n------------------\n\n\n";

	to_log += d.displayDay(true) + "\n\n\n\n";

	if (appendEndofWeek) {

		double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;

		for (mealTime& mT : d.listOfMealTimes) {

			totalCalories += mT.calories;
			totalProtein += mT.protein;
			totalCarbs += mT.carb;
			totalFat += mT.fat;

		}

		//Since you've already rounded all the previous days to your log file, it makes sense as to rounding also your last day too, when averaging them out.

		cal += stod(myRound(totalCalories, 0));

		double cal_avg = cal / 7;

		protein += stod(myRound(totalProtein, 1));
		carbs += stod(myRound(totalCarbs, 1));
		fat += stod(myRound(totalFat, 1));

		double protein_avg = protein / 7;
		double carbs_avg = carbs / 7;
		double fat_avg = fat / 7;

		weight += stod(myRound(d.weight, 2));

		double weight_avg = weight / 7;

		to_log += "Week's average:\n\n\n\t" + displayFood("Calories\\Macros", cal_avg, protein_avg, carbs_avg, fat_avg, "null") + "\n\n\tWeight: " + myRound(weight_avg, 2) + " kg\n\n\n";

	}

	ofile << to_log;

	cout << d << "\n\n\n\n";

	reset();

	ifile.close();
	ofile.close();

}

void Tracker::debug() {

	for (auto& el : allIngredients){

		cout << el.second << "\n\n";

	}

	cout << "\n\n\n\n";

	for (auto& el : mealList) {

		cout << el.second << "\n\n";

	}

	cout << "\n\nMealTimes Order:\n\n";

	for (mealTime& mT : d.listOfMealTimes) {

		if (mT.order == d.listOfMealTimes.size()) {

			cout << mT.name + "[" + to_string(mT.order) + "]";

			break;

		}

		cout << mT.name + " [" + to_string(mT.order) + "] --> ";

	}

	cout << "\n\n\n\n\n\n" << d << "\n\n\n\n";

}

void Tracker::reset() { 

	for (mealTime& mT : d.listOfMealTimes) {

		mT.calories = 0;
		mT.protein = 0;
		mT.carb = 0;
		mT.fat = 0;
		mT.foods = "[Foods ==> ";
		mT.stored = false;

	}

	d.weight = 0;
	d.date = "";

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << d;

	}

}


string Tracker::mealAlreadyHasFood_Warning(string meal) {

	cout << "Warning! Your MEAL [" + meal + "] already has an input.\n\nDo you still wish to input these new values (y/n): ";

	string answer;

	getline(cin, answer);

	cout << "\n\n";

	return answer;

}

bool Tracker::checkIfIngredientExists(string name) {


	if (allIngredients.count(name) == 1 || name == "e" || name == "exit") {

		return true;
	}

	cout << "Sorry, the ingredient |" + name + "| cannot be found, try again!";

	cout << "\n" << endl;

	return false;

}

bool Tracker::checkIfMealExists(string name) {


	if (mealList.count(name) == 1 || name == "e" || name == "exit") {

		return true;
	}

	cout << "Sorry, the meal |" + name + "| cannot be found, try again!";

	cout << "\n" << endl;

	return false;

}

string Tracker::displayFood(string name, double cal, double protein, double carbs, double fat, string proportion, bool toConsole) {

	if (proportion == "null" || proportion == "n" || proportion == "") {

		if (toConsole) {

			cout << name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

		}

		return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

	}


	if (proportion[0] == 'x') {

		proportion = "x" + myRound(stod(proportion.substr(1)), 1);

	}

	else if (proportion.find("g") != -1) {

		proportion = myRound(stod(proportion.substr(0, proportion.find("g"))), 1) + "g";

	}

	else {

		proportion = myRound(stod(proportion.substr(0, proportion.find("ml"))), 1) + "ml";

	}

	if (toConsole) {

		cout << name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

	}

	return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

}

void Tracker::updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion) {

	if (proportion[0] == 'x') {


		if (ing.proportion[0] == 'x') {

			double ratio = stod(proportion.substr(1)) / stod(ing.proportion.substr(1));

			ing.cal *= ratio;
			ing.protein *= ratio;
			ing.carbs *= ratio;
			ing.fat *= ratio;

			ing.proportion = proportion;

		}

		else if (ing.proportion.find("g") != -1) {

			ing.cal *= stod(proportion.substr(1));
			ing.protein *= stod(proportion.substr(1));
			ing.carbs *= stod(proportion.substr(1));
			ing.fat *= stod(proportion.substr(1));

			ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * stod(proportion.substr(1))) + "g";

		}

		else if (ing.proportion.find("ml") != -1) {

			ing.cal *= stod(proportion.substr(1));
			ing.protein *= stod(proportion.substr(1));
			ing.carbs *= stod(proportion.substr(1));
			ing.fat *= stod(proportion.substr(1));

			ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * stod(proportion.substr(1))) + "ml";

		}

		else {

			ing.cal *= stod(proportion.substr(1));
			ing.protein *= stod(proportion.substr(1));
			ing.carbs *= stod(proportion.substr(1));
			ing.fat *= stod(proportion.substr(1));

			ing.proportion = proportion;

		}

	}

	else if (proportion.find("g") != -1) {

		//throw exception if proportion doesn't match with the allIngredients list


		double ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(ing.proportion.substr(0, ing.proportion.find("g")));

		ing.cal *= ratio;
		ing.protein *= ratio;
		ing.carbs *= ratio;
		ing.fat *= ratio;

		ing.proportion = proportion;

	}

	else if (proportion.find("ml") != -1) {

		//throw exception if proportion doesn't match with the allIngredients list


		double ratio = stod(proportion.substr(0, proportion.find("ml"))) / stod(ing.proportion.substr(0, ing.proportion.find("ml")));

		ing.cal *= ratio;
		ing.protein *= ratio;
		ing.carbs *= ratio;
		ing.fat *= ratio;

		ing.proportion = proportion;

	}

	m.cal += ing.cal;

	m.protein += ing.protein;
	m.carbs += ing.carbs;
	m.fat += ing.fat;

}

void Tracker::updateIngredientFromMeal(Meal& m, string proportion) {

	if (proportion[0] == 'x') {

		if (m.proportion[0] == 'x') {

			double ratio = stod(proportion.substr(1)) / stod(m.proportion.substr(1));

			m.cal *= ratio;
			m.protein *= ratio;
			m.carbs *= ratio;
			m.fat *= ratio;

			m.proportion = proportion;

			for (Ingredient& ing : m.ingredientList) {

				ing.cal *= ratio;
				ing.protein *= ratio;
				ing.carbs *= ratio;
				ing.fat *= ratio;

				if (ing.proportion[0] == 'x') {

					ing.proportion = "x" + to_string(stod(ing.proportion.substr(1)) * ratio);

				}

				else if (ing.proportion.find("g") != -1) {

					ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio) + "g";
				}

				else if (ing.proportion.find("ml") != -1) {

					ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio) + "ml";
				}

				else {

					ing.proportion = "x" + to_string(ratio);

				}
			}

		}

		else {

			m.cal *= stod(proportion.substr(1));
			m.protein *= stod(proportion.substr(1));
			m.carbs *= stod(proportion.substr(1));
			m.fat *= stod(proportion.substr(1));

			if (m.proportion.find("g") != -1) {

				m.proportion = to_string(stod(m.proportion.substr(0, m.proportion.find("g"))) * stod(proportion.substr(1))) + "g";

			}

			else if (m.proportion.find("ml") != -1) {

				m.proportion = to_string(stod(m.proportion.substr(0, m.proportion.find("ml"))) * stod(proportion.substr(1))) + "ml";

			}

			else {

				m.proportion = proportion;

			}

			for (Ingredient& ing : m.ingredientList) {

				ing.cal *= stod(proportion.substr(1));
				ing.protein *= stod(proportion.substr(1));
				ing.carbs *= stod(proportion.substr(1));
				ing.fat *= stod(proportion.substr(1));

				if (ing.proportion[0] == 'x') {

					ing.proportion = "x" + to_string(stod(ing.proportion.substr(1)) * stod(proportion.substr(1)));

				}

				else if (ing.proportion.find("g") != -1) {

					ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * stod(proportion.substr(1))) + "g";
				}

				else if (ing.proportion.find("ml") != -1) {

					ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * stod(proportion.substr(1))) + "ml";
				}

				else {

					ing.proportion = proportion;

				}

			}
			
		}

	}

	else if (proportion.find("g") != -1) {

		//throw exception if proportion doesn't match with the meal proportion

		double ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(m.proportion.substr(0, m.proportion.find("g")));

		m.cal *= ratio;
		m.protein *= ratio;
		m.carbs *= ratio;
		m.fat *= ratio;

		m.proportion = proportion;

		for (Ingredient& ing : m.ingredientList) {

			ing.cal *= ratio;
			ing.protein *= ratio;
			ing.carbs *= ratio;
			ing.fat *= ratio;

			if (ing.proportion[0] == 'x') {

				ing.proportion = "x" + to_string(stod(ing.proportion.substr(1)) * ratio);

			}

			else if (ing.proportion.find("g") != -1) {

				ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio) + "g";
			}

			else if (ing.proportion.find("ml") != -1) {

				ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio) + "ml";
			}

			else {

				ing.proportion = "x" + to_string(ratio);

			}

		}

	}

	else if (proportion.find("ml") != -1) {

		//throw exception if proportion doesn't match with the meal proportion

		double ratio = stod(proportion.substr(0, proportion.find("ml"))) / stod(m.proportion.substr(0, m.proportion.find("ml")));

		m.proportion = proportion;

		m.cal *= ratio;
		m.protein *= ratio;
		m.carbs *= ratio;
		m.fat *= ratio;

		for (Ingredient& ing : m.ingredientList) {

			ing.cal *= ratio;
			ing.protein *= ratio;
			ing.carbs *= ratio;
			ing.fat *= ratio;

			if (ing.proportion[0] == 'x') {

				ing.proportion = "x" + to_string(stod(ing.proportion.substr(1)) * ratio);

			}

			else if (ing.proportion.find("g") != -1) {

				ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio) + "g";
			}

			else if (ing.proportion.find("ml") != -1) {

				ing.proportion = to_string(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio) + "ml";
			}

			else {

				ing.proportion = "x" + to_string(ratio);

			}

		}

	}


}