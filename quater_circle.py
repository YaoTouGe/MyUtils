import sys
import math

radius = 1.0
sampleNum = 10

if len(sys.argv) == 2:
    radius = int(sys.argv[1])
elif len(sys.argv) == 3:
    sampleNum = int(sys.argv[2])

radianDelta = math.pi * 0.5 / (sampleNum - 1)
rad = 0

for quaterId in range(4):
    print "quater", quaterId
    rad = quaterId * math.pi * 0.5
    x = round(radius * math.cos(rad))
    y = round(radius * math.sin(rad))
    print "{", x, ",", -y, "},"
    rad = rad + radianDelta

    for i in range(1, sampleNum - 1):
        x = radius * math.cos(rad)
        y = radius * math.sin(rad)

        print "{", x, ",", -y, "},"
        rad = rad + radianDelta
    
    rad = math.pi * 0.5 * (quaterId + 1)
    x = round(radius * math.cos(rad))
    y = round(radius * math.sin(rad))
    
    print "{", x, ",", -y, "},"
    rad = rad + radianDelta