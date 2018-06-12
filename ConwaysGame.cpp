//Brandon Briseno|SID: 1932124|Assignment2: Game of Life|CPSC350-01(German)
//Program description: this program runs the automation invented by
//					   John Conway called the Game of Life. the Game 
//					   follows 3-4 simple rules, but it is able to
//					   produce complex patterns/shapes. This code
//                     approaches the game by creating 3 arrays.
//					   the first array is copied to the second.
//					   then the second and third arrays are copied,
//					   updated, then printed to a console/file.
// Issues: in the case of an infinite automation,
//		   this program will infinitely write to a file. 
//         i couldn't figure out how to robustly detect for infinite configurations
//         as this issue is refered to as the halting problem.
//         just press Ctrl+C to terminate the program.
//         Also theres an issue when reading files.
//         I couldn't figure out how to properly index
//         lines (as an array of chars) from a file.
//         So invalid files will be parsed to ints.
#include "Life.h" //defintition

using namespace std;

Life::Life() //constructor
{
	density = 0;
	aliveOnes = 0; //number of Xs
}
Life::~Life() //destructor for file stream and arrays
{
	if (inFileStream.is_open()) //close istream
	{
		inFileStream.close();
	}
	if (outFileStream.is_open()) //close ostream
	{
		outFileStream.close();
	}

	for (int i = 0; i < ogArrRowSize; ++i) //delete elements at i for each arrray
	{
		delete [] gameTable[i]; 
		delete [] currentArray[i];
		delete [] nextArray[i];
	
	}

	delete [] gameTable; //delete original array
	delete [] currentArray; //delete current array
	delete [] nextArray; //delete next array
}

