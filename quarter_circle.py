import sys
import math

radius = 1.0
sampleNum = 10

if len(sys.argv) == 2:
    radius = int(sys.argv[1])
elif len(sys.argv) == 3:
    sampleNum = int(sys.argv[2])

radianDelta = math.pi * 0.5 / sampleNum
rad = 0

for quarterId in range(4):
    rad = math.pi * 0.5 * quarterId
    x = math.cos(rad)
    y = math.sin(rad)
    print "{" + str(round(x)) + "f," +str(round(y)) + "f},"

    for i in range(sampleNum - 2):
        rad = rad + radianDelta
        x = math.cos(rad)
        y = math.sin(rad)
        print "{" + str(x) + "f," +str(y) + "f},"
    
    rad = math.pi * 0.5 * (quarterId + 1)
    x = math.cos(rad)
    y = math.sin(rad)
    print "{" + str(round(x)) + "f," +str(round(y)) + "f},"
    print