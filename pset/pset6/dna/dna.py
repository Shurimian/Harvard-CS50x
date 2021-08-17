from sys import argv
import csv


def main():

    if len(argv) != 3:
        print("Usage: python dna.py databases.csv sequences.txt")

    # Create a list of dictionaries
    database = []

    # Read database into memory
    with open(argv[1], "r") as fileIn:
        reader = csv.DictReader(fileIn)

        for row in reader:
            database.append(row)

    # Define number of STR sequences, and STR's.
    STR = list(database[0].keys())
    numSTR = len(STR)

    # Create string variable for DNA sequence
    sequence = " "

    # Read DNA into memory
    with open(argv[2], "r") as fileIn:
        sequence = fileIn.read()

    tempCounter = 0
    strCounter = {}

    # Initialize strCounter
    for seq in STR:
        strCounter[seq] = 0

    # Begin comparison
    for seq in STR[1:numSTR]:
        for index in range(len(sequence)):
            if sequence[index:(len(seq) + index)] == seq:
                index += len(seq)
                tempCounter += 1

                if seq != sequence[index:(len(seq) + index)]:
                    if strCounter[seq] < tempCounter:
                        strCounter[seq] = tempCounter
                        tempCounter = 0
                    else:
                        tempCounter = 0
        tempCounter = 0

    for person in database:
        for key in STR[1:numSTR]:
            if int(person[key]) == int(strCounter[key]):
                tempCounter += 1
        if tempCounter == (numSTR - 1):
            print(person[STR[0]])
            strCounter[STR[0]] = person[STR[0]]
        tempCounter = 0

    if strCounter[STR[0]] == 0:
        print("No Match")


main()