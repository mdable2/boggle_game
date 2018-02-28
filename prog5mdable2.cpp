/*-------------------------------------------------------------------
* Boggle Game
* Class: Program #5 for CS 141, Fall 2017
* Lab: Tuesday, 9am
* TA Moumita Samanta
* System: Windows 10, CodeBlocks
* Author: Mark Dabler
* Last Edit: 11/20/17
*/


#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
#include <stdlib.h>   // For rand
using namespace std;


// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board
const int MaxBoardSize = 36;
const int BoardRows = 6;


//--------------------------------------------------------------------------------
// Display name and program information
void displayIdentifyingInformation()
{
    printf("\n");
    printf("Author: Mark Dabler         \n");
    printf("Program: #5, Boggle        \n");
    printf("TA: Moumita Samanta, 9am Tuesday  \n");
    printf("Nov 20, 2017                \n");
    printf("\n");
}//end displayIdentifyingInformation()


//--------------------------------------------------------------------------------
// Display instructions
void displayInstructions()
{
    printf("Welcome to the game of Boggle, where you play against the clock   \n");
    printf("to see how many words you can find using adjacent letters on the  \n");
    printf("board.  Each letter can be used only once for a given word.       \n");
    printf("  \n");
    printf("When prompted to provide input you may also:                      \n");
    printf("     Enter 'r' to reset the board to user-defined values.         \n");
    printf("     Enter 's' to solve the board and display all possible words. \n");
    printf("     Enter 't' to toggle the timer on/off.                        \n");
    printf("     Enter 'x' to exit the program.                               \n");
    printf("  \n");
}//end displayInstructions()


//---------------------------------------------------------------------------
// Class for dictionary functions
class dictionary {
public:
    // Default constructor
    dictionary();
    // Constructor used to read in dictionary
    dictionary(char ** &dictionaryWords, long int &NumberOfLetters);
    // Deconstructor
    ~dictionary();
};


//---------------------------------------------------------------------------
// Default dictionary constructor
dictionary::dictionary() {};


//---------------------------------------------------------------------------
// Default dictionary deconstructor
dictionary::~dictionary() {};

//---------------------------------------------------------------------------
// Read in dictionary. First dynamically allocate space for the dictionary.  Then read in words from file.
dictionary::dictionary(char ** &dictionaryWords, long int &numberOfWords)
{
    // Allocate space for large array of C-style strings
    dictionaryWords = new char*[ MaxNumberOfWords];

    // For each array entry, allocate space for the word (C-string) to be stored there
    for (int i=0; i < MaxNumberOfWords; i++) {
        dictionaryWords[i] = new char[ MaxWordLength+1];
        // just to be safe, initialize C-strings to all null characters
        for (int j=0; j < MaxWordLength; j++) {
            dictionaryWords[i][j] = '\0';
        }//end for (int j=0...
    }//end for (int i...

    // Now read in the words from the file
    ifstream inStream;                 // declare an input stream for my use
    numberOfWords = 0;                   // Row for the current word
    inStream.open( DictionaryFileName);
    assert( ! inStream.fail() );       // make sure file open was OK

    // Keep repeating while input from the file yields a word
    char theWord[ 81];    // declare input space to be clearly larger than largest word
    while( inStream >> theWord) {
        int wordLength = (int)strlen( theWord);
        if( wordLength >= MinWordLength && wordLength <= MaxWordLength) {
            strcpy( dictionaryWords[ numberOfWords], theWord);
            // increment number of words
            numberOfWords++;
        }
    }//end while( inStream...

    cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
    cout << "Number of words of the right length is:  " << numberOfWords << endl;

    // close the file
    inStream.close();
}


//--------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary array, returning index
// if found, -1 otherwise
int binarySearch( const char searchWord[ MaxWordLength+1], // word to be looked up
                  char **dictionaryWords)               // the dictionary of words
{
    int low, mid, high;     // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1

    // Binary search for word
    low = 0;
    high = MaxNumberOfWords - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, dictionaryWords[ mid]);
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }

    // Word was not found
    return -1;
}//end binarySearch()


