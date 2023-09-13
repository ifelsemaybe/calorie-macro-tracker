#include "Calorie_Macro_Tracker.h"

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

	while (!exit) {

		cout << "What do you want to do? ";

		getline(cin, answer);

		cout << "\n" << endl;

		if (answer == "input ingredient" || answer == "input i") {

			t.inputIngredient();
		}

		else if (answer == "input meal" || answer == "input m") {

			t.inputMeal();
		}

		else if (answer == "debug") {

			t.debug();
		}

		else if (answer == "track") {

			t.track();

		}

		else if (answer == "reset") {

			t.reset();
		}

		if (answer == "exit" || answer == "e") {

			exit = true;
		}

	}

	return 0;
}