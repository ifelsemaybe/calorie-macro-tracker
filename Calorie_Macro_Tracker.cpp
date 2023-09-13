#include "Calorie_Macro_Tracker.h"

string getTime() {

	time_t t = time(NULL);

	struct tm time;
	
	localtime_s(&time, &t); // for windows use

	return to_string(time.tm_mon + 1) + "/" + to_string(time.tm_mday) + "/" + to_string(1900 + time.tm_year);

}


string myRound(double d, int decimalPlace) {

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

	out << ing.name + ": calories[" + myRound(ing.cal, 0) + "] protein(" + myRound(ing.protein,1) + ") carbs(" + myRound(ing.carbs,1) + ") fat(" + myRound(ing.fat,1) + ")\n\n";

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

	out << meal.name + ": calories[" + myRound(meal.cal, 0) + "] protein(" + myRound(meal.protein, 1) + ") carbs(" + myRound(meal.carbs, 1) + ") fat(" + myRound(meal.fat, 1) + ")\n\n";

	out << "List of Ingredients: \n\n";

	for (auto& el : meal.ingredientList) {

		out << el;

	}

	out << "\n\n";

	return out;

}

ostream& operator << (ostream& out, const Day& day) {

	out << "Breakfast: \n\n";

	out << "calories[" + myRound(day.caloricBreakfast, 0) + "] protein(" + myRound(day.proteinBreakfast, 1) + ") carbs(" + myRound(day.carbBreakfast, 1) + ") fat(" + myRound(day.fatBreakfast, 1) + ")\n\n";

	out << "Lunch: \n\n";

	out << "calories[" + myRound(day.caloricLunch, 0) + "] protein(" + myRound(day.proteinLunch, 1) + ") carbs(" + myRound(day.carbLunch, 1) + ") fat(" + myRound(day.fatLunch, 1) + ")\n\n";

	out << "Dinner: \n\n";

	out << "calories[" + myRound(day.caloricDinner, 0) + "] protein(" + myRound(day.proteinDinner, 1) + ") carbs(" + myRound(day.carbDinner, 1) + ") fat(" + myRound(day.fatDinner, 1) + ")\n\n";

	out << "Total: \n\n";

	out << "calories[" + myRound(day.caloricBreakfast + day.caloricLunch + day.caloricDinner, 0) + "] protein(" + myRound(day.proteinBreakfast + day.proteinLunch + day.proteinDinner, 1) + ") carbs(" + myRound(day.carbBreakfast + day.carbLunch + day.carbDinner, 1) + ") fat(" + myRound(day.fatBreakfast + day.fatLunch + day.fatDinner, 1) + ")\n\n";

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

			lineCount++; //nbr ingredients stored in "Food Stats.txt"

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


					updateIngredientInMeal(ingredient, temp_meal, ingredient_newProportionStr, false);


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

					updateIngredientInMeal(ingredient, temp_meal, "", false);

				}

			}
			
			mealList[temp_meal.name] = temp_meal;

		}


	}

	myFile.close();

}

