-include config.txt
#For how long will the system run in s
time_s ?= 20
time_ms := $(shell python -c "print(str(int(int($(time_s))*1000))))

# Get the current date and time in the format YYYYMMDD_HHMMSS
CURRENT_DATETIME := $(shell powershell Get-Date -Format "yyyyMMdd_HHmmss")
#For how long will data be registered after each pulse (ex: measure the first 40us after each pulse)

MEASURING_WINDOW_TIME_us ?= 40

#Frequency of the start
FREQ_HZ ?= 10


#Recording window time in bins, 1bin=500ps
GROUP_SIZE_BINS := $(shell python -c "print(str(int((int($(MEASURING_WINDOW_TIME_us))/(500e-6))))))

#How many packets of data will be recieved
PACKETS := $(shell python -c "print(str(int($(time_s))*int($(FREQ_HZ)))))


#Bool. Changing it to 1 prints an append file that helps debug the program
VERBOSE = 0 

TRIAL? = 0

#Location of the python program for making histograms
py_histograms = C:\Users\Administrator\Documents\Diana\Run_TOF-MS_programs\Debug\histogram.py





# Replace slashes with backslashes for Windows paths
SOURCE_FILE = C:\Users\Administrator\Documents\Diana\data_testing\Data\outfile_freq_$(FREQ_HZ)_t_$(GROUP_SIZE_BINS)_Packs_$(PACKETS).txt

#Where to store the .txt file at the end
TARGET_FOLDER ?= C:\Users\Administrator\Documents\Diana\Run_TOF-MS_programs\Data

#Beginning of the name of the figure file
FIGURE_NAME ?= Figure_1

#This location is sent to python to save the figures
FIGURE_LOCATION ?= C:/Users/Administrator/Documents/Diana/data_testing/Figures/Test/

#Final name of the data file.txt
NEW_FILENAME = $(FIGURE_NAME)_group_$(MEASURING_WINDOW_TIME_us)us_run_$(time_s)s_trial_$(TRIAL)_date_$(CURRENT_DATETIME).txt

#This is the title of the plot. Always use underscores instead of space, the plot will print without the underscores
PLOT_NAME ?= Title
.PHONY: Triggering
.PHONY: Histogram
.PHONY: Move

# Target to move and rename the file
move_and_rename: Triggering Histogram 
	cmd /c move $(subst /,\,$(SOURCE_FILE)) "$(subst /,\,$(TARGET_FOLDER))\$(NEW_FILENAME)"


Histogram: 
	python $(py_histograms) 600 $(TRIAL) $(time_s) $(MEASURING_WINDOW_TIME_us) $(FIGURE_NAME) $(PLOT_NAME) $(FIGURE_LOCATION) $(CURRENT_DATETIME)

Triggering: bnc525.ps1
	start /B powershell -File "C:\Users\Administrator\Documents\Diana\Run_TOF-MS_programs\Debug\bnc525.ps1" $(time_ms)
	start /WAIT cmd /C "C:\Users\Administrator\Documents\Diana\xtdc_babel-main\lib\x64\Debug\get_data.exe" $(FREQ_HZ) $(GROUP_SIZE_BINS) $(PACKETS) 0 $(VERBOSE)
	echo Exit Status: %errorlevel%

# Clean target to remove the moved file from the target folder
clean:
	cmd /c del $(subst /,\,$(TARGET_FOLDER))\$(NEW_FILENAME)
