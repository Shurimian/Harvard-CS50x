def main():
    cardStr = input("Number: ")
    cardNum = int(cardStr)

    if len(cardStr) < 13 or len(cardStr) > 16:
        print("INVALID")

    elif len(cardStr) == 13:
        if cardStr[0] == '4' and checkNum(cardStr):
            print("VISA")
        else:
            print("INVALID")

    elif len(cardStr) == 15:
        if checkNum(cardStr):
            if cardStr[0] == '3':
                if cardStr[1] in "47":
                    print("AMEX")
                else:
                    print("INVALID")
            else:
                print("INVALID")
        else:
            print("INVALID")

    elif len(cardStr) == 16:
        if checkNum(cardStr):
            if cardStr[0] == '4':
                print("VISA")
            elif cardStr[0] == '5':
                if cardStr[1] in "12345":
                    print("MASTERCARD")
                else:
                    print("INVALID")
            else:
                print("INVALID")
        else:
            print("INVALID")


def checkNum(cardStr):

    sum1 = 0
    sum2 = 0
    tempVal = 0

    for index in range(-1, -len(cardStr) - 1, -1):
        if index % 2 == 0:
            tempVal = int(cardStr[index]) * 2

            if tempVal < 10:
                sum1 += tempVal
            else:
                sum1 += 1 + (tempVal % 10)
        else:
            sum2 += int(cardStr[index])

    if (sum1 + sum2) % 10 == 0:
        return True
    else:
        return False


main()