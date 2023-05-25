

import numpy as np
import os
import matplotlib.pyplot as plt
def load_lattice_file(filename):
    with open(filename, 'rb') as file:
        # Read the dimensions of the lattice
        rows = np.fromfile(file, dtype=np.int32, count=1)[0]
        cols = np.fromfile(file, dtype=np.int32, count=1)[0]

        # Read the lattice data
        lattice = np.fromfile(file, dtype=np.int32, count=rows * cols)
        lattice = lattice.reshape((rows, cols))

    return lattice
def loadFiles(folder_path):
    list = []
    files =[]
    # Loop over each file in the folder
    for filename in os.listdir(folder_path):
        if "Energy" in filename or "INFO" in filename or "Auto" in filename:
            continue
        files.append(filename)
    files.sort()
    files = sorted(files, key=str.lower)

    for filename in files:
        A = load_lattice_file(filename)
        list.append(A)
    return list


def plot_and_update(data_list, interval=0.5):
    # Create the initial plot with the first array
    fig, ax = plt.subplots()
    im = ax.imshow(data_list[0])

    # Show the plot
    plt.show(block=False)

    # Update the plot with each subsequent array
    for i in range(1, len(data_list)):
        # Update the image data
        im.set_array(data_list[i])

        # Pause to show the updated plot
        plt.pause(interval)

    # Turn off interactive mode
    plt.ioff()
    plt.show()

def load_EM_values(filename):
    # Load the binary file into a numpy array
    data = np.fromfile(filename, dtype=np.float64)

    # Reshape the data into a 2D array with shape (num_values, 4)
    EM_values = data.reshape(-1, 4)

    return EM_values





def plot_EM_values(EM_values):
    # Extract the individual arrays for Energy, Energy error, Magnetization, and Magnetization error
    energy = EM_values[:, 0]
    energy_error = EM_values[:, 1]
    magnetization = EM_values[:, 2]
    magnetization_error = EM_values[:, 3]
    errN = int(np.ceil(energy.size / 75))

    # Create the x-axis values based on the number of measurements
    x = np.arange(len(EM_values))
    # Plot Energy
    plt.figure(figsize=(10, 6))
    plt.subplot(2, 1, 1)
    plt.errorbar(x, energy, yerr=energy_error, fmt='b-', label='Energy',errorevery=errN)
    plt.xlabel('Measurements')
    plt.ylabel('Energy')
    plt.legend()

    # Plot Magnetization
    plt.subplot(2, 1, 2)
    plt.errorbar(x, magnetization, yerr=magnetization_error, fmt='r-', label='Magnetization',errorevery=errN)
    plt.xlabel('Measurements')
    plt.ylabel('Magnetization')
    plt.legend()

    # Adjust spacing between subplots
    plt.tight_layout()

    # Show the plot
    plt.show()


def plot_auto(Autocorrelation):

    plt.figure()
    x = np.arange(100)
    plt.semilogy(x,Autocorrelation[0:100])
    plt.show()
    



