import sys

if len(sys.argv) < 2:
    print("missing arguments")
else:
    filename = sys.argv[1]
    f = open(filename, mode='rb')
    
    index = 0
    print "{",
    for data in f.read():
        print "0x"+ data.encode('hex') + ",",
        index = index + 1
        if index % 10 == 0:
            print ""
    print "}"
