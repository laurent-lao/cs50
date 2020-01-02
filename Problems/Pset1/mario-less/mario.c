#include <cs50.h>
#include <stdio.h>

void printSpace(int);
void printHash(int);
int promptAndCheckHeight();

int main(void)
{
    // Prompt user for height
    int height = promptAndCheckHeight();
    
    // Establish HashCount
    for (int hashCount = 1; hashCount <= height; hashCount++)
    {
        // Establish number of Spaces
        int numberOfSpaces = height - hashCount;
        
        // Print lines
        printSpace(numberOfSpaces);
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
