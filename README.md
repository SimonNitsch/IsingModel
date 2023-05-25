# IsingModel

This is is a Metropolis Simulation of the Ising Model.
The inverse temperature beta and the amount of sweeps performed can be adjusted at runtime.
J can be also adjusted (but currently only at compile time), but it was not tested for values other than 1.
The size of the Lattice (L) has to be known at compile time, because the Lattice is initialized as std::array.
The main.exe file here is compiled with L = 16, using clang++ 12.0.0 on Windows for x64 architectures.

The Lattice_Data_xxx folders here are for Final_Plot.py.
While main.py is automatically executed in main.cpp and Functions.py contains the functions main.py uses,
Final_Plot.py and the Lattice_Data_xxx folders outside of this pipeline and aren't necessary for main.cpp to run.
So, Final_Plot.py needs to be executed separately.

The necessary files for compilation are:
main.cpp,
Lattice.cpp,
Lattice.h,
main.py,
Functions.py.

For compilation on Windows I would recommend clang++,
as the latest available g++ version ported to Windows (8.1.0) has several bugs in the filesystem.h file,
leading to errors when including the std::filesystem library.

The requirements for Python are version 3, as main.cpp will execute a python3 command in the shell.

