// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a stack of nodes
// Allows for an algorithm to free up all memory occured by a dictionary
typedef struct nodestack
{
    node *currentNode;
    struct nodestack *nextInStack;
}
nodestack;

// Represents a trie
node *root;

// For stack structure
nodestack *stacktop = NULL;
int nodestackSize = 0;

// Represents the size of the dictionary
int dictionarySize = 0;

// Get an index according to Char
int indexAccordingToChar(char a)
{
    // Lower alpha
    if (a >= 'a' && a <= 'z')
    {
        return a - 'a';
    }
    // Upper alpha
    else if (a >= 'A' && a <= 'Z')
    {
        return a - 'A';
    }
    // Apostrophe
    else if (a == 39)
    {
        return N - 1;
    }
    // Bad char
    else
    {
        return -1;
    }

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Re-init dictionary size
    dictionarySize = 0;

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Current word attribute
        node *currentNode = root;
        int wordLength = strlen(word);

        // Going over each character of word
        for (int i = 0; i < wordLength; i++)
        {
            int index = indexAccordingToChar(word[i]);

            if (index != -1)
            {
                // If the space doesn't exist yet, create one
                if (currentNode->children[index] == NULL)
                {
                    currentNode->children[index] = malloc (sizeof(node));
                    currentNode->children[index]->is_word = false;

                    // Init that new array
                    for (int j = 0; j < N; j++)
                    {
                        currentNode->children[index]->children[j] = NULL;
                    }
                }

                // Go to the next index
                currentNode = currentNode->children[index];

                if (i == wordLength - 1)
                {
                    // At the last character, toggle the truth value of is_word and exit loop
                    currentNode->is_word = true;
                    dictionarySize++;
                    break;
                }
            }
            else if (index == -1)
            {
                // When it reaches this instance, it encountered a non-alpha/apostrophe char; ignore word
                break;
            }

        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionarySize;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int wordLength = strlen(word);
    bool isWord = true;
    node *currentnode = root;

    for (int i = 0; i < wordLength; i++)
    {
        int index = indexAccordingToChar(*(word+i));

        // Stop word iterator when encountered unindexable char, and proceed to check if word at place
        if (index < 0)
        {
            break;
        }

        if (currentnode->children[index] == NULL)
        {
            isWord = false;
            break;
        }
        else
        {
            // Go deeper in trie
            currentnode = currentnode->children[index];
        }

    }

    if (isWord == true)
    {
        isWord = currentnode->is_word;
    }

    return isWord;
}

struct node* push(node* nodetoPush)
{
    nodestack *newTop = malloc(sizeof(nodestack));
    if (newTop == NULL)
    {
        return NULL;
    }

    newTop->currentNode = nodetoPush;
    newTop->nextInStack = stacktop;

    stacktop = newTop;
    nodestackSize++;
    return stacktop -> currentNode;
}

struct node* pop()
{
    nodestack *top = stacktop;

    if (stacktop == NULL)
    {
        return NULL;
    }

    stacktop = stacktop->nextInStack;
    node *nodeAtTop = top->currentNode;

    free(top);
    nodestackSize--;
    return nodeAtTop;
}

struct node* top()
{
    if (stacktop != NULL)
    {
        return stacktop->currentNode;
    }
    else
    {
        return NULL;
    }
}

void printStack()
{
    nodestack *currentStackNode = stacktop;
    while(currentStackNode != NULL)
    {
        printf("There was a node in stack: %d\n", currentStackNode->currentNode->is_word);
        currentStackNode = currentStackNode->nextInStack;
    }

    printf("Stack size: %d\n", nodestackSize);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    push(root);

    if(dictionarySize == 0 || root == NULL)
    {
        return false;
    }

    while (nodestackSize != 0)
    {
        node *topNode = pop();
        bool hasChildren = false;

        for (int i = 0; i < N; i++)
        {
            node *child = topNode->children[i];

            if (child != NULL)
            {
                push(child);
            }
        }

        free(topNode);
    }

    return true;
}
