/****************
* Ryan         *
* RPG GAME     *
* Due 10/30/16 *
****************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#define PAUSE system("pause")
#define CLS system("cls")
#define NUMSCORES 10

// Data Types
typedef struct
{
	int numWins;
	int numLosses;
	int currentWinStr;
	int currentLossStr;
	int bestWinStreak;
	int bestLossStreak;
}GAMES; // This data type contains the data on the current game

typedef struct
{
	char name[20];
	int lives;
	int money;
	int strength;
	GAMES games;
}PLAYER; // This data type contains the data needed for the game

typedef struct
{
	char name[20];
	int money;
}HIGHSCORES; // This data type will be used to keep track of the high scores

// Functions

// Menu
void printMenu();
char runMenu();

// Initializes the variables for the start of the game
PLAYER initializePlayer();
void initializeGames(GAMES* g);
void initializeHighScores(HIGHSCORES h[], int* numGames);
void readHighScores(HIGHSCORES h[], int* numGames);

// Error Checking Scan Functions
char scanChar();
int scanInt();
float scanFloat();

// Play the game
void playGame(PLAYER* p);
int checkWin(int randValue, int strength);
void printPlayer(PLAYER p);
void runStore(PLAYER* p);

// Stats
void viewCurrentGame(GAMES g);
char gameOver(PLAYER* player, HIGHSCORES scores[], int* numGames);
void sortHighScores(int numScores, HIGHSCORES unSort[]);
void displayHighScores(HIGHSCORES h[], int num);
void writeHighScores(HIGHSCORES h[], int numGames);

int main(void) // Begin Main
{
	srand(time(NULL));

	PLAYER player;
	HIGHSCORES scores[NUMSCORES];
	int numGames = 0;
	char menu;

	player = initializePlayer();
	initializeHighScores(scores, &numGames);

	do
	{
		CLS;
		printPlayer(player);
		menu = runMenu();


		switch (menu)
		{
		case 'F': // Fight a battle
			playGame(&player);
			break;
		case 'S': // Buy from the shop
			runStore(&player);
			break;
		case 'C': // Current Battle Stats
			CLS;
			printf("\n");
			printPlayer(player);
			viewCurrentGame(player.games);
			break;
		case 'H': // High Scores
			displayHighScores(scores, numGames);
			break;
		case 'X': // Exit
			CLS;
			printf("\n\n\t*************************"
				"\n\t* Thank you for playing *"
				"\n\t*************************\n\n\t");
			PAUSE;
			return 0;
		default:
			break;
		}

		if (!player.lives)
		{
			menu = gameOver(&player, scores, &numGames);
			if (menu == 'Y')
				player = initializePlayer();
			else
				menu = 'X';
		}
	} while (menu != 'X');
} // END Main

/*******************************************************************
Function: Comment Header
Data Needed:

Data Returned:

Purpose:

*******************************************************************/


/*******************************************************************
Function: runMenu

Data Needed:
None

Data Returned:
char

Purpose:
This function controls the menu of the program. It returns a
character that will be used within the switch in main.
*******************************************************************/
char runMenu()
{
	char result;

	do
	{
		printMenu(); // prints the menu
		printf("\n\n\tEntry : ");
		result = scanChar(); // character returning function
	} while (result == '0'); // value will be '0' if the user is a fool, or if they enter an invalid entry in scanChar

	return result;
}// END runMenu


/*******************************************************************
Function: printMenu

Data Needed:
None

Data Returned:
None

Purpose:
Prints the menu
*******************************************************************/
void printMenu()
{
	printf("\n\n\t*****************************"
		"\n\t* 'F' : Fight a Battle      *"
		"\n\t* 'S' : Go to the Store     *"
		"\n\t* 'C' : View Current Scores *"
		"\n\t* 'H' : View High Scores    *"
		"\n\t* 'X' : Exit the Program    *"
		"\n\t*****************************");
	return;
} // END printMenu


