import os
import sys
import numpy as np
import time
import matplotlib.pyplot as plt

# Read data from a file
def read_data_from_file(filename):
    with open(filename, 'r') as file:
        data = [float(line.strip()) for line in file]
    return data

# Create histogram
def create_histogram(data, total_counts):
    plt.clf()  # Clear the previous plot
    plt.hist(data, bins=100, alpha=0.7, color='blue', edgecolor='black')
    plt.xlabel('Timestamp(us)')
    plt.ylabel('Counts')
    plt.title(f'Mass Spectrum - Total Counts: {total_counts}')
    plt.draw()  # Redraw the updated plot
    plt.pause(0.001)  # Pause to allow time for plot to be displayed

# Main function
def main():
    folder_path = 'C:/Users/Administrator/Documents/Diana/data_testing/Data/'  # Replace with your folder path
    refresh_interval = 1  # Time interval in seconds between checks
    
    # Check if runtime variable is provided as a command-line argument
    if len(sys.argv) < 5:
        print("Please provide a runtime variable as a command-line argument.")
        return
    
    voltage = int(sys.argv[1])  # Get the runtime variable from command-line argument
    trial  = int(sys.argv[2])
    time_s = int(sys.argv[3]) #time that the laser is on and data is registered in miliseconds
    group_us = int(sys.argv[4])
    file_name = sys.argv[5] #Name with which the figure will be saved
    plot_title = sys.argv[6]
    plot_title = plot_title.replace("_", " ")
    plot_saving_address = sys.argv[7]
    date_formated = sys.argv[8]
    

    processed_files = set()  # Initialize a set to store processed file names
    combined_data = []  # Initialize an empty list to store combined data
    total_counts = 0  # Initialize the total counts variable
    count = 0
    
   # while count < 10:
   #     files = [f for f in os.listdir(folder_path) if f.endswith('.txt') and f not in processed_files]
   #     
   #     if files:
   #         for filename in files:
   #             file_path = os.path.join(folder_path, filename)
   #             data = read_data_from_file(file_path)
   #             combined_data.extend(data)  # Combine new data with existing data
   #             total_counts += len(data)  # Update the total counts
   #             create_histogram(combined_data, total_counts)
   #             processed_files.add(filename)  # Mark the file as processed
   #     
   #     # Add runtime variable to the title
   #     plt.title(f'Mass Spectrum - Runtime: {voltage}s')
   #     
   #     time.sleep(refresh_interval)
   #     count += 1
    files = [f for f in os.listdir(folder_path) if f.endswith('.txt') and f not in processed_files]
    if files:
        for filename in files:
               file_path = os.path.join(folder_path, filename)
               data = read_data_from_file(file_path)
               combined_data.extend(data)  # Combine new data with existing data
               total_counts += len(data)  # Update the total counts
               #create_histogram(combined_data, total_counts)
               processed_files.add(filename)  # Mark the file as processed
#Save the figure at the end with the runtime variable in the filename
    plt.clf()  # Clear the plot before saving
    hist, bins, _ = plt.hist(combined_data, bins=100, alpha=0.7, color='blue', edgecolor='black')
    bin_size = bins[1] - bins[0]
    plt.xlabel('Timestamp(us)')
    plt.ylabel('Counts')
    plt.title(f'{plot_title}')
    plt.legend([f'Total Counts: {total_counts} '+f"t={time_s}s trial {trial} "+f"bin size = {bin_size:.{4}f}"])
    plt.savefig(f'{plot_saving_address}/{file_name}_group_{group_us}us_run_{time_s}s'+f'_trial_{trial}_date_{date_formated}.png')
    plt.show()
    
    
    
if __name__ == "__main__":
    main()