//---------------------------------------------------------------------------
// Get random character
// Find random character using a table of letter frequency counts.
// Iterate through the array and find the first position where the random number is
// less than the value stored.  The resulting index position corresponds to the
// letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
    // The following table of values came from the frequency distribution of letters in the dictionary
    float letterPercentTotals[ NumberOfLetters] = {
                0.07680,  //  a
                0.09485,  //  b
                0.13527,  //  c
                0.16824,  //  d
                0.28129,  //  e
                0.29299,  //  f
                0.32033,  //  g
                0.34499,  //  h
                0.43625,  //  i
                0.43783,  //  j
                0.44627,  //  k
                0.49865,  //  l
                0.52743,  //  m
                0.59567,  //  n
                0.66222,  //  o
                0.69246,  //  p
                0.69246,  //  q
                0.76380,  //  r
                0.86042,  //  s
                0.92666,  //  t
                0.95963,  //  u
                0.96892,  //  v
                0.97616,  //  w
                0.97892,  //  x
                0.99510,  //  y
                1.00000}; //  z

    // generate a random number between 0..1
    // Multiply by 1.0 otherwise integer division truncates remainders
    float randomNumber = 1.0 * rand() / RAND_MAX;

    // Find the first position where our random number is less than the
    // value stored.  The index corresponds to the letter to be returned,
    // where 'a' is 0, 'b' is 1, and so on.
    for( int i=0; i<NumberOfLetters; i++) {
        if( randomNumber < letterPercentTotals[ i]) {
            // we found the spot.  Return the corresponding letter
            return (char) 'a' + i;
        }
    }

    // Sanity check
    cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
    exit( -1);
    return ' ';   // Should never get this
}//end getRandomCharacter


//---------------------------------------------------------------------------
// Function to allow user to reset game board
void resetGameBoard(char gameBoard[])
{
    cout << "You have chosen to reset the game board." << endl << "Please type 16 letters for your new board in one line and then click enter (can be separated by spaces)." << endl << "Input new letters: ";

    // Set game board with random letters
    for (int i = 0; i < MaxBoardSize; i++) {
        if ((i >= 7 && i <= 10) || (i >= 13 && i <= 16) ||
            (i >= 19 && i <= 22) || (i >= 25 && i <= 28)) {
            cin >> gameBoard[i];
            gameBoard[i] = tolower(gameBoard[i]);
        }
        else {
            gameBoard[i] = '*';
        }
    }
}


//---------------------------------------------------------------------------
// Function to initialize game board
void initialGameBoard(char gameBoard[])
{
    // Set game board with random letters
    for (int i = 0; i < MaxBoardSize; i++) {
        if ((i >= 7 && i <= 10) || (i >= 13 && i <= 16) ||
            (i >= 19 && i <= 22) || (i >= 25 && i <= 28)) {
            gameBoard[i] = getRandomCharacter();
        }
        else {
            gameBoard[i] = '*';
        }
    }
}


//---------------------------------------------------------------------------
// Function to display game board
void printGameBoard(char gameBoard[])
{
    for (int i = 0; i < MaxBoardSize; i++) {
        cout << gameBoard[i] << " ";
        if ((i+1) % BoardRows == 0) {
            cout << endl;
        }
    }
    cout << endl;
}


//---------------------------------------------------------------------------
// Function to check if user input is on game board, starting at second letter, recursively
void findOtherLetters(char gameBoard[], char userInput[], int indexOfLetterToFind, int boardIndex, bool charFound[])
{
    int offsets[] = {-1, -7, -6, -5, 1, 7, 6, 5};
    int adjacentIndex;
    char temp;

    // Loop through all adjacent board positions
    for (int i = 0; i < 8; i++) {
        adjacentIndex = boardIndex + offsets[i];
        if (gameBoard[adjacentIndex] == userInput[indexOfLetterToFind]) {
            charFound[indexOfLetterToFind] = true;
            temp = userInput[indexOfLetterToFind];
            gameBoard[adjacentIndex] = ' ';
            if ((indexOfLetterToFind + 1) < strlen(userInput)) {
                findOtherLetters(gameBoard, userInput, indexOfLetterToFind + 1, adjacentIndex, charFound);
            }
            gameBoard[adjacentIndex] = temp;
        }
    }
}


