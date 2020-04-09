import os, sys, shutil

if len(sys.argv) != 3:
    print("missing arguments")
else:
    srcPath = sys.argv[1]
    dstPath = sys.argv[2]
    os.mkdir(dstPath)
    for dir, dirnames, filenames in os.walk(sys.argv[1]):
        for dirname in dirnames:
            os.mkdir(os.path.join(dstPath, dirname))
            os.mkdir(os.path.join(dstPath, dirname, "road"))
            for dummydir, dummydirnames, midmiffiles in os.walk(os.path.join(dirname, "road")):
                for midmiff in midmiffiles:
                    if midmiff.startswith("R_LName") or midmiff.startswith("R_Name") or midmiff.startswith("R"+dirname):
                        shutil.copy(os.path.join(dummydir, midmiff), os.path.join(dstPath, dirname, "road", midmiff))