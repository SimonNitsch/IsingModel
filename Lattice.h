#ifndef LATTICE_H
#define LATTICE_H

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <typeinfo>

template<const std::size_t L>
class Lattice {
public:


    std::string folderPath = "Lattice_Data_" + SetFilename();

    std::string Engergy_file_name = folderPath +  "/Energyfile.bin";
    std::string Autocorr_file_name = folderPath + "/Autocorrelation.bin";

    int number_output_files = 10;

    int L2;
    double beta, J, Energy;
    int Magnetization;
    std::array<std::array<bool, L>, L> Lat;
    std::array<double, 5> e_table;
    std::vector<double>* Energy_vec;
    std::vector<int>* Magnet_vec;

    Lattice(double J, double beta, int number_of_output_files_);
    void run(long long measurements);



    //std::array<double, 4> run(long long measurements);
    //std::vector<std::array<double, 4>> run(long long measurements);


    void print_2dvector(std::array<std::array<bool, L>, L> v);
    void print_2dvector_Color(std::array<std::array<bool, L>, L> v);
    void saveEMToFile(std::vector<std::array<double,4>>* EM_values, const std::string& filename);
    void saveVecToFile(std::vector<double>* vec, const std::string& filename);



    void saveLatticeToFile(int percent);

    template<typename T>
    std::vector<double>* autocorrelation(std::vector<T>* x, int tmax, double* tau);
    template<typename T>
    double true_var(std::vector<T>* x, double tau);

    std::string SetFilename();
    void createFolder(const std::string& folderName);

    void saveLogFile(const std::string& filename, const std::string& content);



private:
    int cycle(int x) const;
    int get_prob(int r);
    double total_energy();
    int total_magnetization();
    void sweep();
    int L_int;

    template<typename T>
    double mean(std::vector<T>* vec);

    template<typename T>
    double mean(std::vector<T>* vec, int start);

    public:
    template<typename T>
    double running_mean(std::vector<T>* vec, long long old_vec_size, double current_mean, bool squared);

    template<typename T>
    std::vector<T>* vec_sq(std::vector<T>* vec);

    template<typename T>
    double var(std::vector<T>* vec);

    template<typename T>
    double var(std::vector<T>* vec, int start);

    double ex_value(std::vector<double> vec);

    int findDigits(int x);
    int digits_of_output_fil_number = 1;
};

#endif /* LATTICE_H */
