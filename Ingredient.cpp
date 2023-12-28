#include "Ingredient.h";

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

	out << displayFood(ing.name, ing.cal, ing.protein, ing.carbs, ing.fat, ing.proportion);

	return out;

}