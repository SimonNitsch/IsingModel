#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <new>
#include <array>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>  // For system function
#include <limits>

#include "Lattice.h"
#include "Lattice.cpp"


void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int SetMesurments() {
    int input;
    std::cout << "Enter an the number of iteratations:   10^ ";
    std::cin >> input;
    clearInputBuffer();
    return input;
}


double estimateRunTime(int iterations)
{                               //0.00193718

    double empirical_time =    0.00167012;//0.084107;
    double estimatet_time = empirical_time * iterations;

    std::cout << "This will take approximatly "<< estimatet_time <<" seconds" << std::endl;
    std::cout << " Be carefule only a rought estimation" << std::endl;
    return estimatet_time;
}


double getBeta(double defaultValue) {
    double input;
    std::cout << "Enter a value for beta (default: " << defaultValue << "): ";

    // Read user input
    std::string userInput;
    std::getline(std::cin, userInput);

    // If the user input is empty, use the default value
    if (userInput.empty()) {
        return defaultValue;
    }

    // Convert the user input to a double
    try {
        input = std::stod(userInput);
    } catch (...) {
        std::cout << "Invalid input. Using the default value: " << defaultValue << std::endl;
        return defaultValue;
    }

    // Clear any remaining input from the buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return input;
}


int main()
{


    const size_t l = 8;
    double J = 1;
    double beta = getBeta(0.4);
    int iter_expo = SetMesurments();
    int iterations = std::pow(10, iter_expo);

    estimateRunTime(iterations);


    class Lattice<l> L(J,beta,100);



    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();
//print_2dvector(L.Lat);

    for (int i = 0; i < 5; i++){
        std::cout << L.e_table[i] << " ";
    }

    std::cout<< "\n";
    system("Pause");
    L.run(iterations);
    //std::array<double,4> EM = L.run(iterations);
    //std::vector<std::array<double, 4>> EM = L.run(iterations);
    //std::cout << "\n" << "Energy per Electron: " << EM[0] << " " << pm << " " << EM[1] << "\n";
    //std::cout << "Magnetization per Electron: " << EM[2] << " " << pm << " " << EM[3] << "\n \n";
    //system("Pause");
    system("Pause");


   // const std::string EnergyFileName= "EnergyFile";
    //saveEMToFile(EnergyFileName,EM);



    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;

    // Print the runtime
    std::cout << "Runtime: " << duration.count() << " seconds for "<< iterations <<" Iterations"<<  std::endl;

    // used to get empirical runtime
    double time_pro_it = duration.count() / iterations;
    std::cout << " That are " << time_pro_it<< "Seconds / iteration" << std::endl;
    std::cout << time_pro_it* iterations << std::endl;


    double est_time = estimateRunTime(iterations);




    // Execute the Python file using the system command
    std::string folderName = L.folderPath;
    std::string command = "python3 main.py " + folderName;


    // Uncommand in order to run the python part
    std::system(command.c_str());



    return 0;

}
