//Zareen Subah
//zxs170630

#include <iostream>
#include <fstream>
#include <iomanip> //used to format the file's output

using namespace std;

bool validateRow(char*, ofstream&); //function prototype for validate row
bool validateColumn(char*, ofstream&); //function prototype for validate column
bool validateGrid(char*, ofstream&); //function prototype for validate grid
char findDuplicate(char*); //function prototype for finding duplicate
string gridPosition(int); //function prototype for reasons if sudoku is not valid

int main()
{
    int puzzleNumber; //define the variable as an integer
    char *numbers = new char[81]; //dynamically allocate the array
    bool solved = true; //define the boolean variable and initialize to true

    ifstream input ("puzzles2.txt"); //define and open the input file
    ofstream output ("solutions.txt"); //define and open the output file

    if (input) //if the file exists
    {
        while(input.peek()!= EOF) //while the character is not the end of file marker
        {
            input >> noskipws; //do not skip any white space
            input >> puzzleNumber; //read in the puzzle number

            for(int i = 0; i < 81; i++)
            {
                if(input.peek()== '\n') //if the character is a new line
                    input.get(); //ignore every newline till next puzzle number or EOF

                input >> *numbers; //read in the numbers

                if(*numbers == ' ') //if the character is a space
                    solved = false; //set solved to false which means the sudoku is not solved

                numbers++; //use pointer arithmetic to move the pointer
            }

            numbers -= 81; //move the pointer back to the beginning

            while(input.peek() == '\n')
                input.get(); //ignore every newline till next puzzle number or EOF

            output << setw(15) << left << puzzleNumber; //write the puzzle number to the file

            if(validateRow(numbers, output)) //if the function returns true
            {
                if(validateColumn(numbers, output)) //if the function returns true
                {
                    if(validateGrid(numbers, output)) //if the function returns true
                    {
                        if(solved == true) //if there are no spaces which means solved is true
                            output << setw(15) << left << "solved" << endl; //write solved to the file

                        else
                            //else if the validation functions return true but solved is false (i.e. there are spaces) it means the puzzle is valid
                            output << setw(15) << left << "valid" << endl; //write valid to the file
                    }
                }
                solved = true; //set solved to true
            }
        }
        output.close(); //close the output file
        input.close(); //close the input file
        delete [] numbers; //free the allocated memory
        return 0;
    }

}


//this function validates the rows to search for duplicates
bool validateRow(char *numbers, ofstream &output)
{

    char *numberSet = new char[9]; //dynamically allocate the char array
    char duplicateNum; //define the variable as a char

    for(int i = 0; i < 9; i++) //for loop that iterates for 9 elements
    {
        for(int j = 0; j < 9; j++) //inner for loop iterates for 9 elements
        {
            *numberSet = *numbers; //assign each number of the numbers array to the numberSet array
            numberSet++; //move the pointer by incrementing 1
            numbers++; //move the pointer by incrementing 1
        }
        numberSet -= 9; //move the pointer to the beginning using pointer arithmetic
        duplicateNum = findDuplicate(numberSet); //assign the value returned by the function to the duplicateNum variable

        if(duplicateNum != 'V') //if the character returned is not V, which means there is a duplicate
        {
            //display the number that is duplicate and the row where it's found
            output << setw(25) << left << "invalid" << "multiple " << duplicateNum << "s in row "<< i+1 << endl;
            return false; //return false if all rows are not valid
        }
    }

    delete [] numberSet; //free the allocated memory
    return true; //return true if all rows are valid
}

//this function validates the columns to check for duplicates
bool validateColumn(char *numbers, ofstream &output)
{
    char *numberSet = new char[9]; //dynamically allocate the array
    char duplicateNum; //define the variable

    for(int i = 0; i < 9; i++) //outer for loop iterating through 9 elements
    {
        for(int j = 0; j < 9; j++) //inner for loop iterating through 9 elements
        {
            *numberSet = *numbers; //assign each number of the numbers array to the numberSet array
            numberSet++; //move the pointer by incrementing 1
            numbers += 9; //add 9 to the pointer to move it to the next element in that column
        }

        numbers -= 81; //move the numbers pointer back to the beginning (since there are two loops- 9 times 9 is equal to 81)
        numbers++; //increment the numbers pointer to move it

        numberSet -= 9; //move the numberSet pointer 9 units back
        duplicateNum = findDuplicate(numberSet); //assign the value returned by the function to the duplicate mum variable

        if(duplicateNum != 'V') //if the character returned is not V, which means there is a duplicate
        {
            //display the number that is duplicate and the column where it's found
            //add one to the counter to write the corresponding column number
            output << setw(25) << left << "invalid" << "multiple " << duplicateNum << "s in column " << i+1 << endl;
            return false; //return false if all columns are not valid
        }
    }
    delete [] numberSet; //free the allocated memory
    return true; //return true for the columns being valid
}

