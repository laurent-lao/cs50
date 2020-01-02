#include <stdio.h>
#include <cs50.h>

long power(long number, long power);
long promptUserForCardNumber();
long getDigitAtLocation(long, int);
int getNumberOfDigits(long);
long firstTwoNumbers(long number);

bool checksumValid(long);
bool hasAMEXStart(long);
bool hasVISAStart(long);
bool hasMCStart(long);

int main()
{
    long cardNumber = promptUserForCardNumber();
    
    if ((!checksumValid(cardNumber)
        || (!hasAMEXStart(cardNumber) && !hasVISAStart(cardNumber) && !hasMCStart(cardNumber))))
    {
        printf("INVALID\n");
    }
    else if (hasAMEXStart(cardNumber))
    {
        printf("AMEX\n");
    }
    else if (hasVISAStart(cardNumber))
    {
        printf("VISA\n");
    }
    else if (hasMCStart(cardNumber))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        // Shouldn't hit here, but just in case
        printf("Unhandled\n");
    }
}

long promptUserForCardNumber()
{
    long userInput = 0;
    bool isValid = false;
    
    // Prompt and check
    do
    {
        userInput = get_long("Card Number: ");
        if (userInput > 0)
        {
            isValid = true;
        }
    }
    while (!isValid);
    
    return userInput;
}

long power(long number, long power)
{
    long powerResult = 1;
    
    for (long i = 0; i < power; i++)
    {
        powerResult *= number;
    }
    
    return powerResult;
}

int getNumberOfDigits(long number)
{
    int count = 1;
    
    while ((number / power(10, count) != 0))
    {
        count++;
    }
    
    return count;
}

long getDigitAtLocation(long number, int location)
{
    // Cut off all number before location
    long truncateNumber = number / power(10, location);
    // Cut off all number after location
    long digit = truncateNumber % 10;
    
    return digit;
}

bool checksumValid(long number)
{
    long sum = 0;
    bool hasToDouble = false; 
    int numberOfDigits = getNumberOfDigits(number);
    
    for (int location = 0; location < numberOfDigits; location++)
    {
        int digitAtLocation = getDigitAtLocation(number, location);
        
        if (!hasToDouble)
        {
            sum += digitAtLocation;
            hasToDouble = true;
        }
        else
        {
            int doubled = digitAtLocation * 2;
            
            // add the digits only
            while (doubled != 0)
            {
                sum += doubled % 10;
                doubled /= 10;
            }
            hasToDouble = false;
        }
    }
    
    // Last number is 0?
    return (sum % 10 == 0) ? true : false;
}

long firstTwoNumbers(long number)
{
    int numberOfDigits = getNumberOfDigits(number);
    return number / power(10, numberOfDigits - 2);
}

bool hasAMEXStart(long number)
{
    int numberOfDigits = getNumberOfDigits(number);
    
    if (numberOfDigits != 15) // Valid number of digits?
    {
        return false;
    }
    else
    {
        long startNums = firstTwoNumbers(number);
        
        if (startNums == 34 || startNums == 37) // Got first two numbers?
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool hasVISAStart(long number)
{
    int numberOfDigits = getNumberOfDigits(number);
    
    if ((numberOfDigits != 13 && numberOfDigits != 16) 
       || getDigitAtLocation(number, numberOfDigits - 1) != 4)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool hasMCStart(long number)
{
    int numberOfDigits = getNumberOfDigits(number);
    
    if (numberOfDigits != 16)
    {
        return false;
    }
    else
    {
        long startNums = firstTwoNumbers(number);
        
        if (startNums >= 51 && startNums <= 55)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
