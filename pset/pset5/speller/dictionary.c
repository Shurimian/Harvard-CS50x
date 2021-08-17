// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Used for strcasecmp (compares 2 strings regardless of case)
// LENGTH + 1 BECAUSE MUST INCLUDE TERMINATING CHARACTER
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int wordCount = 0;
bool loadCheck = false;

// Number of buckets in hash table (We are free to modify this value)
const unsigned int N = 1000000;

// Hash table
node *table[N];

// Checks if word is in dictionary. CASE INSENSITIVE. foo is same as FoO, etc...
bool check(const char *word)
{
    // Modify so word works with lowercase

    // char *lowerWord = malloc(strlen(word) + 1);
    // strcpy(lowerWord, word);

    // // Check for NULL
    // if (lowerWord == NULL)
    // {
    //     return false;
    // }

    // for (int i = 0; i < strlen(word); ++i)
    // {
    //     if ((int) lowerWord[i] <= 90 && (int) lowerWord[i] != 39)
    //     {
    //         lowerWord[i] = tolower(lowerWord[i]);
    //     }
    // }

    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp -> next)
    {
        if (strcasecmp(word, tmp -> word) == 0)
        {
            // free(lowerWord);
            return true;
        }
    }

    // free(lowerWord);
    return false;
}

// Hashes word to a number. Use this to index words
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int temp = 0;

    while ((temp = toupper(*word++)))
    {
        hash = ((hash << 5) + hash) + temp;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dicIn = fopen(dictionary, "r");
    char buffer[LENGTH + 1];
    int index = 0;

    if (dicIn == NULL)
    {
        return false;
    }

    while (fscanf(dicIn, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        ++wordCount;

        if (n == NULL)
        {
            return false;
        }

        index = hash(buffer);
        strcpy(n -> word, buffer);

        n -> next = table[index];

        table[index] = n;
    }

    fclose(dicIn);
    loadCheck = true;

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (loadCheck == true)
    {
        return wordCount;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int index = 0;

    while (index < N)
    {
        while (table[index] != NULL)
        {
            node *temp = table[index] -> next;
            free(table[index]);
            table[index] = temp;
        }

        ++index;
    }

    if (index == N)
    {
        return true;
    }

    return false;
}
