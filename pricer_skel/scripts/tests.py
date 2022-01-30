
# import required module
import os
import subprocess
# assign directory
directory = '.'
 
ignore = ["./testVanilleGtest"]
# iterate over files in
# that directory
for filename in os.listdir(directory):
    
    f = os.path.join(directory, filename)
    
    # checking if it is a file
    if os.path.isfile(f) and f[0:6] == "./test" and not f in ignore:
        print(f)
        bashCommand = f
        process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
        output, error = process.communicate()
        print(output.decode("utf-8"))