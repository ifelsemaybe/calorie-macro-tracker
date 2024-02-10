#include "Ingredient.h";
#include "Utility.h";

Ingredient::Ingredient() {
	
	cal = 0;
	carbs = 0;
	protein = 0;
	fat = 0;
	id = -1;

	brand = "none";

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";

	this->propValues[0] = -1;
	this->propValues[1] = -1;
	this->propValues[2] = -1;
	this->propValues[3] = -1;

}

Ingredient::Ingredient(string name, double cal, double protein, double carbs, double fat, string brand, int id, double propValues[]) {

	this->name = name;
	this->cal = cal;
	this->protein = protein;
	this->carbs = carbs;
	this->fat = fat;
	this->brand = brand;
	this->id = id;
	this->propValues[0] = propValues[0];
	this->propValues[1] = propValues[1];
	this->propValues[2] = propValues[2];
	this->propValues[3] = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";
}

Ingredient::Ingredient(const Ingredient& copy) {

	cal = copy.cal;
	carbs = copy.carbs;
	protein = copy.protein;
	fat = copy.fat;
	name = copy.name;
	brand = copy.brand;
	id = copy.id;
	this->propValues[0] = copy.propValues[0];
	this->propValues[1] = copy.propValues[1];
	this->propValues[2] = copy.propValues[2];
	this->propValues[3] = -1;

	this->propTypes[0] = "g";
	this->propTypes[1] = "mL";
	this->propTypes[2] = "x";

}

ostream& operator << (ostream& out, const Ingredient& ing) {

	int index = -1;

	if (ing.propValues[3] == -1) {


		for (int x = 0; x < 3; x++) {

			if (ing.propValues[x] != -1) {

				index = x;
				break;

			}

		}
	}

	else {

		index = ing.propValues[3];

	}

	string proportion;

	if (index == 2) {

		proportion = "x" + to_string(ing.propValues[2]);

	}

	else {

		proportion = (index == -1) ? "" : to_string(ing.propValues[index]) + ing.propTypes[index];

	}


	out << displayFood(ing.name, ing.cal, ing.protein, ing.carbs, ing.fat, proportion);

	return out;

}