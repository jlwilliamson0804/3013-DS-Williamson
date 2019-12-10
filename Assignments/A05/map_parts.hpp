#pragma once
#include <cmath>
#include "json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;

static double haversine(double lat1, double lon1, double lat2, double lon2) {
	// distance between latitudes 
	// and longitudes 
	double dLat = (lat2 - lat1) *
		M_PI / 180.0;
	double dLon = (lon2 - lon1) *
		M_PI / 180.0;

	// convert to radians 
	lat1 = (lat1)* M_PI / 180.0;
	lat2 = (lat2)* M_PI / 180.0;

	// apply formulae 
	double a = pow(sin(dLat / 2), 2) +
		pow(sin(dLon / 2), 2) *
		cos(lat1) * cos(lat2);
	double rad = 6371;
	double c = 2 * asin(sqrt(a));
	return rad * c;
}

struct City {

	string name;
	double growth;
	double lat;
	double lon;
	int population;
	int rank;
	string state;
	int id;

	City() {
		id = 0;
		name = "";
		growth = 0.0;
		lat = 0.0;
		lon = 0.0;
		population = 0;
		rank = 0;
		state = "";
	}

	City(json obj, int _id) {
		name = obj["city"];
		growth = obj["growth"];
		lat = obj["latitude"];
		lon = obj["longitude"];
		population = obj["population"];
		rank = obj["rank"];
		state = obj["state"];
		id = _id;
	}

	double Distance(double toLat, double toLon) {
		return haversine(lat, lon, toLat, toLon);
	}

	friend ostream& operator << (ostream& os, const City& c)
	{
		os << c.name << '/' << c.lat << '/' << c.lon;
		return os;
	}
};

struct Road {

	int type;
	double distance;

	Road() {
		type = 0;
		distance = 0.0;
	}
	Road(int t, double d) {
		type = t;
		distance = d;
	}
};