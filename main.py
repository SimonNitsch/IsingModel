import numpy as np
import os
import matplotlib.pyplot as plt
from Functions import *

print("Helooa")
import sys

# Get the command-line arguments
arguments = sys.argv

# Check if there are any arguments
if len(arguments) > 1:
    # The first argument (index 0) is the script filename, so we access the second argument (index 1)
    input_argument = arguments[1]


#Energy = loadEMFromFile("EnergyFile")
#print(Energy)


folder_name  = arguments[1]  # Specify the name of the folder
print(folder_name)
# Get the current working directory
current_dir = os.getcwd()
# Create the path to the folder in the working directory
folder_path = os.path.join(current_dir, folder_name)
# Change the working directory
new_dir = folder_path
os.chdir(new_dir)


List_of_all_arrays = loadFiles(folder_path)
plot_and_update(List_of_all_arrays,0.2)




Energy_data = load_EM_values("Energyfile.bin")
plot_EM_values(Energy_data)

Autocorrelation = np.fromfile("Autocorrelation.bin", dtype=np.double)
Autocorrelation = Autocorrelation.flatten()
print(Autocorrelation)
plot_auto(Autocorrelation)

