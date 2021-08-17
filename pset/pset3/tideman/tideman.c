#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;

    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Cycles through the candidates

    for (int i = 0; i < candidate_count; ++i)
    {
        // Check for name match

        if (strcmp(name, candidates[i]) == 0)
        {
            // rank value will range from [0, candidate_count - 1]
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Cycles through candidate 1
    for (int i = 0; i < candidate_count; ++i)
    {
        // Cycles through candidate 2
        for (int ii = i; ii < candidate_count; ++ii)
        {
            if (i != ii)
            {
                ++preferences[ranks[i]][ranks[ii]];
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;

    for (int i = 0; i < candidate_count; ++i)
    {
        for (int ii = 0; ii < candidate_count; ++ii)
        {
            if (preferences[i][ii] > preferences[ii][i] && i != ii)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = ii;
                ++pair_count;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int tempMax = 0;
    int winnerIndex = 0;
    int counter = 0;
    int tempWinner;
    int tempLoser;

    while (counter < candidate_count)
    {
        for (int i = 0; i < candidate_count; ++i)
        {
            for (int ii = 0; ii < candidate_count; ++ii)
            {
                if (tempMax < abs(preferences[i][ii] - preferences[ii][i]) && preferences[i][ii] > preferences[ii][i] && i != ii)
                {
                    tempMax = abs(preferences[i][ii] - preferences[ii][i]);
                    winnerIndex = i;
                }
                else if (tempMax < abs(preferences[i][ii] - preferences[ii][i]) && preferences[i][ii] < preferences[ii][i] && i != ii)
                {
                    tempMax = abs(preferences[i][ii] - preferences[ii][i]);
                    winnerIndex = ii;
                }
            }
        }

        for (int i = 0; i < candidate_count; ++i)
        {
            if (winnerIndex == i)
            {
                tempWinner = pairs[i].winner;
                tempLoser = pairs[i].loser;

                pairs[i].winner = pairs[counter].winner;
                pairs[i].loser = pairs[counter].loser;

                pairs[counter].winner = tempWinner;
                pairs[counter].loser = tempLoser;

                ++counter;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < candidate_count; ++i)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        if (i == candidate_count - 1 && locked[pairs[i].winner][pairs[i].loser] == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    printf("%s\n", candidates[pairs[0].winner]);
    return;
}

