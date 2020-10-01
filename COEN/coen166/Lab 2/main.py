from vacuum import roomba

def main():
    val1 = input("Enter 0 or 1: ")
    val2 = input("Enter 0 or 1: ")
    val3 = input("Enter 0 or 1: ")
    states = [int(val1), int(val2), int(val3)]

    obj = roomba(states, [])
    obj.check()
    print("Actions of the vacuum: ", obj.todo)
    print("Performance of the vacuum: ", obj.score)

if __name__ == '__main__':
    main()