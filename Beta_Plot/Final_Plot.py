import numpy as np
import matplotlib.pyplot as plt
import os

folder = "Lattice_Data_"

E4 = np.zeros(6)
M4 = np.zeros(6)
E8 = np.zeros(6)
M8 = np.zeros(6)
E16 = np.zeros(6)
M16 = np.zeros(6)

for j in [4, 8, 16]:
    for i in range(1,7):

        EM = np.fromfile(os.path.join(folder+str(j)+"0"+str(i), "Energyfile.bin"))
        EM = EM.reshape(-1,4)
        E4[i-1] = EM[-1,0]
        M4[i-1] = abs(EM[-1,2])
        print("Read file number %s" %i)

    plt.figure()
    plt.subplot(2,1,1)
    plt.plot(np.arange(6)/10 +0.1, E4)
    plt.xlabel("beta")
    plt.ylabel("Energy")
    plt.title("Energy and Magn. for L = %s" %j)
    plt.subplot(2,1,2)
    plt.plot(np.arange(6)/10 +0.1, M4)
    plt.xlabel("beta")
    plt.ylabel("Absolute Magnetization")
    plt.show()




