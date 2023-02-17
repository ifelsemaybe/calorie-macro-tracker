#include "Calorie_Macro_Tracker.h"

int main() {

	Tracker t = Tracker();

	t.readStats();

	string answer;

	bool exit = false;

	while (!exit) {

		cout << "What do you want to do? ";

		getline(cin, answer);

		cout << "\n" << endl;

		if (answer == "input ingredient") {

			t.inputIngredient();
		}

		else if (answer == "input meal") {

			t.inputMeal();
		}

		else if (answer == "debug") {

			t.debug();
		}

		else if (answer == "track") {

			t.track();

		}

		if (answer == "exit" || answer == "e") {

			exit = true;
		}

	}

	return 0;
}