/*******************************************************************
Function: initializePlayer

Data Needed:
NONE

Data Returned:
PLAYER, this data type will be used throughout the function
PLAYER incluses:
name stored as a string
lives stored as int
money stored as int
strength stored as int
GAMES see function initializeGames

Purpose:
To initialize the variable of type PLAYER for use during the
game.

*******************************************************************/
PLAYER initializePlayer()
{
	PLAYER p;
	CLS;
	printf("\n\t**************"
		"\n\t* FIGHT CLUB *"
		"\n\t**************\n");
	printf("\n\tCharacter's Name: ");
	scanf(" %[A-Z a-z]", p.name);
	p.lives = 4;
	p.money = 0;
	p.strength = 0;
	initializeGames(&p.games); // initializes GAMES p.games
	return p;
} // END initializePlayer


/*******************************************************************
Function: initializeGames

Data Needed:
a pointer to a variable of type GAMES

Data Returned:
NONE, however we are passing by reference.

Purpose:
To initialize our values within the GAMES datatype
*******************************************************************/
void initializeGames(GAMES* g)
{
	g->numWins = 0;
	g->numLosses = 0;
	g->currentLossStr = 0;
	g->currentWinStr = 0;
	g->bestWinStreak = 0;
	g->bestLossStreak = 0;
	return;
} // END initializeGames


/*******************************************************************
Function: initializeHighScores

Data Needed:
NONE

Data Returned:
A variable of type HIGHSCORES

Purpose:
setting the intial conditions of the high scores.
*******************************************************************/
void initializeHighScores(HIGHSCORES h[], int* numGames)
{
	int i; // this is a counter

	for (i = 0; i < NUMSCORES; i++)
		h[i].money = 0;

	readHighScores(h, numGames);
	return;
} // END highScores


/*******************************************************************
Function: readHighScores

Data Needed:
an array of type HIGHSCORES
a pointer of type int

Data Returned:
NONE

Purpose:
To read our bin file of highScores to populate or array of scores
*******************************************************************/
void readHighScores(HIGHSCORES h[], int* numGames)
{
	FILE* scoresPtr;
	scoresPtr = fopen("highScores.bin", "rb");  // file open for reading

	if (scoresPtr == NULL)
		return;

	fread(numGames, sizeof(int), 1, scoresPtr);
	fread(h, sizeof(HIGHSCORES), (*numGames), scoresPtr);

	fclose(scoresPtr); // close file
	return;
} // END readHighScores

/************************************************************************
Function: writeHighScores

Data Needed:
1) highScores[]		contains stored highScores
2) numScores		refrences the number of highscores in the array

Data Returned:
NONE

Purpose:
This function writes the binary file we will be using to store
the best scores.
***********************************************************************/
void writeHighScores(HIGHSCORES h[], int numGames)
{
	FILE* scoresPtr;
	scoresPtr = fopen("highScores.bin", "wb"); // file open for writing

	if (scoresPtr == NULL) // Terminates the program when an error occurs writing the file
	{
		printf("Error WRITING the File");
		PAUSE;
		exit(1);
	}

	fwrite(&numGames, sizeof(int), 1, scoresPtr);
	fwrite(h, sizeof(HIGHSCORES), numGames, scoresPtr);

	fclose(scoresPtr); // file closed
	return;
} // END readHighScores

/*******************************************************************
scanInt, ScanFloat both check to ensure that the value assigned
through scanf() is valid for that particular data type
*******************************************************************/
int scanInt()
{

	int intNum;

	if (scanf("%d", &intNum)) //returns true if the input is an int
		return intNum;
	else
	{
		return -999;
	}
}


/*******************************************************************
scanInt, ScanFloat both check to ensure that the value assigned
through scanf() is valid for that particular data type
*******************************************************************/
float scanFloat()
{

	float num;

	if (scanf("%f", &num)) //returns true if the input is a float
		return num;
	else
	{
		return -999;
	}
}


