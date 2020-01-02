#include <stdio.h>
#include <cs50.h>
#include <math.h>

float promptUserForCashOwed();

int main()
{
    float cashOwed = promptUserForCashOwed();
    
    // Round
    int centsOwed = round(cashOwed * 100);
    
    int remaining = centsOwed;
    int coinCount = 0;
    
    // Number of 25 cents
    int numberOf25 = remaining / 25;
    remaining %= 25;
    
    // Number of 10 cents
    int numberOf10 = remaining / 10;
    remaining %= 10;
    
    // Number of 5 cents
    int numberOf5 = remaining / 5;
    remaining %= 5;
    
    // Number of 1 cents
    int numberOf1 = remaining;
    
    // Show coin count
    coinCount = numberOf25 + numberOf10 + numberOf5 + numberOf1;
    printf("%d\n", coinCount);
    
}

float promptUserForCashOwed()
{
    float userInput = 0;
    bool isValid = false;
    
    do
    {
        userInput = get_float("Change owed: ");
        
        if (userInput >= 0)
        {
            isValid = true;
        }
    }
    while (!isValid);
    
    return userInput;
}