void Tracker::inputIngredient() {

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


	if (proportion == "null" || proportion == "n") {

		input = name + ": " + myRound(calories, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";
		
		allIngredients[name] = Ingredient(name, calories, protein, carbs, fat, "");

	}

	else {

		if (proportion[0] == 'x') {

			proportion = "x" + myRound(stod(proportion.substr(1)), 1);

		}

		else if (proportion.find("g") != -1) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("g"))), 1) + "g";

		}

		else {

			proportion = myRound(stod(proportion.substr(0, proportion.find("ml"))), 1) + "ml";

		}

		input = name + ": " + myRound(calories, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

		allIngredients[name] = Ingredient(name, calories, protein, carbs, fat, proportion);
	}

	text.clear();

	text = readFoodStats(text);

	list<string>::iterator it = text.begin();

	advance(it, lineCount * 2 + 3);

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

		updateIngredientInMeal(ing, m, proportion, true);

	}

	cout << "\n" << endl;

	mealList[name] = m;

	string input;

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

	while (!exit) {

		cout << "ingredient or meal? ";

		getline(cin, answer);

		cout << "\n" << endl;

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

			}

			else {

				if (proportion[0] == 'x') {

					if (allIngredients[name].proportion[0] == 'x') {

						double ratio = stod(proportion.substr(1)) / stod(allIngredients[name].proportion.substr(1));

						cal += (allIngredients[name].cal * ratio);
						protein += (allIngredients[name].protein * ratio);
						carbs += (allIngredients[name].carbs * ratio);
						fat += (allIngredients[name].fat * ratio);

						foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion) + " - ";

					}

					else {

						double ratio = stod(proportion.substr(1));

						cal += (allIngredients[name].cal * ratio);
						protein += (allIngredients[name].protein * ratio);
						carbs += (allIngredients[name].carbs * ratio);
						fat += (allIngredients[name].fat * ratio);

						if (allIngredients[name].proportion.find("g") != -1) {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, to_string(stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("g"))) * ratio)) + " - ";

						}

						else if (allIngredients[name].proportion.find("ml") != -1) {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, to_string(stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("ml"))) * ratio)) + " - ";

						}

						else {

							foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion) + " - ";

						}

					}
				}

				else if (proportion.find("g") != -1) {

					//if there's a mismatch in proportion mesures throw error 

					double ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("g")));

					cal += (allIngredients[name].cal * ratio);
					protein += (allIngredients[name].protein * ratio);
					carbs += (allIngredients[name].carbs * ratio);
					fat += (allIngredients[name].fat * ratio);

					foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion) + " - ";

				}

				else {

					double ratio = stod(proportion.substr(0, proportion.find("ml"))) / stod(allIngredients[name].proportion.substr(0, allIngredients[name].proportion.find("ml")));

					cal += (allIngredients[name].cal * ratio);
					protein += (allIngredients[name].protein * ratio);
					carbs += (allIngredients[name].carbs * ratio);
					fat += (allIngredients[name].fat * ratio);

					foods += displayFood(name, allIngredients[name].cal * ratio, allIngredients[name].protein * ratio, allIngredients[name].carbs * ratio, allIngredients[name].fat * ratio, proportion) + " - ";

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
					Ingredient ing = Ingredient();
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

							for (Ingredient i : m.ingredientList) {

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

							updateIngredientInMeal(ing, m, proportion, true);


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

					updateIngredientFromMeal(m, proportion, true);

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

		}

		if (answer == "input ingredient" || answer == "input i") {

			inputIngredient();

		}

		else if (answer == "input meal" || answer == "input m") {

			inputMeal();
		}

		if (answer == "exit" || answer == "e") {

			exit = true;
		}

	}


	cout << "Total: " << myRound(cal,0) << " cal (" << myRound(protein, 1) << "g/" << myRound(carbs, 1) << "g/" << myRound(fat, 1) << "g)\n\n\n";

	do{
		cout << "Where do you want these values to be saved at? ";

		cin >> answer;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n" << endl;

	} while (answer != "b" && answer != "breakfast" && answer != "l" && answer != "lunch" && answer != "d" && answer != "dinner" && answer != "e" && answer != "exit");
	


	if (answer == "b" || answer == "breakfast") {

		if (d.caloricBreakfast != 0) {

			string yesOrNo = mealAlreadyHasFood_Warning("Breakfast");

			to_lower(yesOrNo);

			if (yesOrNo == "n" || yesOrNo == "no") {

				cal = 0;
				protein = 0;
				carbs = 0;
				fat = 0;
				foods = "";

			}

		}

		d.caloricBreakfast += cal;

		d.proteinBreakfast += protein;
		d.carbBreakfast += carbs;
		d.fatBreakfast += fat;

		if (d.weight == 0) {

			cout << "What is your weight? ";

			cin >> d.weight;

			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "\n" << endl;

			d.date = getTime();
		}

		d.foodsBreakfast += foods;

	}

	else if (answer == "l" || answer == "lunch") {

		if (d.caloricLunch != 0) {

			string yesOrNo = mealAlreadyHasFood_Warning("Lunch");

			to_lower(yesOrNo);

			if (yesOrNo == "n" || yesOrNo == "no") {

				cal = 0;
				protein = 0;
				carbs = 0;
				fat = 0;
				foods = "";

			}

		}

		d.caloricLunch += cal;

		d.proteinLunch += protein;
		d.carbLunch += carbs;
		d.fatLunch += fat;

		d.foodsLunch += foods;

	}

	else if (answer == "d" || answer == "dinner") {

		if (d.caloricDinner != 0) {

			string yesOrNo = mealAlreadyHasFood_Warning("Dinner");

			to_lower(yesOrNo);

			if (yesOrNo == "n" || yesOrNo == "no") {

				cal = 0;
				protein = 0;
				carbs = 0;
				fat = 0;
				foods = "";

			}

		}

		d.caloricDinner += cal;

		d.proteinDinner += protein;
		d.carbDinner += carbs;
		d.fatDinner += fat;

		d.foodsDinner += foods;

		cout << "Do you want to log the day? ";

		cin >> answer;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n" << endl;

		if (answer == "y" || answer == "yes") {

			ifstream ifile("log.txt");

			ofstream ofile("log.txt", ios::app);

			double cal = 0;

			double protein = 0;
			double carbs = 0;
			double fat = 0;

			double weight = 0;

			d.foodsBreakfast = d.foodsBreakfast.substr(0, d.foodsBreakfast.size() - 3) + "]";

			d.foodsLunch = d.foodsLunch.substr(0, d.foodsLunch.size() - 3) + "]";

			d.foodsDinner = d.foodsDinner.substr(0, d.foodsDinner.size() - 3) + "]";

			string line;

			if (!ifile.is_open()) {

				cout << "Unable to open file";
			}

			while (getline(ifile, line)) {

				if (line.find("Week") != -1 && line.find("average") == -1) {

					l.currentWeek++;
					weight = 0;

				}

				if (line.find("average") != -1) {

					l.currentDay = 1;

					cal = 0;
					protein = 0;
					carbs = 0;
					fat = 0;

				}

				if (line.find("Day") != -1) {

					l.currentDay++;

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

			if (l.currentDay == 7) {

				l.appendEndofWeek = true;

			}

			else if (l.currentDay == 1) {

				l.appendNewWeek = true;
			}



			if (l.appendNewWeek) {


				l.to_log += "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\nWeek " + to_string(l.currentWeek) + "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n\n\n";

			}

			l.to_log += "Day " + to_string(l.currentDay) + " (" + d.date + "):\n------------------\n\n\n\tBreakfast: " + myRound(d.caloricBreakfast, 0) + " cal (" + myRound(d.proteinBreakfast, 1) + "g/" + myRound(d.carbBreakfast, 1) + "g/" + myRound(d.fatBreakfast, 1) + "g)\n\n" + d.foodsBreakfast + "\n\n\n\tLunch: " + myRound(d.caloricLunch, 0) + " cal (" + myRound(d.proteinLunch, 1) + "g/" + myRound(d.carbLunch, 1) + "g/" + myRound(d.fatLunch, 1) + "g)\n\n" + d.foodsLunch + "\n\n\n\tDinner: " + myRound(d.caloricDinner, 0) + " cal (" + myRound(d.proteinDinner, 1) + "g/" + myRound(d.carbDinner, 1) + "g/" + myRound(d.fatDinner, 1) + "g)\n\n" + d.foodsDinner + "\n\n\n\nTotal: " + myRound(d.caloricBreakfast + d.caloricLunch + d.caloricDinner, 0) + " cal (" + myRound(d.proteinBreakfast + d.proteinLunch + d.proteinDinner, 1) + "g/" + myRound(d.carbBreakfast + d.carbLunch + d.carbDinner, 1) + "g/" + myRound(d.fatBreakfast + d.fatLunch + d.fatDinner, 1) + "g)\n\nWeight: " + myRound(d.weight, 2) + " kg\n\n\n\n";


			if (l.appendEndofWeek) {

				cal += stod(myRound(d.caloricBreakfast + d.caloricDinner + d.caloricLunch, 0));

				double cal_avg = cal / 7;

				protein += stod(myRound(d.proteinBreakfast + d.proteinDinner + d.proteinLunch, 1));
				carbs += stod(myRound(d.carbBreakfast + d.carbDinner + d.carbLunch, 1));
				fat += stod(myRound(d.fatBreakfast + d.fatDinner + d.fatLunch, 1));

				double protein_avg = protein / 7;
				double carbs_avg = carbs / 7;
				double fat_avg = fat / 7;

				weight += stod(myRound(d.weight, 2));

				double weight_avg = weight / 7;

				l.to_log += "Week's average:\n\n\n\tCalories/Macros: " + myRound(cal_avg, 0) + " cal (" + myRound(protein_avg, 1) + "g/" + myRound(carbs_avg, 1) + "g/" + myRound(fat_avg, 0) + "g)\n\n\tWeight: " + myRound(weight_avg, 2) + " kg\n\n\n";

			}

			ofile << l.to_log;

			cout << d;

			resetDay();

			l.to_log = "";

			ifile.close();
			ofile.close();

		}

	}

	ofstream ofs("day_saved.txt");

	{

		text_oarchive oa(ofs);
		oa << d;

	}

	ofs.close();

}

void Tracker::debug() {

	for (auto& el : allIngredients){

		cout << el.second;

	}

	cout << "\n\n\n\n";

	for (auto& el : mealList) {

		cout << el.second;

	}

	cout << "\n\n\n\n";

	cout << d;

	cout << "\n\n\n\n";

}

void Tracker::reset() {

	ofstream file;

	file.open("day_saved.txt", ofstream::out | ofstream::trunc);

	file.close();

	resetDay();

}

void Tracker::resetDay() {

	d.caloricBreakfast = 0;
	d.caloricDinner = 0;
	d.caloricLunch = 0;

	d.carbBreakfast = 0;
	d.carbDinner = 0;
	d.carbLunch = 0;

	d.fatBreakfast = 0;
	d.fatDinner = 0;
	d.fatLunch = 0;

	d.proteinBreakfast = 0;
	d.proteinDinner = 0;
	d.proteinLunch = 0;

	d.weight = 0;

	d.foodsBreakfast = "[Foods ==> ";
	d.foodsLunch = "[Foods ==> ";
	d.foodsDinner = "[Foods ==> ";

}


string Tracker::mealAlreadyHasFood_Warning(string meal) {

	cout << "Warning! Your MEAL [" + meal + "] already has an input.\n\nDo you still wish to input these new values(y / n) : ";

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

string Tracker::displayFood(string name, double cal, double protein, double carbs, double fat, string proportion) {

	if (proportion == "null" || proportion == "n" || proportion == "") {

		return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g)";

	}

	return name + ": " + myRound(cal, 0) + " cal (" + myRound(protein, 1) + "g/" + myRound(carbs, 1) + "g/" + myRound(fat, 1) + "g) (" + proportion + ")";

}

void Tracker::updateIngredientInMeal(Ingredient &ing, Meal &m, string proportion, bool roundProportion) {

	if (proportion[0] == 'x') {

		if (roundProportion) {

			proportion = "x" + myRound(stod(proportion.substr(1)), 1);

		}

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

		if (roundProportion) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("g"))), 1) + "g";

		}

		double ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(ing.proportion.substr(0, ing.proportion.find("g")));

		ing.cal *= ratio;
		ing.protein *= ratio;
		ing.carbs *= ratio;
		ing.fat *= ratio;

		ing.proportion = proportion;

	}

	else if (proportion.find("ml") != -1) {

		//throw exception if proportion doesn't match with the allIngredients list

		if (roundProportion) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("ml"))), 1) + "ml";

		}

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

	m.ingredientList.push_back(ing);


}