//---------------------------------------------------------------------------
// Function to check if user input is on game board
bool findFirstLetter(char gameBoard[], char userInput[])
{
    char temp;
    int indexOfLetterToFind = 0;
    bool charFound[strlen(userInput)] =  {false};

    for (int i = 0; i < MaxBoardSize; i++) {
        if (gameBoard[i] == userInput[indexOfLetterToFind]) {
            charFound[indexOfLetterToFind] = true;
            temp = gameBoard[i];
            gameBoard[i] = ' ';
            if ((indexOfLetterToFind + 1) < strlen(userInput)) {
                findOtherLetters(gameBoard, userInput, indexOfLetterToFind + 1, i, charFound);
            }
            gameBoard[i] = temp;
        }
    }

    // Checks to see if each character is found on board and returns if word was found
    for (int x = 0; x < strlen(userInput); x++) {
        if (charFound[x] && x == (strlen(userInput) - 1)) {
            return true;
        }
        else if (x == (strlen(userInput) - 1)) {
            return false;
        }
    }
}


//---------------------------------------------------------------------------
// Function to check if user input is on game board
void wordsFound(char **dictionaryWords, bool *wordTracker)
{
    cout << "Current words found: ";
    // Display length 3, 4, 5, 6+ in alphabetical order
    for (int i = 0; i < MaxNumberOfWords; i++) {
        if (wordTracker[i] && strlen(dictionaryWords[i]) == 3) {
            cout << dictionaryWords[i] << " ";
        }
    }
    for (int i = 0; i < MaxNumberOfWords; i++) {
        if (wordTracker[i] && strlen(dictionaryWords[i]) == 4) {
            cout << dictionaryWords[i] << " ";
        }
    }
    for (int i = 0; i < MaxNumberOfWords; i++) {
        if (wordTracker[i] && strlen(dictionaryWords[i]) == 5) {
            cout << dictionaryWords[i] << " ";
        }
    }
    for (int i = 0; i < MaxNumberOfWords; i++) {
        if (wordTracker[i] && strlen(dictionaryWords[i]) >= 6) {
            cout << dictionaryWords[i] << " ";
        }
    }
    cout << endl;
}


//---------------------------------------------------------------------------
// Function to check if user input has already been used
bool usedWord(int index, bool wordFound[])
{
    if (wordFound[index]) {
        return true;
    }
    else {
        return false;
    }
}


//---------------------------------------------------------------------------
// Function to calculate and return points earned for valid word
int wordPoints(char userInput[])
{
    if (strlen(userInput) < 3) {
        return 0;
    }
    else if (strlen(userInput) == 3) {
        return 1;
    }
    else if (strlen(userInput) == 4) {
        return 2;
    }
    else if (strlen(userInput) == 5) {
        return 4;
    }
    else {
        return strlen(userInput);
    }
}


//---------------------------------------------------------------------------
// Automatically solves board for user in same way the program does with regular user input, but this time it checks each dictionary entry as user input
void solveBoard(char gameBoard[], char **dictionaryWords, int minimum, int maximum, int &score, bool wordTracker[])
{
    char *solutionInput;
    score = 0;

    for (int i = 0; i < MaxNumberOfWords; i++) {
        if ((strlen(dictionaryWords[i]) >= minimum) && (strlen(dictionaryWords[i]) <= maximum)) {
            solutionInput = dictionaryWords[i];
        }
        if (findFirstLetter(gameBoard, solutionInput)) {
            int dictionaryIndex = binarySearch(solutionInput, dictionaryWords);
            if (dictionaryIndex != -1) {
                if (!usedWord(dictionaryIndex, wordTracker)) {
                    wordTracker[dictionaryIndex] = true;
                    score = score + wordPoints(solutionInput);
                    cout << solutionInput << " ";
                }
            }
        }
    }
}


