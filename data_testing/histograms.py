import matplotlib.pyplot as plt
import time
import os
import math

#directory in which the files for the data stream are stored
directory = 'C:\Users\Administrator\Documents\Diana\data_testing'
#finds the second last file.
 #Since they are named after their creation time this automatically
 #means it is the second last created file.
 #the newest file is ignored, because C++ is currently writing in it.
def last_file():
    files = os.listdir(directory)
    Array_length = len(files)
    latest = files[Array_length - 2]
    return latest

 #it deletes all the files except...
 #... for the last one, because c++ is writing in it
 #.. for the second last one, because this is going to be read in
def last_file_delete():
    files = os.listdir(directory)
    last_file_position = len(files) - 2
    latest = files[last_file_position]
    position = 0
    while position < last_file_position:
    #print("position : " + str(position))
    #print("file :" + str(directory + files[position]))
        os.remove(directory + files[position])
        position += 1
    return latest

print("program started")

 #line for testing purposes. Later on it can be exchanged for endless loop
Loops = 30
startvalue = 0
endvalue = 2000
steps = 10
YAxislimit = 300
XAxislimit = endvalue + 10

for i in range(Loops):
 #if case in order to catch errors
 #(too long to fit into one line in pdf format)
    if open(directory + last_file(), "r").readable() and os.stat(directory+ last_file()).st_size != 0:
        #open the second last file and delete all older files.
        #The deletion keeps the files from piling up into infinity and
        #slowing down the read in of the file into an array
        with open(directory + last_file_delete()) as linefile:
        #Just for seeing it while programming. Can be deleleted later on
            print("Loop number " + str(i))
            #the stream file has only one line
            lineread = linefile.read()
             #converting from a string to a list of numbers
            RefreshFileRead = list(map(int, lineread[:-1].split(',')))
            #Formula to calculate the mass from the timestamp, the a is the 
            #constant from formula~\ref{eq:MassTime} and the b takes a possible lag 
            #between the card triggering and first pulses into consideration.
            #currently inactive, calibration is needed first:
            #b=100
            #a=10
            #RefreshFileRead = list(map(lambda x: a*math.sqrt(x+b)
            #,map(float, lineread[:-1].split(’,’))))

             #making the histogram and updating it in every cycle by drawing it new
            TOFBins = range(startvalue, endvalue, steps)
            plt.hist(RefreshFileRead, TOFBins, histtype='step')
            plt.xlabel('time in ns')
            plt.ylabel('number of Hits')
            plt.title('TOF Data')
            ax = plt.gca()
            ax.set_ylim([0,YAxislimit])
            ax.set_xlim([-10,XAxislimit])
            #The following 4 commands are in order to keep it updating and
            #keeping the program running.
            #plt.show() alone stops the program from continuing.
            plt.ion()
            plt.draw()
            plt.pause(0.01)
            #If the update rate is too fast for the human eye, insert a sleep
            time.sleep(0.1)
            plt.clf()
    else:
        time.sleep(0.01)
        print("fail")
    