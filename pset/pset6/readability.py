def main():

    text = input("Text: ").upper()
    wordCount = len(text.split(" "))
    sentenceCount = 0
    letterCount = 0

    for i in range(len(text)):
        if text[i] in ".!?":
            sentenceCount += 1
        elif ord(text[i]) >= 65 and ord(text[i]) <= 90:
            letterCount += 1

    L = letterCount / wordCount * 100
    S = sentenceCount / wordCount * 100
    index = int((0.0588 * L - 0.296 * S - 15.8) + 0.5)

    if index < 1:
        print(f"Before Grade 1")
    elif index > 16:
        print(f"Grade 16+")
    else:
        print(f"Grade {index}")


main()