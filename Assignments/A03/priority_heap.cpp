/*
Name: Joseph Williamson
Date: Fall 2019
Course: 3013
Program: A03
Description:
	This program uses json.hpp along with a helper
	and reads in a data file "animals.json". It will also 
	calculate a priority value for each animal an place 
	them into the correct heap.
*/
#define _USE_MATH_DEFINES

#include <math.h>
#include <string>
#include <iostream>
#include "json.hpp"
#include <iomanip>
#include <fstream>

using namespace std;

const static double EarthRadiusKm = 6372.8;

using json = nlohmann::json;

/*
	Beginning of JsonHelper code block
*/
class JsonHelper {
private:
	json Data;
	int Size;
	int Index;
	string FileName;

public:
	JsonHelper(string filename) {
		FileName = filename;

		ifstream input(FileName);
		input >> Data;
		input.close();

		Size = Data.size();
		Index = 0;
	}

	json getNext() {
		if (Index < Size) {
			return Data[Index++];
		}
		return NULL;
	}

	json getIth(int i) {
		if (i < Size) {
			return Data[i];
		}
		return NULL;
	}

	/**
	 * pushBack: converts a string to json and pushes on end of array
	 *
	 * Params:
	 *     string obj  : a string json object which whould be in the following format:
	 *
	 *                           "{\"key0\":"value0",\"key1\":1,\"keyn\":\"valuen"\}"
	 * Returns:
	 *     void
	 *
	 */
	void pushBack(string obj) {

		json j = json::parse(obj);
		Data.push_back(j);
		cout << Data.dump(4) << endl;
	}

	int getSize() {
		return Size;
	}

	void saveFile(string filename = "") {
		if (filename == "") {
			filename = FileName;
		}

		ofstream out(filename);
		out << std::setw(4) << Data << endl;
		out.close();
	}
};

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}

class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}

/*
	end of JsonHelper
*/

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float adjuster;
	bool validated;
	string version;
	float priority;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		adjuster = 0.0;
		validated = 0;
		version = "";
		priority = 0.0;
	}

	Animal(string name, long _date, double lat, double lon, float adj, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		adjuster = adj;
		validated = val;
		version = ver;
		priority = 0.0;
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		adjuster = j["adjuster"];
		validated = j["validated"];
		version = j["version"];
		priority = 0.0;
	}
};

class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;
	int size;

public:
	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
		}
	}



	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->adjuster << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version << " "
				<< Animals[i]->priority << endl;

		}
	}
};

/*
	Heap class block
*/

class Heap {
private:
	int *H;       // Pointer to allocate dynamic array
	int Next;     // Next available location
	int MaxSize;  // Max size since were using array
	int HeapSize; // Actual number of items in the array.

	/**
	 * Function IncreaseKey:
	 *      Bubbles element up from given index.
	 *
	 * Params:
	 *     [int] index  - index of item to be increased
	 * Returns
	 *      void
	 */
	void BubbleUp(int i) {
		int p = Parent(i);
		while (p > 0 && H[i] > H[p]) {
			Swap(i, p);
			i = p;
			p = Parent(i);
		}
	}

	/**
	 * Function DecreaseKey:
	 *      Bubbles element down from given index.
	 *
	 * Params:
	 *      [int] index - index of item to be decreased
	 * Returns
	 *      void
	 */
	void BubbleDown(int i) {
		int c = PickChild(i);

		while (c > 0) {
			if (H[i] < H[c]) {
				Swap(i, c);
				i = c;
				c = PickChild(i);
			}
			else {
				c = -1;
			}
		}
	}

	/**
	 * Function Swap:
	 *      Swaps two elements in an array
	 *
	 * Params:
	 *      [int] index1 - index of item to swap with
	 *      [int] index2 - same
	 * Returns
	 *      void
	 */
	void Swap(int p, int i) {
		int temp = H[p];
		H[p] = H[i];
		H[i] = temp;
	}

