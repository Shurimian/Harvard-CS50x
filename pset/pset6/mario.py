import cs50


def main():

    height = cs50.get_int("Enter an integer between 1-8 inclusive: ")

    while height < 1 or height > 8:
        height = cs50.get_int("Re-enter valid height: ")

    space = height - 1

    for i in range(height):
        print(" " * space, end="")
        print("#" * (height - space), end="")
        print("  ", end="")
        print("#" * (height - space))
        space -= 1


main()