void Tracker::updateIngredientFromMeal(Meal& m, string proportion, bool roundProportion) {

	if (proportion[0] == 'x') {

		if (roundProportion) {

			proportion = "x" + myRound(stod(proportion.substr(1)), 1);

		}

		if (m.proportion[0] == 'x') {

			double ratio = stod(proportion.substr(1)) / stod(m.proportion.substr(1));

			m.cal *= ratio;
			m.protein *= ratio;
			m.carbs *= ratio;
			m.fat *= ratio;

			m.proportion = proportion;

			for (Ingredient ing : m.ingredientList) {

				ing.cal *= ratio;
				ing.protein *= ratio;
				ing.carbs *= ratio;
				ing.fat *= ratio;

				if (ing.proportion[0] == 'x') {

					ing.proportion = "x" + myRound(stod(ing.proportion.substr(1)) * ratio, 1);

				}

				else if (ing.proportion.find("g") != -1) {

					ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio, 1) + "g";
				}

				else if (ing.proportion.find("ml") != -1) {

					ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio, 1) + "ml";
				}

				else {

					ing.proportion = "x" + myRound(ratio, 1);

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

			for (Ingredient ing : m.ingredientList) {

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

		if (roundProportion) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("g"))), 1) + "g";

		}

		double ratio = stod(proportion.substr(0, proportion.find("g"))) / stod(m.proportion.substr(0, m.proportion.find("g")));

		m.cal *= ratio;
		m.protein *= ratio;
		m.carbs *= ratio;
		m.fat *= ratio;

		m.proportion = proportion;

		for (Ingredient ing : m.ingredientList) {

			ing.cal *= ratio;
			ing.protein *= ratio;
			ing.carbs *= ratio;
			ing.fat *= ratio;

			if (ing.proportion[0] == 'x') {

				ing.proportion = "x" + myRound(stod(ing.proportion.substr(1)) * ratio, 1);

			}

			else if (ing.proportion.find("g") != -1) {

				ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio, 1) + "g";
			}

			else if (ing.proportion.find("ml") != -1) {

				ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio, 1) + "ml";
			}

			else {

				ing.proportion = "x" + myRound(ratio, 1);

			}

		}

	}

	else if (proportion.find("ml") != -1) {

		//throw exception if proportion doesn't match with the meal proportion

		if (roundProportion) {

			proportion = myRound(stod(proportion.substr(0, proportion.find("ml"))), 1) + "ml";

		}

		double ratio = stod(proportion.substr(0, proportion.find("ml"))) / stod(m.proportion.substr(0, m.proportion.find("ml")));

		m.proportion = proportion;

		m.cal *= ratio;
		m.protein *= ratio;
		m.carbs *= ratio;
		m.fat *= ratio;

		for (Ingredient ing : m.ingredientList) {

			ing.cal *= ratio;
			ing.protein *= ratio;
			ing.carbs *= ratio;
			ing.fat *= ratio;

			if (ing.proportion[0] == 'x') {

				ing.proportion = "x" + myRound(stod(ing.proportion.substr(1)) * ratio, 1);

			}

			else if (ing.proportion.find("g") != -1) {

				ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("g"))) * ratio, 1) + "g";
			}

			else if (ing.proportion.find("ml") != -1) {

				ing.proportion = myRound(stod(ing.proportion.substr(0, ing.proportion.find("ml"))) * ratio, 1) + "ml";
			}

			else {

				ing.proportion = "x" + myRound(ratio, 1);

			}

		}

	}


}