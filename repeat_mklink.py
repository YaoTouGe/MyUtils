import os,sys
import subprocess

argNum = len(sys.argv)
if argNum < 4:
    print "missing arguments"
else:
    repeatTime = int((sys.argv[1]))
    src = sys.argv[3]
    targetName = sys.argv[2]

    for i in range(repeatTime):
        currentDir = targetName + str(i)
        os.mkdir(currentDir)
        subDir = os.path.join(currentDir, "road")
        os.mkdir(subDir)
        
        for dirname, dirs, files in os.walk(os.path.join(src, "road")):
            for file in files:
                name, ext = os.path.splitext(file)
                cmd = "mklink " + os.path.join(subDir, currentDir + ext) + " " + os.path.join(dirname, file)
                print cmd
                os.system(cmd)