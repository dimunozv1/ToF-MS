import tkinter as tk
from tkinter import ttk, filedialog
from tkinter import*
import subprocess

# Default parameter values
DEFAULT_FREQUENCY = "10"
DEFAULT_DATA_ACQUISITION = "40"
DEFAULT_RUN_TIME = "20"
DEFAULT_TRIAL = "0"
DEFAULT_FIGURE_NAME = "Figure 1"
DEFAULT_PLOT_NAME = "Title"
DEFAULT_DATA_FOLDER = "C:/Users/Administrator/Documents/Diana/Run_TOF-MS_programs/Data"
DEFAULT_FIGURE_FOLDER = "C:/Users/Administrator/Documents/Diana/Run_TOF-MS_programs/Figures"
global target_folder
global source_file
global figure_location
def run_makefile():
    # Capture user input and generate a configuration file here
    freq_value = freq_entry.get() if freq_entry.get() else DEFAULT_FREQUENCY
    data_acquisition_value = Data_acquistion_entry.get() if Data_acquistion_entry.get() else DEFAULT_DATA_ACQUISITION
    run_time_value = run_time_entry.get() if run_time_entry.get() else DEFAULT_RUN_TIME
    trial_value = trial_entry.get() if trial_entry.get() else DEFAULT_TRIAL
    figure_name = figure_name_entry.get() if figure_name_entry.get() else DEFAULT_FIGURE_NAME
    figure_name = figure_name.replace(" ", "_")
    plot_name = plot_name_entry.get() if plot_name_entry.get() else DEFAULT_FIGURE_NAME
    plot_name = plot_name.replace(" ", "_")
    #source_file = source_file_entry.get()
    target_folder = target_folder_entry.get() if target_folder_entry.get() else DEFAULT_DATA_FOLDER
    figure_location = figure_location_entry.get() if figure_location_entry.get() else DEFAULT_FIGURE_FOLDER
    # Create a configuration file (e.g., config.txt) with these values
    with open("C:\\Users\\Administrator\\Documents\\Diana\\Run_TOF-MS_programs\\Debug\\config.txt", "w") as config_file:
        config_file.write(f"time_s = {run_time_value}\n")
        config_file.write(f"MEASURING_WINDOW_TIME_us = {data_acquisition_value}\n")
        config_file.write(f"FREQ_HZ = {freq_value}\n")
        config_file.write(f"TRIAL = {trial_value}\n")
        # Add more parameters to the configuration file as needed
        # Add more parameters to the configuration file as needed, e.g., file paths
        #config_file.write(f"SOURCE_FILE = {source_file}\n")
        config_file.write(f"TARGET_FOLDER = {target_folder}\n")
        config_file.write(f"FIGURE_NAME = {figure_name}\n")
        config_file.write(f"FIGURE_LOCATION = {figure_location}\n")
        config_file.write(f"PLOT_NAME = {plot_name}\n")
    
    # Run the Makefile with the generated configuration file
    makefile_command = ["mingw32-make"]
    subprocess.run(makefile_command, shell=True, cwd="C:\\Users\\Administrator\\Documents\\Diana\\Run_TOF-MS_programs\\Debug")
def browse_source_file():
    
    source_file = filedialog.askopenfilename()
    #source_file_entry.delete(0, tk.END)
    #source_file_entry.insert(0, source_file)

def browse_target_folder():
    target_folder = filedialog.askdirectory()
    #target_folder = target_folder.replace(" ", "_")
    target_folder_entry.delete(0, tk.END)
    target_folder_entry.insert(0, target_folder)

def browse_figure_location():
    figure_location = filedialog.askdirectory()
    #figure_location = figure_location.replace(" ", "_")
    figure_location_entry.delete(0, tk.END)
    figure_location_entry.insert(0, figure_location)

# Create a Tkinter window
window = tk.Tk()
window.title("TOF-MS Controller")
# Set the window icon
window.iconbitmap("C:/Users/Administrator/Documents/Diana/Run_TOF-MS_programs/Debug/Icon.ico")



# Create input fields and labels for parameters
freq_label = tk.Label(window, text="Frequency (Hz):")
freq_entry = tk.Entry(window)

Data_acquistion_label = tk.Label(window, text="Data acquisition window (us):")
Data_acquistion_entry = tk.Entry(window)

run_time_label = tk.Label(window, text="Program run time (s):")
run_time_entry = tk.Entry(window)

trial_label = tk.Label(window, text="Trial:")
trial_entry = tk.Entry(window)

target_folder_label = tk.Label(window, text="Output Data saving destination:")
target_folder_entry = tk.Entry(window)
browse_target_button = tk.Button(window, text="Browse", command=browse_target_folder)

figure_name_label = tk.Label(window, text="Output Figure File Name:")
figure_name_entry = tk.Entry(window)

figure_location_label = tk.Label(window, text="Output Figure saving destination:")
figure_location_entry = tk.Entry(window)
browse_figure_location_button = tk.Button(window, text="Browse", command=browse_figure_location)

plot_name_label = tk.Label(window, text="Plot Title:")
plot_name_entry = tk.Entry(window)

# Set default values
freq_entry.insert(0, DEFAULT_FREQUENCY)
Data_acquistion_entry.insert(0, DEFAULT_DATA_ACQUISITION)
run_time_entry.insert(0, DEFAULT_RUN_TIME)
trial_entry.insert(0, DEFAULT_TRIAL)
figure_name_entry.insert(0, DEFAULT_FIGURE_NAME)
plot_name_entry.insert(0, DEFAULT_PLOT_NAME) 


# Add more input fields and labels for other parameters here

#source_file_label = tk.Label(window, text=" Data Source File:")
#source_file_entry = tk.Entry(window)
#browse_source_button = tk.Button(window, text="Browse", command=browse_source_file)


 



run_button = tk.Button(window, text="Run TOF-MS", command=run_makefile)

# Layout the widgets using grid or pack
freq_label.grid(row=0, column=0)
freq_entry.grid(row=0, column=1)
Data_acquistion_label.grid(row=1, column=0)
Data_acquistion_entry.grid(row=1, column=1)
run_time_label.grid(row=2, column=0)
run_time_entry.grid(row=2, column=1)
# Add more widgets here
trial_label.grid(row=3, column=0)
trial_entry.grid(row=3, column=1)
#source_file_label.grid(row=3, column=0)
#source_file_entry.grid(row=3, column=1)
#browse_source_button.grid(row=3, column=2)
target_folder_label.grid(row=4, column=0)
target_folder_entry.grid(row=4, column=1)
browse_target_button.grid(row=4, column=2)
figure_name_label.grid(row=5, column=0)
figure_name_entry.grid(row=5, column=1)
figure_location_label.grid(row=6, column=0)
figure_location_entry.grid(row=6, column=1)
browse_figure_location_button.grid(row=6, column=2)
plot_name_label.grid(row=7, column=0)
plot_name_entry.grid(row=7, column=1)


run_button.grid(row=8, columnspan=2)

window.mainloop()
