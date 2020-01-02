#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

char getCharAtID(int);
const int maxAlpha = 51;
const int startAlpha = -1;
const int maxLength = 5;

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Bad usage.\n");
        return 1;
    }

    // Get the salt
    char salt[3];
    for (int index = 0; index <= 2; index++)
    {
        if (index == 2)
        {
            salt[index] = '\0';
        }
        else
        {
            salt[index] = argv[1][index];
        }
    }

    
    for (int arraySize = 1; arraySize <= maxLength; arraySize++)
    {
        // Create array 'aaaa....'
        char tempKey[arraySize + 1];
        tempKey[arraySize] = '\0';
        for (int index = 0; index < arraySize; index++)
        {
            tempKey[index] = 'A';
        }

        // Create array of charIDs '[0, 0, 0,...]'
        int ids[arraySize];
        for (int index = 0; index < arraySize; index++)
        {
            ids[index] = 0;
        }
        
        // Use the charIDs to create keys
        bool isDone = false;
        while (!isDone)
        {
            // Check if is done by checking whether each ids are = 51
            bool foundNon51 = false;
            for (int index = 0; index < arraySize; index++)
            {
                if (ids[index] != maxAlpha)
                {
                    foundNon51 = true;
                    break;
                }
            }
            
            // FoundNon51 would be false if every ids are 51,51,51,51...
            if (!foundNon51)
            {
                isDone = true;
            }
            
            // Generate the tempKey using IDS
            for (int index = 0; index < arraySize; index++)
            {
                tempKey[index] = getCharAtID(ids[index]);
            }

            // Get Hash
            string tempHash = crypt(tempKey, salt);

            // Check against hash
            if (strlen(tempHash) == strlen(argv[1]))
            {
                bool isSame = true;
                for (int index = 0; index < strlen(tempHash); index++)
                {
                    if (tempHash[index] != argv[1][index])
                    {
                        isSame = false;
                        break;
                    }
                }

                if (isSame)
                {
                    printf("%s\n", tempKey);
                    return 0;
                }
            }
            
            if (!isDone)
            {
                // Increment the last index at ids
                ids[arraySize - 1]++;

                // Help convert [0, 52] into [1, 0];
                for (int idIndex = arraySize - 1; idIndex > 0; idIndex--)
                {

                    if (ids[idIndex] >= 52)
                    {
                        // Zero out that id, increment the previous index
                        ids[idIndex] = 0;
                        ids[idIndex - 1]++;
                    }

                }
            }
        }
    }
    
}

char getCharAtID(int charID)
{
    if (charID >= 0 && charID <= 25)
    {
        return 'A' + charID;
    }
    else if (charID >= 26 && charID <= maxAlpha)
    {
        int value = charID - 26;
        return 'a' + value;
    }
    else
    {
        return '\0';
    }
}