/*******************************************************************
scanChar is similar to the other two except it has an additional
condition to change lower case letters to upper case
*******************************************************************/
char scanChar()
{

	char result;

	if (scanf(" %c", &result)) //returns true if the input is a char
	{
		if (islower(result));
		result = toupper(result);
	}
	else
	{
		(result) = '0';
		printf("\n\t*******************"
			"\n\t*  Invalid Entry  *"
			"\n\t*******************\n\n\t");
		PAUSE;
	}

	return result;
}


/*******************************************************************
Function: playGame

Data Needed:
Pointer to a variable of type PLAYER

Data Returned:
NONE

Purpose:
To run the game once. The win or loss will be recorded within
the PLAYER struct.

*******************************************************************/
void playGame(PLAYER* p)
{
	int randValue;

	randValue = rand() % 100;
	randValue++;

	if (checkWin(randValue, p->strength))	//  if we won
	{
		p->money += 50;
		p->games.numWins++;
		p->strength += 5;
		p->games.currentWinStr++;

		p->games.currentLossStr = 0;

		CLS;
		printPlayer(*p);
		printMenu();
		printf("\n\n\t***********"
			"\n\t* YOU WON *"
			"\n\t***********\n\n\t");
		PAUSE;
	}										// we won
	else									// else we lost
	{
		p->lives--;
		p->games.numLosses++;

		if (p->strength)
			p->strength -= 5;

		p->games.currentLossStr++;

		p->games.currentWinStr = 0;

		CLS;
		printPlayer(*p);
		printMenu();
		printf("\n\n\t************"
			"\n\t* YOU LOST *"
			"\n\t************\n\n\t");
		PAUSE;
	}										// we lost

	if (p->games.currentLossStr > p->games.bestLossStreak)	// checks if the current loss streak is larger than the highest loss streak							
		p->games.bestLossStreak = p->games.currentLossStr;

	if (p->games.currentWinStr > p->games.bestWinStreak)	// checks if the current win streak is larger than the highest win streak
		p->games.bestWinStreak = p->games.currentWinStr;

	return;
} // END playGame


/*******************************************************************
Function: checkWin

Data Needed:
randValue: is a random int which will be used to deterime if we won
strength: is an int which weighs whether or not we won

Data Returned:
1 : if we won
0 : if we lost

Purpose:
This function determines whether or not we won the game. It then
returns a flag.
*******************************************************************/
int checkWin(int randValue, int strength)
{
	int flag = 0;

	if (strength > 30)
		strength = 30;



	if (randValue + strength >= 50) // weighs the rand value based off strength
		flag = 1;
	return flag;
} // END checkWin


/*******************************************************************
Function: printPlayer

Data Needed:
p : is a variable of type PLAYER

Data Returned:
NONE

Purpose:
Prints the values stored in PLAYER
*******************************************************************/
void printPlayer(PLAYER p)
{
	printf("\n\tName\t : %s\n\tLives\t : %d\n\tStrength : %d\n\tMoney \t : $%d"
		, p.name, p.lives, p.strength, p.money);
	return;
} // END printPlayer


/*******************************************************************
Function: runStore

Data Needed:
pointer to a variable of PLAYER type

Data Returned:
NONE

Purpose:
To allow the user to buy items within the game.
*******************************************************************/
void runStore(PLAYER* p)
{
	char result;
	do
	{
		CLS;
		printPlayer(*p); // Will display current money/ stats
		printf("\n\n\t*****************************"
			"\n\t* 'S' : 5 Strength  : $50   *"
			"\n\t* 'L' : 1 Life      : $100  *"
			"\n\t* 'R' : Return to Main Menu *"
			"\n\t*****************************"
			"\n\tEntry: ");
		do
		{
			result = scanChar();
		} while (result == '0'); // checks if result is a character
		switch (result)
		{
		case 'L': // lives
			if (p->money >= 100) // checks if they have enough funds
			{
				p->money -= 100;
				p->lives++;
			}
			else
			{
				printf("\n\tNOT ENOUGH MONEY\n\t");
				PAUSE;
			}
			break;
		case 'S': // strength
			if (p->money >= 50) // checks if they have enough funds
			{
				p->money -= 50;
				p->strength += 5;
			}
			else
			{
				printf("\n\tNOT ENOUGH MONEY\n\t");
				PAUSE;
			}
			break;
		case 'R': // Leave Shop
			break;
		default:
			printf("\n\tChoose a valid menu option\n\t");
			PAUSE;
			break;
		}
	} while (result != 'R');
	return;
} // END runStore


