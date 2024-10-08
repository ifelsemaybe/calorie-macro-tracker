#include "API.h"

size_t write_cb(void* data, size_t size, size_t nmemb, string* userdata);
void setIngredient(Ingredient& i, nlohmann::json_abi_v3_11_3::json nutrients, bool& found);


bool API::getRequest(string ingrName, Ingredient& ingr) {

	string API_Key; string API_ID; string line;

	ifstream file;
	file.open("API_Key&ID.txt");

	while (getline(file, line)) {

		if (line.find("API_Key: ") != -1) {

			API_Key = line.substr(line.find("app_key"));

		}
		else {

			API_ID = line.substr(line.find("app_id"));

		}

	}

	file.close();

	string url = "https://api.edamam.com/api/food-database/v2/parser?" + API_ID + "&" + API_Key;

	string query = "ingr=Milliliter%20";


	CURL* curl = curl_easy_init();

	struct curl_slist* headers = NULL;

	query += ingrName;
	url += "&" + query;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());


	headers = curl_slist_append(headers, "Accept: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


	string response;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); /*<-- Uncomment for extra debug info*/


	CURLcode result;
	result = curl_easy_perform(curl);

	bool found = false;

	if (result != CURLE_OK) {

		fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
		return found;

	}


	nlohmann::json jsonData = nlohmann::json::parse(response);

	if (!jsonData["parsed"][0].empty()) {

		auto food = jsonData["parsed"][0]["food"];

		ingr.name = (string)food["label"];
		to_lower(ingr.name);

		auto nutrients = jsonData["parsed"][0]["food"]["nutrients"];

		setIngredient(ingr, nutrients, found);

		auto measure = jsonData["parsed"][0]["measure"];

		if ((string)measure["label"] == "Milliliter") {

			ingr.propValues[1] = 1 / (double)measure["weight"] * 100;

		}


	}

	else {

		for (auto& el : jsonData["hints"].items()) {

			string label = (string)el.value()["food"]["label"];
			to_lower(label);

			if (ingrName == label && (string)el.value()["food"]["category"] == "Generic meals") {

				auto nutrients = el.value()["food"]["nutrients"];
				setIngredient(ingr, nutrients, found);
				ingr.name = label;

				auto measures = el.value()["food"]["measures"];

				bool foundML = false;

				for (auto& x : measures.items()) {

					string propType = (string)x.value()["label"];

					if (propType == "Milliliter") {

						ingr.propValues[1] = 1 / stod((string)x.value()["weight"]) * 100;
						foundML = true;
						break;
					}

				}

				if (!foundML) {

					ingr.propValues[1] = -1;

				}

				if (found) {

					break;
				}

			}

		}

	}

	ingr.propValues[2] = -1;
	ingr.brand = "none";

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	return found;

}

void setIngredient(Ingredient& i, nlohmann::json_abi_v3_11_3::json nutrients, bool& found) {

	i.cal = nutrients["ENERC_KCAL"];
	i.protein = nutrients["PROCNT"];
	i.carbs = nutrients["CHOCDF"];
	i.fat = nutrients["FAT"];

	i.propValues[0] = 100;

	found = true;

}

size_t write_cb(void* data, size_t size, size_t nmemb, string* userdata) {

	userdata->append((char*)data, size * nmemb);

	return size * nmemb;

}
