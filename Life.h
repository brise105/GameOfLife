//Brandon Briseno|SID: 1932124|Assignment2: Game of Life|CPSC350-01(German)

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib> //cstandard library
#include <math.h>

const int ALIVE = 1; //X
const int DEAD = 0; //-

class Life {

public:
	Life(); //constructor
	~Life(); //destructor

	void welcomeFunction(); //ask user to pick game, input, and output mode
	void getArraySizes(); //read contents from file or get dimentions/density from user
	void setArrays(); //create arrays 

	void fillArrayOG(int **arr); //fill visible/original array 
	void fillArrayBoundary(int **arr); //fill invisible/boundary array (gameMode logic)

	void copyFunction(int **arr1, int **arr2); //copy items from one array to another

	void countAliveOnes(int **arr); //keeps track of alive spaces
	bool unstableGameBool(int **arr1, int **arr2); //compares current and next array

	void outStyled(); //method for writing to console/.out file
	void coutOGArray(int **arr); //print og array to console
	void coutCurrentArray(int **arr); //print og array to console
	//void coutCurrentArrayBoundary(int **arr); //print entire array to console
	void outToFile(int **arr); //print entire array to file.out

	void prepGame(); //master mutator method
	void automateLife(); //generation advancer
private:
	char gMode; //game mode (c-lassic, m-irror, d-nut)

	bool readingIn; //true = reading from from a file?
	std::string inFileName; //input file strings
	std::ifstream inFileStream; //file in stream
	float density; //rng 

	char oMode; //output mode (file)
	std::string outFileName; //output file strings
	std::ofstream outFileStream; //file out stream

	int **gameTable; //the entire game table (og+boundary)
	bool nullGameTable; //true = og array doesn't exist yet
	int ogArrRowSize; //gameTable row size 
	int ogArrColumnSize; //gameTable column size 

	int arrRowSize; //gameTable row size 
	int arrColumnSize; //x size of gameTableSize

	int **currentArray; //array seacher
	int **nextArray; //array queuer

	int state;

	int genCount; //count number of automations
	int aliveOnes; //count number of Xs
};