//this function validates the 3x3 grids
bool validateGrid(char *numbers, ofstream &output)
{
    int gridNumber = 0; //define the variable as an integer and initialize to zero
    char *numberSet = new char[9]; //dynamically allocate the array
    char duplicateNum; //define the variable as a character

    for (int i = 0; i < 3; i++) //for loop that iterates for 3 elements
    {
        for (int j = 0; j < 3; j++) //first inner for loop that iterates for 3 elements
        {
            for(int k = 0; k < 3; k++) //second inner for loop that iterates for 3 elements
            {
                for(int l = 0; l < 3; l++) //third inner for loop that iterates for 3 elements
                {
                    *numberSet = *numbers; //assign each number of the number array to the numberSet array
                    numberSet++; //increment the pointer numberSet by one to move it
                    numbers++; //increment the pointer numbers by one to move it
                }

                numbers += 6; //move the pointer 6 units to the right to move the next grid
            }

            gridNumber++; //add one to the grid number
            numbers -= 24; //move the pointer 24 units to the left to move back
            numberSet -=9; //move the pointer numberSet to move to the beginning

            duplicateNum = findDuplicate(numberSet); //assign the value returned by the function into the duplicateNum variable

            if (duplicateNum != 'V') //if the char returned by the function is not V it means there is a duplicate
            {
                //write the number that is duplicate
                //write the string returned by the gridPosition function that specifies which grid the duplicate is found in
                output << setw(25) << left << "invalid" << "multiple " << duplicateNum << "s " << gridPosition(gridNumber) << " grid" << endl;
                return false; //return false because the all 3x3 grids are not valid
            }
        }
        numbers += 18; //move the pointer 18 units to the right
    }

    delete [] numberSet; //free the allocated memory
    return true; //return true if all the grids are valid
}

//this function determines if there is a duplicate and returns the number which is the duplicate
char findDuplicate(char *numberSet)
{
    char temp; //define the character variable
    for(int i = 0; i < 9; i++)
    {
        numberSet += i; //add the counter variable to the pointer to move it
        temp = *numberSet; //set temp to the value in the numberSet variable
        numberSet -= i; //subtract the counter variable to the pointer to move it back

        if (temp == ' ') //if the character is a space
            continue; //skip the current iteration of the array and start the next iteration

        for(int j = 0; j < 9; j++)
        {
            //if the outer loop's counter is not equal to the inner loop's counter
            //and temp is equal to the current number in the array
            if(i!=j && temp == *numberSet)
                return temp; //return the number that is found duplicate

            numberSet++; //increment the pointer to move it
        }
        numberSet -= 9; //move the pointer to the beginning
    }

    return 'V'; //else return V (V is a random character used to show that there are no duplicates found)
}


//this function determines in which grid the duplicate number is found using the grid number
string gridPosition (int gridNumber)
{
    //switch case
    switch(gridNumber)
    {
    case 1:
        return "upper left"; //the duplicate is in the top left grid
    case 2:
        return "upper middle"; //the duplicate is in the top middle grid
    case 3:
        return "upper right"; //the duplicate is in the top right grid
    case 4:
        return "middle left"; //the duplicate is in the middle left grid
    case 5:
        return "middle most"; //the duplicate is in the middle most grid
    case 6:
        return "middle right"; //the duplicate is in the middle right grid
    case 7:
        return "lower left"; //the duplicate is in the bottom left grid
    case 8:
        return "lower middle"; //the duplicate is in the bottom middle grid
    case 9:
        return "lower right"; //the duplicate is in the bottom right grid
    }
}