	/**
	 * Function Parent:
	 *      Returns parent index of a given index
	 *
	 * Params:
	 *      [int]index - index to calculate parent from
	 * Returns
	 *      index [int]
	 */
	int Parent(int i) {
		return int(i / 2);
	}

	/**
	 * Function LeftChild:
	 *      Returns left index of a given index
	 *
	 * Params:
	 *      [int] index - index to calculate child from
	 * Returns
	 *      [int] index - left child index
	 */
	int LeftChild(int i) {
		return i * 2;
	}

	/**
	 * Function RightChild:
	 *      Returns right index of a given index
	 *
	 * Params:
	 *      [int] index - index to calculate child from
	 * Returns
	 *      [int] index - right child index
	 */
	int RightChild(int i) {
		return i * 2 + 1;
	}

	/**
	 * Function PickChild:
	 *      Return index of child to swap with or -1 to not swap.
	 *
	 * Params:
	 *      [int] index - index of parent element
	 * Returns
	 *      [int] index - index to swap with or -1 to not swap
	 */
	int PickChild(int i) {
		if (RightChild(i) >= Next) {    //No right child
			if (LeftChild(i) >= Next) { //No left child
				return -1;
			}
			else { //you have a left no right
				return LeftChild(i);
			}
		}
		else {
			//right child exists
			if (H[RightChild(i)] > H[LeftChild(i)]) {
				return RightChild(i);
			}
			else {
				return LeftChild(i);
			}
		}
	}

public:
	/**
	 * Function Heap:
	 *      Constructor that allocates memory for array and
	 *      inits vars.
	 *
	 * Params:
	 *      void
	 * Returns
	 *      void
	 */
	Heap(int size) {
		H = new int[size];
		Next = 1;
		MaxSize = size;
		HeapSize = 0;
	}

	/**
	 * Function Insert:
	 *      Insert value into heap.
	 *
	 * Params:
	 *      [int] x - value to be inserted
	 * Returns
	 *      void
	 */
	void Insert(int x) {
		H[Next] = x;
		BubbleUp(Next);
		Next++;
		HeapSize++;
	}

	/**
	 * Function Extract:
	 *      Removes top element from heap (whether min or max).
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [int] top_value - top value in the heap (min or max)
	 */
	int Extract() {

		if (Empty()) {
			return -1;
		}

		int retval = H[1];
		H[1] = H[--Next];
		HeapSize--;

		if (HeapSize > 1) {
			BubbleDown(1);
		}

		return retval;
	}

	/**
	 * Function PrintHeap:
	 *      Prints the values currently in the heap array
	 *      based on array location, not heap order
	 *
	 * Params:
	 *      void
	 * Returns
	 *      void
	 */
	void PrintHeap() {
		for (int i = 1; i < Next; i++) {
			cout << H[i] << " ";
		}
		cout << endl;
	}

	/**
	 * Function Size:
	 *      Returns the number of items in the heap
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [int] heapSize - size of heap
	 */
	int Size() {
		return Next - 1;
	}

	/**
	 * Function Empty:
	 *      Returns boolean true of array is empty
	 *
	 * Params:
	 *      void
	 * Returns
	 *      [bool] empty - is array empty
	 */
	bool Empty() {
		return Next == 1;
	}

	/**
	 * Function Heapify:
	 *      Creates a heap out of a given array of nums in no specific order.
	 *
	 * Params:
	 *      [int*] array - array of values to heapify
	 *      [int] size - size of array
	 * Returns
	 *      void
	 */
	void Heapify(int *A, int size) {
		int i = size / 2;
		// H = A;
		// Next = size;
		// HeapSize = size - 1;

		for (int j = i; j >= 1; j--) {
			BubbleDown(j);
		}
	}
};

/*
	End of Heap class block
*/

int main(int argc, char **argv) {

	AnimalHelper AH("animals.json");
	char ch;

	AH.PrintAnimals();
	system("pause");

	
	

	return 0;
}