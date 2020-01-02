#include <cs50.h>
#include <stdio.h>
#include <string.h>

char cipherByKey(char, int);

int main(int argc, string argv[])
{
	bool isValid = false;
	int key = 0;

	// Try to parse key
	if (argc == 2)
	{
		key = atoi(argv[1]);
		// if argument was 0, it's fine
		if (key > 0)
		{
			isValid = true;
		}
	}

	if (!isValid)
	{
		printf("%s\n", "Usage: ./caesar key\n");
		return 1;
	}

    string inputString = get_string("plaintext: ");
    char cipheredText[strlen(inputString)];

    for (int index = 0; index < strlen(inputString); index++)
    {
    	cipheredText[index] = cipherByKey(inputString[index], key);
    }

    printf("ciphertext: ");
    printf("%s\n", cipheredText);
}

char cipherByKey(char characterToCipher, int key)
{
	char valueOfA = 0;
	bool isLower = characterToCipher >= 'a' && characterToCipher <= 'z';
	bool isUpper = characterToCipher >= 'A' && characterToCipher <= 'Z';

	// Just return the character if not alpha
	if (!isLower && !isUpper)
	{
		return characterToCipher;
	}

	// Assign good value of A
	if (isLower)
	{
		valueOfA = 'a';
	}
	else
	{
		valueOfA = 'A';
	}

	// Character manip
	int characterToAlphaIndex = characterToCipher - valueOfA; // a = 0, z = 25
	int characterWithKey = characterToAlphaIndex + key;
	int keyedMappedToAlpha = characterWithKey % 26;

	return keyedMappedToAlpha + valueOfA;
}
