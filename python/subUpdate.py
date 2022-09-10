import os
import sys
import subprocess

def traversalAndUpdate(rootPath):
    pathToUpdate = [rootPath]

    while len(pathToUpdate) > 0:
        path = pathToUpdate.pop()
        process = subprocess.run("svn update " + path)
        if process.returncode != 0:
            subprocess.run("svn cleanup " + path)
            for _, dirs, files in os.walk(path):
                for filePath in files:
                    subprocess.run("svn update " + filePath)
                for subDir in dirs:
                    if os.path.basename(subDir).startswith("."):
                        continue
                    # print("======================\nupdate "+ subDir + "\n=======================" )
                    process = subprocess.run("svn update " + subDir)
                    if process.returncode != 0:
                        subprocess.run("svn cleanup " + subDir)
                        pathToUpdate.append(subDir)
                break

def main():
    try:
        subprocess.run("svn help", stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except FileNotFoundError:
        print("请安装svn的命令行工具")
        return

    rootPath = "."
    if len(sys.argv) >= 2:
        rootPath = sys.argv[1]
    traversalAndUpdate(rootPath)

if __name__ == "__main__":
    main()