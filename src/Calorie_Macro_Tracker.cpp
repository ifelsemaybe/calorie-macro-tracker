#include "Calorie_Macro_Tracker.h"
#include "Utility.h"
#include "API.h"


void Tracker::readStats() {

	sqlite3* db = nullptr;
	int rc = 0;
	const char* sql = 0;
	char* zErrMsg = 0;

	rc = sqlite3_open("Food Stats.db", &db);

	initializeSqlite3(db, sql, rc, zErrMsg);

	//INGREDIENTS

	sql = "SELECT i.name, i.brand, i.id, i.calorie, i.protein, i.carbohydrate, i.fat, ipw.g, ipv.mL, ipt.x FROM ((Ingredients i LEFT OUTER JOIN Ingredient_Proportion_Weight ipw ON i.id = ipw.ing_id) LEFT OUTER JOIN Ingredient_Proportion_Volumetric ipv ON i.id = ipv.ing_id) LEFT OUTER JOIN Ingredient_Proportion_Tally ipt ON i.id = ipt.ing_id;";

	rc = sqlite3_exec(db, sql, Icallback, this, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Ingredients Insert", zErrMsg);

	//MEALS + their respective ingredients list

	sql = "SELECT m.name, m.variation, m.variation_description, m.id, m.calorie, m.protein, m.carbohydrate, m.fat, mpw.g, mpv.mL, mpt.x, im.ing_id, im.value, im.proportion FROM (((Meals m LEFT OUTER JOIN Meal_Proportion_Weight mpw ON m.id = mpw.meal_id) LEFT OUTER JOIN Meal_Proportion_Volumetric mpv ON m.id = mpv.meal_id) LEFT OUTER JOIN Meal_Proportion_Tally mpt ON m.id = mpt.meal_id) LEFT OUTER JOIN Ingredient_Makeup im ON m.id = im.meal_id;";

	rc = sqlite3_exec(db, sql, Mcallback_ing, this, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Meals + Respective Ingredient's List Insert", zErrMsg);

	//TO DO - IMPLEMENT MEALS inside of MEALS a.k.a meals list

	//sql = "SELECT m.name, m.variation, m.variation_description, m.id, m.calorie, m.protein, m.carbohydrate, m.fat, mpw.g, mpv.mL, mpt.x, mm.meal_as_ing_id, mm.value, mm.proportion FROM (((Meals m LEFT OUTER JOIN Meal_Proportion_Weight mpw ON m.id = mpw.meal_id) LEFT OUTER JOIN Meal_Proportion_Volumetric mpv ON m.id = mpv.meal_id) LEFT OUTER JOIN Meal_Proportion_Tally mpt ON m.id = mpt.meal_id) LEFT OUTER JOIN Meal_Makeup mm ON m.id = mm.meal_id;";

	//rc = sqlite3_exec(db, sql, Mcallback_, this, &zErrMsg);

	//if (rc != 0) {

	//	cout << "Error: " << zErrMsg;
	//	sqlite3_free(zErrMsg);

	//	sqlite3_close(db);
	//	return;

	//}

	sqlite3_close(db);

}

void Tracker::inputIngredient() {

	sqlite3* db = nullptr;
	int rc = -1;
	const char* sql = 0;
	char* zErrMsg = 0;

	rc = sqlite3_open("Food Stats.db", &db);

	initializeSqlite3(db, sql, rc, zErrMsg);

	string answer;

	cout << "Input manually or API? ";

	getline(cin, answer);

	cout << endl;

	bool api = false;
	bool manual = false;

	to_lower(answer);

	answer == "api" ? api = true : manual = true;

	string name;

	cout << "name? ";

	getline(cin, name);

	cout << endl;

	to_lower(name);

	string nameAPI = name;

	while (nameAPI.find(" ") != -1) {

		nameAPI = nameAPI.substr(0, nameAPI.find(" ")) + "%20" + nameAPI.substr(nameAPI.find(" ") + 1);

	}


	Ingredient i = Ingredient();

	if (api) { //API returns true if ingredient was found!

		if (!API::getRequest(nameAPI, i)) {

			cout << "Sorry! Ingredient " << name << " was not found through API... please input values manually.\n\n";
			manual = true;


		}

	}

	if (manual){

		double calories;

		double protein, carbs, fat;

		string proportionWeight, proportionVolume, proportionTally, brand;

		checkForBrand(name, brand);

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

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		string type;
		double value = 0;

		cout << "proportion(weight)? ";

		getline(cin, proportionWeight);

		cout << endl;

		type = proportionWeight.substr(proportionWeight.find_first_not_of(ALLDECIMALS));
		value = stod(proportionWeight);

		if (type != "g" && type != "kg" && type != "oz" && type != "lb") {

			if (type == "null" || type == "n" || type == "") {

				i.propValues[0] = -1;

			}

			else {

				cout << "The type for weight was not one of the main types :(";

				abort();
			}


		}

		else {

			i.propValues[0] = value;
			i.propTypes[0] = type;

		}


		cout << "proportion(volume)? ";

		getline(cin, proportionVolume);

		cout << endl;

		type = proportionVolume.substr(proportionVolume.find_first_not_of(ALLDECIMALS));
		value = stod(proportionVolume);

		if (type != "mL" && type != "L" && type != "tsp" && type != "tbsp" && type != "cup" && type != "fl oz") {

			if (type == "null" || type == "n" || type == "") {

				i.propValues[1] = -1;

			}

			else {

				cout << "The type for volume was not one of the main types :(";

				abort();

			}


		}

		else {

			i.propValues[1] = value;
			i.propTypes[1] = type;

		}

		cout << "proportion(tally)? ";

		getline(cin, proportionTally);

		cout << endl;

		type = proportionTally.substr(0,1);
		value = stod(proportionTally.substr(1));

		if (type != "x") {

			if (type == "null" || type == "n" || type == "") {

				i.propValues[2] = -1;

			}

			else {

				cout << "The type for tally was not one of the main types :(";

				abort();

			}


		}

		else {

			i.propValues[2] = value;

		}

		i.name = name;
		i.brand = brand;

		i.cal = calories;
		i.protein = protein;
		i.carbs = carbs;
		i.fat = fat;



	}


	string str_sql = "INSERT INTO Ingredients(name, brand, calorie, protein, carbohydrate, fat) Values(\'" + i.name + "\', " + "\'" + i.brand + "\', " + to_string(i.cal) + ", " + to_string(i.protein) + ", " + to_string(i.carbs) + ", " + to_string(i.fat) + ");";
	sql = str_sql.c_str();

	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Ingredient Input", zErrMsg);


	str_sql = "SELECT i.id FROM Ingredients i WHERE i.name = \'" + i.name + "\' AND i.brand = \'" + i.brand + "\';";
	sql = str_sql.c_str();

	rc = sqlite3_exec(db, sql, callback_ingID, &i, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Ingredient ID request", zErrMsg);



	if (i.propValues[0] != -1) {

		if (i.propTypes[0] != "g") toG(i.propValues[0], i.propTypes[0]);

		double* allWeights = findAllWeights(i.propValues[0]);

		double g = allWeights[0];
		double kg = allWeights[1];
		double lb = allWeights[2];
		double oz = allWeights[3];

		str_sql = "INSERT INTO Ingredient_Proportion_Weight(g, kg, lb, oz, ing_id) Values(" + to_string(g) + ", " + to_string(kg) + ", " + to_string(lb) + ", " + to_string(oz) + ", " + to_string(i.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Ingredient Input Weights", zErrMsg);

	}

	if (i.propValues[1] != -1) {

		if (i.propTypes[1] != "mL") toML(i.propValues[1], i.propTypes[1]);

		double* allVolumes = findAllVolumes(i.propValues[1]);

		double mL = allVolumes[0];
		double L = allVolumes[1];
		double tsp = allVolumes[2];
		double tbsp = allVolumes[3];
		double cup = allVolumes[4];
		double fl_oz = allVolumes[5];

		str_sql = "INSERT INTO Ingredient_Proportion_Volumetric(mL, L, tsp, tbsp, cup, fl_oz, ing_id) Values(" + to_string(mL) + ", " + to_string(L) + ", " + to_string(tsp) + ", " + to_string(tbsp) + ", " + to_string(cup) + ", " + to_string(fl_oz) + ", " + to_string(i.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Ingredient Input Volumes", zErrMsg);

	}

	if (i.propValues[2] != -1) {

		str_sql = "INSERT INTO Ingredient_Proportion_Tally(x, ing_id) VALUES(" + to_string(i.propValues[2]) + ", " + to_string(i.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Ingredient Input Tally", zErrMsg);
	}

	allIngredients[i.id] = i;

	sqlite3_close(db);

}

void Tracker::inputMeal() {

	sqlite3* db = nullptr;
	int rc = -1;
	const char* sql = 0;
	char* zErrMsg = 0;

	rc = sqlite3_open("Food Stats.db", &db);

	initializeSqlite3(db, sql, rc, zErrMsg);

	string name, proportionWeight, proportionVolume, proportionTally;

	Meal m = Meal();

	string type;
	double value;

	cout << "Meal name? ";

	getline(cin, name);

	if (name.find(" -v ") != -1) {

		m.variation = stoi(name.substr(name.find(" -v ") + 4));
		m.name = name.substr(0, name.find(" -v "));

		cout << "Variation Description? ";

		getline(cin, m.varDescription);

		cout << endl;
	}

	else {

		m.name = name;
		m.variation = 0;
		m.varDescription = "default";

	}


	cout << endl;

	cout << "proportion(weight)? ";

	getline(cin, proportionWeight);

	cout << endl;

	type = proportionWeight.substr(proportionWeight.find_first_not_of(ALLDECIMALS));
	value = stod(proportionWeight);

	if (type != "g" && type != "kg" && type != "oz" && type != "lb") {

		if (type == "null" || type == "n" || type == "") {

			m.propValues[0] = -1;

		}

		else {

			cout << "The type for weight was not one of the main types :(";

			abort();

		}


	}

	else {

		m.propValues[0] = value;
		m.propTypes[0] = type;

	}


	cout << "proportion(volume)? ";

	getline(cin, proportionVolume);

	cout << endl;

	type = proportionVolume.substr(proportionVolume.find_first_not_of(ALLDECIMALS));
	value = stod(proportionVolume);

	if (type != "mL" && type != "L" && type != "tsp" && type != "tbsp" && type != "cup" && type != "fl oz") {

		if (type == "null" || type == "n" || type == "") {

			m.propValues[1] = -1;

		}

		else {

			cout << "The type for volume was not one of the main types :(";

			abort();

		}


	}

	else {

		m.propValues[1] = value;
		m.propTypes[1] = type;

	}

	cout << "proportion(tally)? ";

	getline(cin, proportionTally);

	cout << endl;

	type = proportionTally.substr(0, 1);
	value = stod(proportionTally.substr(1));

	if (type != "x") {

		if (type == "null" || type == "n" || type == "") {

			m.propValues[2] = -1;

		}

		else {

			cout << "The type for tally was not one of the main types :(";

			abort();

		}


	}

	else {

		m.propValues[2] = value;

	}


	bool end = false;

	int itr = 1;

	string ing_name;
	string ing_brand;
	string ing_proportion;

	string proportionType;
	double proportionValue;
	int ing_id = -1;

	while (!end) {

		do{
			
			cout << "Ingredient " + to_string(itr) + " name? ";

			getline(cin, ing_name);

			if (ing_name == "input ingredient" || ing_name == "input i") {

				cout << "\n";

				inputIngredient();

				continue;

			}

			if (ing_name == "exit" || ing_name == "e") {

				end = true;

				break;

			}
			
			checkForBrand(ing_name, ing_brand);

			ing_id = checkIfIngredientExists(ing_name, ing_brand);

		} while (ing_id == -1);


		if (end == true) {

			continue;

		}

		cout << "Ingredient " + to_string(itr) + " proportion? ";

		getline(cin, ing_proportion);
		
		cout << endl;

		if (ing_proportion == "e" || ing_proportion == "exit") {

			end = true;
			continue;

		}		

		checkForProportionValidity(ing_proportion, proportionType, proportionValue);

		itr++;

		Ingredient ing = allIngredients[ing_id];

		clampToOneProportion(&ing, proportionType);

		updateIngredientInMeal(ing, m, proportionValue, proportionType);

		m.ingredientList.push_back(ing);

	}

	string str_sql;

	str_sql = "INSERT INTO Meals(name, variation, variation_description, calorie, protein, carbohydrate, fat) VALUES (\'" + m.name + "\', " + to_string(m.variation) + ", \'" + m.varDescription + "\', " + to_string(m.cal) + ", " + to_string(m.protein) + ", " + to_string(m.carbs) + ", " + to_string(m.fat)+ ");";
	sql = str_sql.c_str();

	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Input Meal", zErrMsg);

	str_sql = "SELECT id FROM Meals WHERE name = \'" + m.name + "\' AND variation = " + to_string(m.variation) + ";";
	sql = str_sql.c_str();

	rc = sqlite3_exec(db, sql, callback_mealID, &m, &zErrMsg);

	if (rc != 0) checkErrSqlite3("Fetch Meal ID", zErrMsg);

	
	if (m.propValues[0] != -1) {

		if (m.propTypes[0] != "g") toG(m.propValues[0], m.propTypes[0]);

		double* allWeights = findAllWeights(m.propValues[0]);

		double g = allWeights[0];
		double kg = allWeights[1];
		double lb = allWeights[2];
		double oz = allWeights[3];

		str_sql = "INSERT INTO Meal_Proportion_Weight(g, kg, lb, oz, meal_id) Values(" + to_string(g) + ", " + to_string(kg) + ", " + to_string(lb) + ", " + to_string(oz) + ", " + to_string(m.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Meal Input Weights", zErrMsg);

	}

	if (m.propValues[1] != -1) {

		if (m.propTypes[1] != "mL") toML(m.propValues[1], m.propTypes[1]);

		double* allVolumes = findAllWeights(m.propValues[1]);

		double mL = allVolumes[0];
		double L = allVolumes[1];
		double tsp = allVolumes[2];
		double tbsp = allVolumes[3];
		double cup = allVolumes[4];
		double fl_oz = allVolumes[5];

		str_sql = "INSERT INTO Meal_Proportion_Volumetric(mL, L, tsp, tbsp, cup, fl_oz, meal_id) Values(" + to_string(mL) + ", " + to_string(L) + ", " + to_string(tsp) + ", " + to_string(tbsp) + ", " + to_string(cup) + ", " + to_string(fl_oz) + ", " + to_string(m.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Meal Input Volumes", zErrMsg);

	}

	if (m.propValues[2] != -1) {

		str_sql = "INSERT INTO Meal_Proportion_Tally(x, meal_id) VALUES(" + to_string(m.propValues[2]) + ", " + to_string(m.id) + ");";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Meal Input Tally", zErrMsg);

	}

	int counter = 1;

	for (Ingredient ing : m.ingredientList) {

		int index = ing.propValues[3];

		str_sql = "INSERT INTO Ingredient_Makeup(meal_id, ing_id, value, proportion) VALUES (" + to_string(m.id) + ", " + to_string(ing.id) + ", " + to_string(ing.propValues[index]) + ", \'" + ing.propTypes[index] + "\');";
		sql = str_sql.c_str();

		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

		if (rc != 0) checkErrSqlite3("Meal [" + m.name +  "] Input Ingredient " + to_string(counter) + " [" + ing.name + "]", zErrMsg);

		counter++;

	}


}

void Tracker::track() {

	string answer, name, proportion, brand;
	
	int variation = 0;

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

			int id = -1;

			askForIngredient(id, name, brand);

			if (id == -1) {

				continue;

			}

			cout << "proportion? ";

			getline(cin, proportion);

			cout << "\n" << endl;

			if (name == "exit" || name == "e") {

				continue;

			}

			Ingredient ing = allIngredients[id];

			if (proportion == "null" || proportion == "n" || proportion == "") { 

				cal += ing.cal;

				protein += ing.protein;
				carbs += ing.carbs;
				fat += ing.fat;

				foods += displayFood(name, ing.cal, ing.protein, ing.carbs, ing.fat, proportion, true) + " - ";

			}

			else {

				double proportionValue = 0;
				string proportionType;

				checkForProportionValidity(proportion, proportionType, proportionValue);

				double ratio = findRatio(&ing, proportionType, proportionValue);

				cal += (ing.cal * ratio);

				protein += (ing.protein * ratio);
				carbs += (ing.carbs * ratio);
				fat += (ing.fat * ratio);

				foods += displayFood(name, ing.cal * ratio, ing.protein * ratio, ing.carbs * ratio, ing.fat * ratio, proportion, true) + " - ";

			}

			cout << "\n\n\n";

		}

		else if (answer == "meal" || answer == "m") {

			int id = -1;
			string proportion = "";

			do {

				cout << "name? ";

				getline(cin, name);

				cout << "\n" << endl;

				if (name == "exit" || name == "e") {

					break;

				}

				if (name.find(" -v ") != -1) {

					variation = stoi(name.substr(name.find(" -v ") + 4));
					name = name.substr(0, name.find(" -v "));

				}

				id = checkIfMealExists(name, variation);

			} while (id == -1);

			if (id == -1) {

				continue;
			}

			Meal m = allMeals[id];

			string answer;
			bool exit = false;

			while (!exit) {

				cout << "What do you want to do? ";

				getline(cin, answer);

				cout << "\n" << endl;

				if (answer == "Change Ingredient(s) Proportion" || answer == "cip") {

					bool exit = false;
					string name_, brand_;
					Ingredient* ing = nullptr;
					bool found = false;

					int ing_id = -1;

					while (!exit) {

						askForIngredient(ing_id, name_, brand_);

						if (ing_id == -1) {

							exit = true;

							continue;

						}

						else {

							for (Ingredient& i : m.ingredientList) {

								if (i.id == ing_id) {

									found = true;
									ing = &i;
									break;
								}

							}

							if (!found) {

								cout << "Sorry your ingredient is not a part of the meal's ingredient list! Try again!\n\n";
								continue;

							}

							string proportion_;

							cout << "proportion? ";

							getline(cin, proportion_);

							cout << "\n" << endl;


							if (proportion_ == "exit" || proportion_ == "e") {

								exit = true;
								continue;

							}

							string proportionType;
							double proportionValue = 0;

							checkForProportionValidity(proportion_, proportionType, proportionValue);

							clampToOneProportion(ing, proportionType);

							m.cal -= ing->cal;

							m.protein -= ing->protein;
							m.carbs -= ing->carbs;
							m.fat -= ing->fat;


							updateIngredientInMeal(*ing, m, proportionValue, proportionType);


						}

					}

				}

				else if (answer == "Change Meal Proportion" || answer == "cmp") {

					string proportionType;
					double proportionValue = 0;

					cout << "What is the new proportion for the meal? ";

					getline(cin, proportion);

					cout << "\n" << endl;

					checkForProportionValidity(proportion, proportionType, proportionValue);

					clampToOneProportion(&m, proportionType);

					updateIngredientFromMeal(m, proportionValue, proportionType);

				}

				else if (answer == "exit" || "e") {

					exit = true;
					continue;

				}


			}



			cal += m.cal;

			protein += m.protein;
			carbs += m.carbs;
			fat += m.fat;

			foods += displayFood(m.name, m.cal, m.protein, m.carbs, m.fat, proportion, true) + " - ";

			cout <<  "\n\n\n";

		}

		if (answer == "input ingredient" || answer == "input i") {

			inputIngredient();

		}

		else if (answer == "input meal" || answer == "input m") {

			inputMeal();
		}

		else if (answer == "exit" || answer == "e") {

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

						selected = d.listOfMealTimes[mT.order - 1];
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

	for (auto& el : allIngredients) {

		cout << el.second << "\n\n";

	}

	cout << "\n\n\n\n";

	for (auto& el : allMeals) {

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

int Tracker::checkIfIngredientExists(string name, string brand) {

	for (auto& it : allIngredients) {

		if (it.second.name == name && it.second.brand == brand) {

			return it.second.id;
		}

	}


	cout << "Sorry, the ingredient |" + name + " [" + brand  + "] | cannot be found, try again!";

	cout << "\n" << endl;

	return -1;

}


int Tracker::checkIfMealExists(string name, int variation) {


	for (auto& it : allMeals) {

		if (it.second.name == name && it.second.variation == variation) {

			return it.second.id;
		}

	}

	cout << "Sorry, the meal |" + name + " |" + to_string(variation) + "| | cannot be found, try again!";

	cout << "\n" << endl;

	return -1;

}

void Tracker::askForIngredient(int& id, string& name, string& brand) {

	do {

		cout << "name? ";

		getline(cin, name);

		cout << "\n" << endl;

		if (name == "exit" || name == "e") {


			break;

		}

		checkForBrand(name, brand);

		id = checkIfIngredientExists(name, brand);

	} while (id == -1);

}

void Tracker::updateIngredientInMeal(Ingredient &ing, Meal &m, double proportionValue, string proportionType, bool countMeal) {

	double ratio = findRatio(&ing, proportionType, proportionValue);

	ing.cal *= ratio;
	ing.protein *= ratio;
	ing.carbs *= ratio;
	ing.fat *= ratio;

	checkIndex(ing.propValues[3], ing, ratio);

	if (countMeal) {

		m.cal += ing.cal;
		m.protein += ing.protein;
		m.carbs += ing.carbs;
		m.fat += ing.fat;

	}

}

void Tracker::updateIngredientFromMeal(Meal& m, double proportionValue, string proportionType) {

	double ratio = findRatio(&m, proportionType, proportionValue);

	m.cal *= ratio;
	m.protein *= ratio;
	m.carbs *= ratio;
	m.fat *= ratio;

	checkIndex(m.propValues[3], m, ratio);

	for (Ingredient& ing : m.ingredientList) {

		checkIndex(ing.propValues[3], ing, ratio);

		ing.cal *= ratio;
		ing.protein *= ratio;
		ing.carbs *= ratio;
		ing.fat *= ratio;

	}


}