//---------------------------------------------------------------------------
int main()
{
    // declare variables
    char **dictionaryWords;              // 2d array of dictionary words, dynamically allocated
    long int numberOfWords;              // How many words actually found in dictionary
    char userInput[MaxUserInputLength];  // User input
    char gameBoard[MaxBoardSize];        // Game board char array
    int userScore = 0;
    bool pauseTime = false;
    int tempTime = 0;

    // Declare a variable to hold a time, and get the current time
    time_t startTime = time(NULL);

    displayIdentifyingInformation();
    displayInstructions();

    // Read in dictionary by declaring object dict. numberOfWords returns the actual number of words found
    dictionary dict(dictionaryWords, numberOfWords);

    // Boolean array to track if words have been found or not
    bool wordTracker[MaxNumberOfWords] = {false};

    // Set initial game board
    initialGameBoard(gameBoard);

    // Loop while there is still time left
    cout << "Do word lookups for 60 seconds." << endl;
    int elapsedSeconds = 0;
    while (elapsedSeconds < TotalSecondsToPlay) {
        cout << endl;
        // Prompt for and get user input and print board
        printGameBoard(gameBoard);
        cout << "Current Score: " << userScore << endl;
        cout << "Enter a word: ";
        cin >> userInput;
        for (int i = 0; i < strlen(userInput); i++) {
            userInput[i] = tolower(userInput[i]);
        }

        // User chooses to reset game
        if (userInput[0] == 'r' && strlen(userInput) == 1) {
            cout << endl;
            resetGameBoard(gameBoard);
            userScore = 0;
        }

        // Pause timer
        else if (userInput[0] == 't' && strlen(userInput) == 1) {
            if (!pauseTime) {
                tempTime = difftime( time( NULL), startTime);
                pauseTime = true;
            }
            else {
                pauseTime = false;
                startTime = startTime + tempTime;
            }
        }

        // Exit program
        else if (userInput[0] == 'x' && strlen(userInput) == 1) {
            cout << "Thanks for playing!" << endl;
            return 0;
        }

        // Solve board
        else if (userInput[0] == 's' && strlen(userInput) == 1) {
            int minimum, maximum;

            cout << "You have chosen to solve the puzzle." << endl << "Please enter min length of words you would like to find: ";
            cin >> minimum;
            cout << endl << "Please enter the max length of words you would like to find: ";
            cin >> maximum;

            solveBoard(gameBoard, dictionaryWords, minimum, maximum, userScore, wordTracker);
            cout << endl << "Board score: " << userScore << endl;
            cout << "Thanks for playing!" << endl;
            return 0;
        }

        // Validate user input is on game board and in the dictionary
        else if (findFirstLetter(gameBoard, userInput)) {
            int dictionaryIndex = binarySearch(userInput, dictionaryWords); // Tracks index of word found in dictionary
            if (dictionaryIndex != -1) {
                if (!usedWord(dictionaryIndex, wordTracker)) {
                    cout << userInput << " is on the board and in the dictionary." << endl;
                    cout << userInput << " is worth " << wordPoints(userInput) << " points!" << endl;
                    wordTracker[dictionaryIndex] = true; // Updates wordTracker to reflect word found
                    userScore = userScore + wordPoints(userInput);
                }
                else {
                    cout << userInput << " has already been used. Cannot use words more than once. Try again!" << endl;
                }
            }
            else {
                cout << userInput << " is on the board but not in the dictionary." << endl;
            }
        }
        else {
            cout << userInput << " is not on the board! Please try again." << endl;
        }

        // Print out words found
        wordsFound(dictionaryWords, wordTracker);

        // Calculate how many seconds have elapsed since we started the timer.
        if (!pauseTime) {
            elapsedSeconds = difftime( time( NULL), startTime);
            cout << "There are " << (TotalSecondsToPlay - elapsedSeconds) << " seconds left." << endl;
        }
    }
    cout << "I let you finish your last move. Time is up!" << endl;

    return 0;
}//end main()