void Life::welcomeFunction()
{
	cout << "Welcome to Conway's Game of Life!!" << endl;
	cout << "RULES:" << endl;
	cout << "1--Any live cell with fewer than two live neighbours dies." << endl;
	cout << "2--Any live cell with more than three live neighbours dies." << endl;
	cout << "3--Any live cell with two or three live neighbours lives on to the next generation." << endl;
	cout << "4--Any dead cell with exactly three live neighbours will come to life in the next generation. " << endl;
	bool valid = false;
	while (valid != true) //validate input
	{
		string readOrRNG;

		cout << "Will you be reading game data from a file or would you like to randomly generate some? (ex: 'r' or 'RNG')" << endl;
		cin >> readOrRNG;

		if ((readOrRNG == "r")||(readOrRNG == "R"))
		{
			readingIn = true; 
			cout << "Enter the fileName.txt: "; 
			cin >> inFileName;

			inFileStream.open(inFileName.c_str());
			if (!inFileStream.is_open())
			{
				readingIn == false;
				cout << "Incorrect file name! Closing program." << endl;
				cout << endl;
				inFileStream.close();			
				exit(0);
			}
			setArrays();
			valid == true;
			break;
		}
		if ((readOrRNG == "rng")||(readOrRNG == "RNG"))
		{
			setArrays();
			valid == true; 
			break;
		}
		else
		{
			cout << endl;
			cout << "Oops, something went wrong. Try again." << endl;
			cout << endl;
		}
	}
	bool valid2 = false;
	while (valid2 != true) //validate gamemode
	{
		cout << "What game mode would you like to play in: classic, mirror, or doughnut? (ex: 'c', 'm', 'd')" << endl;
		cin >> gMode;

		if ((gMode == 'c')||(gMode == 'c')||(gMode == 'm')||(gMode == 'm')||(gMode == 'd')||(gMode == 'D'))
		{
			valid2 == true; 
			break;
		}
		else
		{
			cout << endl;
			cout << "Oops, something went wrong. Try again." << endl;
			cout << endl;			
			cin.clear();
			cin.ignore(999, '\n');
		}	

	}
	bool valid3 = false;
	while (valid3 != true) //validate output style
	{
		cout << "Choose your output style:" << endl;
		cout << "(b) buffer output to console" << endl;
		cout << "(w) wait for user key press (Enter)" << endl;
		cout << "(o) print to example.out" << endl;
		cin >> oMode;

		if ((oMode == 'o') || (oMode == 'O'))
		{
			cout << "Enter the name of the file.out: ";
			cin >> outFileName;
			outFileStream.open(outFileName.c_str(), ios::out|ios::app); //2 parameters: file name & opening mode
			if (outFileStream.is_open())
			{
				outFileStream << "Brandon Briseno|SID: 1932124|Assignment2: Game of Life|CPSC350-01(German)" << endl;
			}
			else
			{
				oMode = 'x'; //keep valid3 equal to false
			}
		}
		if ((oMode == 'b') || (oMode == 'B') || (oMode == 'w') || (oMode == 'W')||(oMode == 'o') || (oMode == 'O'))
		{
			valid3 == true;
			break;
		}
		else
		{
			cout << endl;
			cout << "Oops, something went wrong. Try again." << endl;
			cout << endl;
			cin.clear(); //clear cin
			cin.ignore(999, '\n'); //ignore fist 999 chars till endl
		}
	}
}
void Life::getArraySizes() //gets dimentions for creating grid
{

	if (inFileStream.is_open()) //get dimentions from file
	{
		string readLine; //string var holder for reading lines

		getline (inFileStream, readLine);
		ogArrRowSize = atoi(readLine.c_str()); //parses cstring
		getline (inFileStream, readLine);
		ogArrColumnSize= atoi(readLine.c_str()); //parses cstring

		arrRowSize = ogArrRowSize + 2;
		arrColumnSize = ogArrColumnSize + 2;
	}
	else //get dimentions from user input
	{
		cout << "Enter the number of desired rows: ";
		cin >> ogArrRowSize;

		cout << "Enter the number of desired columns: ";
		cin >> ogArrColumnSize;

		arrRowSize = ogArrRowSize + 2; //original y + 2
		arrColumnSize = ogArrColumnSize + 2; //original x + 2

		cout << "Enter a decimal value greater than 0 AND less than or equal to 1: ";
		cin >> density;
		if ((density <= 0)||(density > 1)||(ogArrRowSize <= 0)||(ogArrColumnSize <= 0))
		{
			cout << endl;
			cout << "Oops, something went wrong. Try again." << endl;
			cout << endl;
			getArraySizes();
		}
		readingIn == false;
	}
} 
void Life::setArrays() //creates empty grid based from file or RNG
{
	gameTable = new int*[arrRowSize];
	for (int i = 0; i < arrRowSize; ++i)
	{
		gameTable[i] = new int[arrColumnSize];
	}

	currentArray = new int*[arrRowSize];
	for (int j = 0; j < arrRowSize; ++j) //change to i maybe 
	{
		currentArray[j] = new int[arrColumnSize];
	}

	nextArray = new int*[arrRowSize];
	for (int k = 0; k < arrRowSize; ++k) //change to i maybe 
	{
		nextArray[k] = new int[arrColumnSize];
	}
} 
void Life::fillArrayOG(int **arr) //fills the visible gameTable
{
	string readLine;

	if (readingIn == true) //fills array from reading in a file. set to false on line 109.
	{
		for (int i = 1; i < arrRowSize-1; ++i)
		{
			getline (inFileStream, readLine);
			for (int j = 1; j < arrColumnSize-1; ++j)
			{
				if ((readLine[j-1] == 'x') || (readLine[j-1] == 'X'))
				{
					arr[i][j] = ALIVE;
				}
				else if (readLine[j-1] == '-')
				{
					arr[i][j] = DEAD;
				}
			}
		}
	}
	if (readingIn == false)//fills array based on user input
	{
		int Y = 0; //row variable for rng
		int X = 0; //column variable for rng
		int nLiveOnes = ceil((ogArrRowSize*ogArrColumnSize)*(density)); 

		for (int i = 1; i < arrRowSize-1; ++i) //fill board w/ dead stuff
		{
			for (int j = 1; j < arrColumnSize-1; ++j)
			{
				arr[i][j] = DEAD;
			}
		}

		while (nLiveOnes > 0)
		{
			Y = rand() % ogArrRowSize+1;
			X = rand() % ogArrColumnSize+1;
			if (arr[Y][X] != ALIVE)
			{
				arr[Y][X] = ALIVE;
				--nLiveOnes;
			} 
		}
	}
}
void Life::fillArrayBoundary(int **arr) //fills edge of the game table (game mode logic)
{
	if ((gMode == 'c') || (gMode == 'C')) //classic mode
	{
		for (int i = 0; i < arrRowSize; ++i)
		{
			for (int j = 0; j < arrColumnSize; ++j)
			{
				if ((i == 0)||(i == arrRowSize-1)) //checks first row and last
				{
					arr[i][j] = DEAD;
				}
				if ((j == 0)||(j == arrColumnSize-1)) //checks first column and last
				{
					arr[i][j] = DEAD;
				}
			}

		}
	}

	if ((gMode == 'm') || (gMode == 'M') || (gMode == 'd') || (gMode == 'D')) //mirror or d-nut mode
	{
		for (int i = 1; i < arrRowSize-1; ++i) //itterates, checks, then updates index based on gMode
		{
			for (int j = 1; j < arrColumnSize-1; ++j)
			{
				if ((i == 1) && (j == 1)) //(1)top-left of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i-1][j-1] = arr[i][j]; //diagonal left top = current index
						arr[i-1][j] = arr[i][j]; //above = current index
						arr[i][j-1] = arr[i][j]; //leftside = current index
					}
					if ((gMode == 'd')||(gMode == 'D')) 
					{
						arr[i-1][j-1] = arr[arrRowSize-2][arrColumnSize-2]; //diagonal left top = top-right
					}
				}

				if ((i == 1) && (j  > 1) && (j < arrColumnSize-2)) //(2)top-middle of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i-1][j] = arr[i][j]; //above = current index 
					}
					if ((gMode == 'd')||(gMode == 'D'))
					{
						arr[i-1][j-1] = arr[arrRowSize-2][j-1]; //diagonal left top = diagonal left bottom  
						arr[i-1][j] = arr[arrRowSize-2][j]; //above = bottom
						arr[i-1][j+1] = arr[arrRowSize-2][j+1]; //diagonal right top = diagonal right bottom
					}
				}

				if ((i == 1) && (j == arrColumnSize-2)) //(3)top-right of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i-1][j+1] = arr[i][j]; //diagonal right top = current index
						arr[i-1][j] = arr[i][j]; //above = current index
						arr[i][j+1] = arr[i][j]; //rightside = current index
					}
					if ((gMode == 'd')||(gMode == 'D')) 
					{
						arr[i-1][j+1] = arr[arrRowSize-2][1]; //diagonal right top = bottom-left
					}
				}

				if ((i > 1) && (i < arrRowSize-2) && (j == 1)) //(4)middle leftside of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i][j-1] = arr[i][j]; //leftside = current index 
					}
					if ((gMode == 'd')||(gMode == 'D'))
					{
						arr[i+1][j-1] = arr[i+1][arrColumnSize-2]; //diagonal left top = diagonal right top 
						arr[i][j-1] = arr[i][arrColumnSize-2]; //leftside = rightside
						arr[i-1][j-1] = arr[i-1][arrColumnSize-2]; //diagonal left bottom = diagonal right bottom
					}				
				}

				if ((i > 1) && (i < arrRowSize-2) && (j == arrColumnSize-2)) //(5)middle rightside of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i][j+1] = arr[i][j]; //rightside = current index 
					}
					if ((gMode == 'd')||(gMode == 'D'))
					{
						arr[i-1][j+1] = arr[i-1][1]; //diagonal right top = diagonal left top 
						arr[i][j+1] = arr[i][1]; //rightside = leftside
						arr[i+1][j+1] = arr[i+1][1]; //diagonal right bottom = diagonal left bottom
					}				
				}

				if ((i == arrRowSize-2) && (j == 1)) //(6)bottom-left of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i+1][j-1] = arr[i][j]; //diagonal left bottom = current index
						arr[i][j-1] = arr[i][j]; //leftside = current index
						arr[i+1][j] = arr[i][j]; //below = current index
					}
					if ((gMode == 'd')||(gMode == 'D')) 
					{
						arr[i-1][j-1] = arr[arrRowSize-2][arrColumnSize-2]; //diagonal left bottom = bottom-right
					}
				}

				if ((i == arrRowSize-2) && (j  > 1) && (j < arrColumnSize-2)) //(7)bottom-middle of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i+1][j] = arr[i][j]; //below = current index 
					}
					if ((gMode == 'd')||(gMode == 'D'))
					{
						arr[i+1][j-1] = arr[1][j-1]; //diagonal left bottom = diagonal right top  
						arr[i+1][j] = arr[1][j]; //below = above
						arr[i+1][j+1] = arr[1][j+1]; //diagonal right bottom = diagonal left top
					}
				}

				if ((i == arrRowSize-2) && (j == arrColumnSize-2)) //(8)bottom-right of visible gameTable
				{
					if ((gMode == 'm')||(gMode == 'M'))
					{
						arr[i+1][j+1] = arr[i][j]; //diagonal right top = current index
						arr[i][j+1] = arr[i][j]; //rightside = current index
						arr[i+1][j] = arr[i][j]; //below = current index
					}
					if ((gMode == 'd')||(gMode == 'D')) 
					{
						arr[i+1][j+1] = arr[1][1]; //diagonal right bottom = top-left
					}
				}
			}
		}
	}
}
void Life::copyFunction(int **arr1, int **arr2) //copies contents of arr1 to arr2
{
	for (int i = 0; i < arrRowSize; ++i)
	{
		for (int j = 0; j < arrColumnSize; ++j)
		{
			if (arr1[i][j] == ALIVE)
			{
				arr2[i][j] = ALIVE;
			}
			else if (arr1[i][j] == DEAD)
			{
				arr2[i][j] = DEAD;
			}
		}
	}
}
void Life::countAliveOnes(int **arr) //counts number of alive ones in the passed argument/array
{
	int count = 0;
	for (int i = 0; i < arrRowSize; ++i)
	{
		for (int j = 0; j < arrColumnSize; ++j)
		{
			if (arr[i][j] == ALIVE)
			{
				count +=1;
			}
		}
	}
	aliveOnes = count;
}
bool Life::unstableGameBool(int **arr1, int **arr2) //unstable = true
{
	for (int i = 0; i < arrRowSize; ++i)
	{
		for (int j = 0; j < arrColumnSize; ++j)
		{
			if (arr1[i][j] != arr2[i][j]) //if generations are not equal, game is unstable
			{
				bool nullGameTable = true; //true = gameTable has stablaized 
				return true; 
			}
			else
			{
				bool nullGameTable = false; //default
			}
		}
	}
	return false;	
}
void Life::coutOGArray(int **arr) //print original array
{
	cout << "Generation: " << genCount << endl;
	for (int i=1; i < arrRowSize-1; ++i)
	{
		for (int j=1; j < arrColumnSize-1; ++j)
		{
			if (arr[i][j] == ALIVE)
			{
				cout << "X";
			}
			else if (arr[i][j] == DEAD)
			{
				cout << "-";
			}
		}
		cout << endl;
	}
	++genCount;
}
void Life::coutCurrentArray(int **arr) //print current array to console
{
	cout << "Generation: " << genCount << endl;
	for (int i=1; i < arrRowSize-1; ++i)
	{
		for (int j=1; j < arrColumnSize-1; ++j)
		{
			if (arr[i][j] == ALIVE)
			{
				cout << "X";
			}
			else if (arr[i][j] == DEAD)
			{
				cout << "-";
			}
		}
		cout << endl;
	}
	++genCount;
}
/*void Life::coutCurrentArrayBoundary(int **arr) //debugging method: print entire array to console 
{
	cout << "Generation: " << genCount << endl;
	for (int i=0; i < arrRowSize; ++i)
	{
		for (int j=0; j < arrColumnSize; ++j)
		{
			if (arr[i][j] == ALIVE)
			{
				cout << "X";
			}
			else if (arr[i][j] == DEAD)
			{
				cout << "-";
			}
		}
		cout << endl;
	}
	++genCount;
}*/
void Life::outToFile(int **arr) //print og array to file
{ 
	outFileStream << "Generation: " << genCount << endl;
	if (outFileStream.is_open())
	{
		for (int i=1; i < arrRowSize-1; ++i)
		{
			for (int j=1; j < arrColumnSize-1; ++j)
			{
				if (arr[i][j] == ALIVE)
				{
					outFileStream << "X";
				}
				else if (arr[i][j] == DEAD)
				{
					outFileStream << "-";
				}
			}
			outFileStream << endl;
		}
	}
	++genCount;
}
void Life::outStyled()
{
	if ((oMode == 'b')||(oMode == 'B')) //buffer/breif pause
	{
		switch (state)
		{
			case 0:
				coutOGArray(gameTable);
				state = 1;
				break;
			case 1:
				coutCurrentArray(currentArray);
				break;
			default:
				break;
		}
	}
	if ((oMode == 'w')||(oMode == 'W')) //wait for input
	{
		switch (state)
		{
			case 0:
				coutOGArray(gameTable);
				cout << "Press enter to continue..." << endl; //unable to find portable solution for this mode
				cin.get();
				state = 1;
				break;
			case 1:
				coutCurrentArray(currentArray);
				cout << "Press enter to continue..." << endl; //unable to find portable solution for this mode
				cin.get();				
				break;
			default:
				break;
		}
	}
	if ((oMode == 'o')||(oMode == 'O')) //write out to file
	{
		switch (state)
		{
			case 0:
				outToFile(gameTable);
				state = 1;
				break;
			case 1:
				outToFile(currentArray);			
				break;
			default:
				break;
		}	

	}
}
void Life::prepGame()
{
	welcomeFunction();
	getArraySizes();
	setArrays();
	fillArrayOG(gameTable); 
	fillArrayBoundary(gameTable);
	copyFunction(gameTable, currentArray);
	countAliveOnes(currentArray);
	outStyled();
}
void Life::automateLife()
{
	while (nullGameTable == true)
	{
		for (int i=1; i < arrRowSize-1; ++i)
		{
			for (int j=1; j < arrColumnSize-1; ++j)
			{
				if ((currentArray[i-1][j-1])+(currentArray[i-1][j])+(currentArray[i-1][j+1]) //death(isolation) <= 1
				   +(currentArray[i][j-1])+(currentArray[i][j+1])
				   +(currentArray[i+1][j-1])+(currentArray[i+1][j])+(currentArray[i+1][j+1]) <= 1) 
				{
					nextArray[i][j] = DEAD;
				}
				if ((currentArray[i-1][j-1])+(currentArray[i-1][j])+(currentArray[i-1][j+1]) //death(isolation) <= 1
				   +(currentArray[i][j-1])+(currentArray[i][j+1])
				   +(currentArray[i+1][j-1])+(currentArray[i+1][j])+(currentArray[i+1][j+1]) == 2)
				{
					nextArray[i][j] = currentArray[i][j];
				}
				if ((currentArray[i-1][j-1])+(currentArray[i-1][j])+(currentArray[i-1][j+1]) //death(isolation) <= 1
				   +(currentArray[i][j-1])+(currentArray[i][j+1])
				   +(currentArray[i+1][j-1])+(currentArray[i+1][j])+(currentArray[i+1][j+1]) == 3)
				{
					nextArray[i][j] = ALIVE;
				}
				if ((currentArray[i-1][j-1])+(currentArray[i-1][j])+(currentArray[i-1][j+1]) //death(isolation) <= 1
				   +(currentArray[i][j-1])+(currentArray[i][j+1])
				   +(currentArray[i+1][j-1])+(currentArray[i+1][j])+(currentArray[i+1][j+1]) >= 4)
				{
					nextArray[i][j] = DEAD;
				}
			}
		}
		fillArrayBoundary(nextArray);
		if ((unstableGameBool(currentArray, nextArray)) == false)
		{
			if ((oMode == 'o')||(oMode == 'O')) //writing to file
			{
				if (outFileStream.is_open())
				{
					outFileStream << "The generations have stablized. Ending program." << endl;
					cout << "Your game data has been added to your file. Ending program." << endl;
				}
			}
			else //writing to console
			{
				cout << "The generations have stablized. Ending program." << endl;
			}
			break;
		}
		copyFunction(nextArray, currentArray);
		countAliveOnes(currentArray);
		outStyled();
	}
}