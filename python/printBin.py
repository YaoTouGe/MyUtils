import sys

def main():
    if len(sys.argv) < 2:
        print("missing arguments")
    else:
        printLineCount = -1
        if len(sys.argv) > 2:
            try:
                printLineCount = int(sys.argv[2])
            except ValueError:
                print("line count should be integer")

        filename = sys.argv[1]
        f = open(filename, mode='rb')
        
        index = 0
        lineNumer = 0
        print("{\n line0\t", end="")
        for data in f.read():
            print(hex(data), end="\t")
            index = index + 1
            if index % 10 == 0:
                lineNumer = lineNumer + 1
                if printLineCount > 0 and lineNumer >= printLineCount:
                    break
                print("")
                print(" line%s"%lineNumer , end="\t")
                
        print("\n}")

if __name__ == "__main__":
    main()
