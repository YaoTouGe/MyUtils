import sys, os

argvNum =  len(sys.argv)

if argvNum < 2:
    print "missing argument!"
else:
    outFile = open(sys.argv[2], 'w')
    isFirstLine = True

    for dirpath, dirnames, filenames in os.walk(sys.argv[1]):
        for fname in filenames:
            if fname.endswith(".csv"):
                file = open(fname, 'r')
                # first line is header
                header = file.readline()
                if isFirstLine:
                    isFirstLine = False
                    outFile.write(header)
                    print "header is "+ header
                outFile.writelines(file.readlines())
                file.close()

    outFile.close()