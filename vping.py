import subprocess

add_list = ["66.112.221.61","c19s1.jamjams.net","c19s2.jamjams.net","c19s3.jamjams.net","c19s4.jamjams.net","c19s5.jamjams.net"]
processes = []

#for address in add_list:
#	processes.append(subprocess.Popen(["ping",address], stdout=subprocess.PIPE))
for address in add_list:
	subprocess.call(["ping",address])