/*******************************************************************
Function: viewCurrentGame

Data Needed:
A variable of type PLAYER

Data Returned:
NONE

Purpose:
Print the stats of the current game
*******************************************************************/
void viewCurrentGame(GAMES g)
{
	printf("\n\tWins \t : %d\n\tLosses \t : %d", g.numWins, g.numLosses);
	printf("\n\n\tBest Winning Streak : %d", g.bestWinStreak);
	printf("\n\tWorst Losing Streak : %d\n\n\t", g.bestLossStreak);
	PAUSE;
	return;
} // END viewCurrentGame


/*******************************************************************
Function: gameOver

Data Needed:
player   : pointer to data type PLAYER
scores   : an array of ints
numGames : an int for the number of games

Data Returned:
NONE

Purpose:
Runs when lives = 0. resets the values, checks them against the highscores
*******************************************************************/
char gameOver(PLAYER* player, HIGHSCORES scores[], int* numGames)
{
	char result;
	if (*numGames < NUMSCORES)
	{
		scores[*numGames].money = player->money;
		strcpy(scores[*numGames].name, player->name);
		(*numGames)++;
	}
	else if (player->money > scores[NUMSCORES].money)
	{
		scores[NUMSCORES].money = player->money;
		strcpy(scores[NUMSCORES].name, player->name);
	}

	sortHighScores(*numGames, scores);
	writeHighScores(scores, *numGames);
	displayHighScores(scores, *numGames);
	printf("\n\n\tPlay Again? (Y/N) : ");
	do
	{
		result = scanChar();
	} while (result == '0'); // checks if result is a character
	return result;
} // END gameOver


/*******************************************************************
Function: sortHighScores

Data Needed:
unSort[] an array of type HIGHSCORES
numScores an int for the number of items to sort

Data Returned:
NONE, but this function passes by reference

Purpose:
To sort the list in terms of highest money while also keeping
the string aligned.
*******************************************************************/
void sortHighScores(int numScores, HIGHSCORES unSort[])
{
	char switchMade; // a bool esque value
	HIGHSCORES temp; // a temporary variable to swap with
	int count; // loop counter

	do // begin do while
	{
		switchMade = 'N'; // bool initialized to FALSE
		for (count = 0; count < (numScores - 1); count++) // This will run once for each location in the array
		{
			if (unSort[count].money < unSort[count + 1].money) // checks to see if the next value is greater than the previous
			{
				//standard bubblesort
				temp = unSort[count + 1];
				unSort[count + 1] = unSort[count];
				unSort[count] = temp;
				//end bubblesort
				switchMade = 'Y'; // bool initialized to TRUE
			}
		}
	} while (switchMade == 'Y'); // end do while
	return;
} // END sortHighScores



/*******************************************************************
Function: displayHighScores

Data Needed:
an array of type HIGHSCORES
an int which represents the elements within the array

Data Returned:
NONE

Purpose:
Output the contents of the array to the user
*******************************************************************/
void displayHighScores(HIGHSCORES h[], int num)
{
	int i;
	CLS;
	printf("\n\n\tGame\tName\tMoney");
	printf("\n\t_____________________");
	for (i = 0; i < num; i++)
		printf("\n\t%2d\t%s\t$%d", i + 1, h[i].name, h[i].money);
	printf("\n\t");
	PAUSE;
	return;
} // END displayHighScores 