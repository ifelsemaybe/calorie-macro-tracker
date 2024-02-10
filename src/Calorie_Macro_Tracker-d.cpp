#include "Calorie_Macro_Tracker.h"



bool guard(string command) {

	cout << "Are you sure you want to " + command + " (y/n)? ";

	string answer;

	getline(cin, answer);

	cout << "\n\n";

	to_lower(answer);

	if (answer == "n" || answer == "no") {

		return false;
	}

	return true;

}

string nameOfMealTime(vector<string>& v) {

	string toEvaluate;

	for (int x = 1; x < v.size(); x++) {

		toEvaluate += v[x];
	}

	return toEvaluate;

}

int main() {

	Tracker t = Tracker();

	t.readStats();

	string answer;

	bool exit = false;

	ifstream file = ifstream("day_saved.txt");

	if (file.peek() != ifstream::traits_type::eof()) {

		{

			text_iarchive ia(file);

			ia >> t.d;

		}

	}

	file.close();

	if (t.d.checkIfDayIsNotSetUp()) { //might be able to combo off last if statement instead of check method! 

		t.d.dayInit();

	}

	while (!exit) {

		cout << "What do you want to do? ";

		getline(cin, answer);

		cout << "\n" << endl;

		vector<string> answerSplit;

		split(answerSplit, answer, is_any_of(" "));

		if (answer == "input ingredient" || answer == "input i") {

			t.inputIngredient();
		}

		else if (answer == "input meal" || answer == "input m") {

			t.inputMeal();
		}

		else if (answer == "debug") {

			t.debug();

		}

		else if (answer == "log") {

			if (guard(answer)) {

				t.log();
			}

		}

		else if (answer == "track") {

			t.track();

		}

		else if (answer == "reset") {
	
			if (guard(answer)) {

				t.reset();
			}
		
		}

		if (answer == "exit" || answer == "e") {

			exit = true;
		}

		if (answerSplit.size() > 1) {

			if (answerSplit[0] == "add" || answerSplit[0] == "a") {

				t.d.add(nameOfMealTime(answerSplit));

			}

			else if (answerSplit[0] == "remove" || answerSplit[0] == "r") {

				string toEvaluate = nameOfMealTime(answerSplit);

				mealTime selected;

				bool found = false;

				for (mealTime mT : t.d.listOfMealTimes) {

					if (mT.name == toEvaluate) {

						selected = mT;

						found = true;
						break;
					}
				}

				if (found) {

					t.d.remove(selected);

				}

				else {

					cout << "Sorry, the Meal Time you want to remove has not been found! Try again...\n\n";
				}

			}

			else if (answerSplit[0] == "switch" || answerSplit[0] == "sw") {

				string toEvaluate1 = nameOfMealTime(answerSplit);

				string toEvaluate2 = toEvaluate1.substr(toEvaluate1.find(',') + 1);

				toEvaluate1.erase(toEvaluate1.begin() + toEvaluate1.find(','), toEvaluate1.end());

				bool found1 = false;
				bool found2 = false;

				mealTime selected1;
				mealTime selected2;

				for (mealTime mT : t.d.listOfMealTimes) {

					if (mT.name == toEvaluate1) {

						selected1 = mT;

						found1 = true;
						continue;
					}

					else if (mT.name == toEvaluate2) {

						selected2 = mT;

						found2 = true;
						continue;

					}

					if (found1 && found2) {

						break;
					}
				}

				if (found1 && found2) {

					t.d.switchOrder(selected1, selected2);

				}

				else {

					cout << "Sorry, the Meal Time(s) you want to switch has/have not been found! Try again...\n\n";
				}

			}

		}

	}

	return 0;
}