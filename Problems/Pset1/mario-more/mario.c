#include <cs50.h>
#include <stdio.h>

int promptAndCheckHeight();
void printSpace(int);
void printHash(int);

int main(void)
{
    // Get Height from user
    int height = promptAndCheckHeight();
    
    // Establish number of hashes
    for (int hashCount = 1; hashCount <= height; hashCount++)
    {
        // Establish number of spaces
        int numberOfSpaces = height - hashCount;
        
        // Print line
        printSpace(numberOfSpaces);
        printHash(hashCount);
        printSpace(2);
        printHash(hashCount);
        printf("\n");
    }
}

int promptAndCheckHeight()
{
    int userInput = 0;
    bool isValid = false;
    
    do
    {
        //Prompt
        userInput = get_int("Height: ");
        
        // Check Validity
        if (userInput > 0 && userInput <= 8)
        {
            isValid = true;
        }
    } while (!isValid);
    
    // Since is now valid
    return userInput;
}

void printSpace(int numberOfSpaces)
{
    for (int count = 0; count < numberOfSpaces; count++)
    {
        printf(" ");
    }
}

void printHash(int numberOfHashes)
{
    for (int count = 0; count < numberOfHashes; count++)
    {
        printf("#");
    }
}
