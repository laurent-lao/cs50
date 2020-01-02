#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool isLower(char);
bool isUpper(char);
int alphaToKey(char);
char encryptChar(int, char);

int main(int argc, string argv[])
{
    //Argument checking
    bool isValidArgument = true;
    if (argc != 2)
    {
        isValidArgument = false;
    }
    else
    {
        // Check if every letter is alpha in argument
        for (int index = 0; index < strlen(argv[1]); index++)
        {
            char charAtIndex = argv[1][index];
            if (!isLower(charAtIndex) && !isUpper(charAtIndex))
            {
                // Stop the loop immediately when encounter non-alpha character
                isValidArgument = false;
                break;
            }
        }
    }
    
    // Exit program if non-valid argument
    if (!isValidArgument)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    
    // Get plaintext
    string plaintext = get_string("plaintext: ");
    
    int keywordLength = strlen(argv[1]);
    int plaintextLength = strlen(plaintext);
    int keywordIndex = 0;
    int plaintextIndex = 0;
    
    while (plaintextIndex < plaintextLength)
    {
        // Wrap around KeywordIndex
        if (keywordIndex == keywordLength)
        {
            keywordIndex %= keywordLength;
        }
        
        char plaintextChar = plaintext[plaintextIndex];
        if (!isLower(plaintextChar) && !isUpper(plaintextChar))
        {
            // Move on
            plaintextIndex++;
            continue;
        }
        else
        {
            // Encrypt with key
            int key = alphaToKey(argv[1][keywordIndex++]);
            plaintext[plaintextIndex] = encryptChar(key, plaintext[plaintextIndex]);
            plaintextIndex++;
        }
    }
    
    printf("ciphertext: %s\n", plaintext);
    return 0;
}

int alphaToKey(char character)
{
    int valueOfA = 0;
    
    if (isLower(character))
    {
        valueOfA = 'a';
    }
    else if (isUpper(character))
    {
        valueOfA = 'A';
    }
    else 
    {
        return 0;
    }
    
    return (character - valueOfA);
}

char encryptChar(int key, char character)
{
    int valueOfA = 0;
    if (isLower(character))
    {
        valueOfA = 'a';
    }
    else if (isUpper(character))
    {
        valueOfA = 'A';
    }
    else
    {
        return character;
    }
    
    return (((character - valueOfA) + key) % 26) + valueOfA;
}

bool isLower(char character)
{
    if (character >= 'a' && character <= 'z')
    {
        return true;
    }
    
    return false;
}

bool isUpper(char character)
{
    if (character >= 'A' && character <= 'Z')
    {
        return true;
    }
    
    return false;
}
