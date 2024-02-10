#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Ingredient.h"

#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "boost/algorithm/string.hpp"

#undef max;

using namespace std;
using namespace boost;

static class API {

public:

	static bool getRequest(string ingrName, Ingredient& ingr);

};