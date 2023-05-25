#include "Lattice.h"


template<const std::size_t L>
Lattice<L>::Lattice(double J, double beta, int number_of_output_files_){
    srand(time(0));
    this->J = J;
    this-> number_output_files = number_of_output_files_;

    this->digits_of_output_fil_number = findDigits(number_of_output_files_);
    this->beta = beta;


    createFolder(folderPath);


    Energy_vec = new std::vector<double>;
    Magnet_vec = new std::vector<int>;
    L2 = L * L;
    L_int = (int) L;

    for (int i = 0; i < L_int; i++) {
        for (int j = 0; j < L_int; j++) {
            Lat[i][j] = static_cast<bool>(rand() % 2);
        }
    }
    Energy = total_energy();
    Magnetization = total_magnetization();
    std::cout << "Energy: " << Energy << "\n";

    for (int i = 0; i < 5; i++) {
        e_table[i] = std::exp(-beta * 2 * J * (2 * i - 4));
    }
}



/*
//std::array<double, 4> Lattice::run(long long measurements)
std::vector<std::array<double, 4>> Lattice::run(long long measurements)
{
    Energy_vec->reserve(measurements + 1);
    Magnet_vec->reserve(measurements + 1);
    Energy_vec->emplace_back(Energy);
    Magnet_vec->emplace_back(Magnetization);

    std::vector<std::array<double, 4>> EM_values; // List to store EM values

    long long perc = measurements;
    for (long long i = 0; i < measurements; i++) {
        sweep();

        // output sometime
        if (i * 40 >= perc) {
            std::cout << (i * 100) / measurements << " % \n";
            perc += measurements;
            saveLatticeToFile((i * 100) / measurements);
            //saveArrayToFile();
        }

        std::array<double, 4> EM;
        int i_start = Energy_vec->size() / 5;
        EM[0] = mean(*Energy_vec, i_start) / static_cast<double>(L2);
        EM[1] = std::pow(var(*Energy_vec, i_start), 0.5) / static_cast<double>(L2);
        EM[2] = mean(*Magnet_vec, i_start) / static_cast<double>(L2);
        EM[3] = std::pow(var(*Magnet_vec, i_start), 0.5) / static_cast<double>(L2);

        EM_values.push_back(EM); // Store EM values in the list

        saveEMToFile(EM_values,Engergy_file_name);
    }

    return EM_values;
}*/


//std::array<double, 4> Lattice::run(long long measurements)

template<const std::size_t L>
void Lattice<L>::run(long long measurements)
{
    Energy_vec->reserve(measurements + 1);
    Magnet_vec->reserve(measurements + 1);
    Energy_vec->emplace_back(Energy);
    Magnet_vec->emplace_back(Magnetization);

    std::vector<std::array<double, 4>>* EM_values = new std::vector<std::array<double, 4>>; // List to store EM values
    EM_values->reserve(measurements+1);
    std::array<double, 4> EM;
    EM[0] = Energy/static_cast<double>(L2);
    EM[1] = 0;
    EM[2] = static_cast<double>(Magnetization)/static_cast<double>(L2);
    EM[3] = 0;
    EM_values->push_back(EM);
    double sq_E_mean = std::pow(Energy, 2);
    double sq_M_mean = std::pow(Magnetization, 2);
    double E_mean = Energy;
    double M_mean = Magnetization;

    long long perc = measurements;
    long long saved_files = 0;
    for (long long i = 0; i < measurements; i++) {
        sweep();

        // output sometime the %
        if (i * (40) >= perc) {
            std::cout << (i * 100) / measurements << " %" << " - Av. Energy: " << (*EM_values)[i][0] << " - Av. Magnetization: " << (*EM_values)[i][2] << "\n";
            perc += measurements;
            print_2dvector_Color(Lat);
        }

        // save the currend Lattice to file
        if (i  >= (measurements/number_output_files) * saved_files) {
            saveLatticeToFile((i * std::pow(10, digits_of_output_fil_number)) / measurements);
            saved_files += 1;

        }

        sq_E_mean = running_mean(Energy_vec, i+1, sq_E_mean, true);
        sq_M_mean = running_mean(Magnet_vec, i+1, sq_M_mean, true);
        E_mean = running_mean(Energy_vec, i+1, E_mean, false);
        M_mean = running_mean(Magnet_vec, i+1, M_mean, false);
        
        EM[0] = E_mean / static_cast<double>(L2);
        EM[1] = std::sqrt(sq_E_mean - std::pow(E_mean,2)) / static_cast<double>(L2);
        EM[2] = M_mean / static_cast<double>(L2);
        EM[3] = std::sqrt(sq_M_mean - std::pow(M_mean,2)) / static_cast<double>(L2);

        EM_values->push_back(EM); // Store EM values in the list


    }
    saveEMToFile(EM_values,Engergy_file_name);

    double* tim = new double;
    double* tim2 = new double;
    std::vector<double>* aut = autocorrelation(Energy_vec, 1000, tim);
    std::vector<double>* aut2 = autocorrelation(Magnet_vec, 1000, tim2);
    saveVecToFile(aut,Autocorr_file_name);
    double tvar = std::sqrt(std::abs(true_var(Energy_vec, (*tim))));
    double tvar2 = std::sqrt(std::abs(true_var(Magnet_vec, (*tim2))));
    char pm = 241;



    std::string log_file_name = folderPath + "/INFO.txt";
    saveLogFile(log_file_name,std::to_string(measurements));
    std::cout << "\n \n" << "Energy: " << EM_values->back()[0] << " " << pm << " " << tvar << "\n";
    std::cout << "Magnetization: " << EM_values->back()[2] << " " << pm << " " << tvar2 << "\n \n";
    std::cout << "Autocorrelation Time for Energy: " << (*tim) << "\n";
    std::cout << "Autocorrelation Time for Magnetization: " << (*tim2) << "\n \n";
    //return EM_values;
}



template<const std::size_t L>
void Lattice<L>::print_2dvector_Color(std::array<std::array<bool, L>, L> v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < (v[i]).size(); j++) {
            if (v[i][j] == 1) {
                std::cout << "\x1B[48;5;40m\x1B[38;5;15m"; // dark green background, white foreground
            } else {
                std::cout << "\x1B[48;5;196m\x1B[38;5;15m"; // red background, white foreground
            }
            std::cout << v[i][j] << " ";
            std::cout << "\x1B[0m"; // reset colors
        }
        std::cout <<"\n";
    }
}


// For printing the lattice
template<const std::size_t L>
void Lattice<L>::print_2dvector(std::array<std::array<bool, L>, L> v)
{
    for (int i = 0; i < v.size(); i++){
        for (int j = 0; j < (v[i]).size(); j++)
        {
            std::cout << v[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}






template<const std::size_t L>
void Lattice<L>::saveEMToFile(std::vector<std::array<double, 4>>* EM_values, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    //std::ostringstream oss;
    //std::string filename = folderPath + "/lattice_" + oss.str() + ".bin";

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    std::vector<std::array<double, 4>> EM_saved;
    EM_saved.reserve(500);
    long long j = 0;
    long long EMsize = EM_values->size();
    for (long long i = 0; i < EMsize; i++){
        if(i*500 >= j*EMsize){
            EM_saved.push_back((*EM_values)[i]);
            j++;
        }
    }

    for (auto& EM : EM_saved) {
        file.write(reinterpret_cast<const char*>(EM.data()), EM.size() * sizeof(double));
    }

    file.close();
    std::cout << "Energy file saved as" << filename << std::endl;
}

template<const std::size_t L>
void Lattice<L>::saveVecToFile(std::vector<double>* vec, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& v : (*vec)) {
        file.write(reinterpret_cast<const char*>(&v), sizeof(double));
    }

    file.close();
    std::cout << "Energy file saved as" << filename << std::endl;
}


template<const std::size_t L>
void Lattice<L>::saveLatticeToFile(int percent)
{
    std::ostringstream oss;
    oss << std::setw(digits_of_output_fil_number) << std::setfill('0') << percent; // Pad the percentage value with leading zeros
    std::string filename = folderPath + "/lattice_" + oss.str() + ".bin";

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    int rows = Lat.size();
    int cols = Lat[0].size();

    // Write the dimensions of the lattice
    file.write(reinterpret_cast<const char*>(&rows), sizeof(int));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(int));

    // Write the lattice data
    for (const auto& row : Lat) {
        for (bool value : row) {
            int intValue = value ? 1 : 0;
            file.write(reinterpret_cast<const char*>(&intValue), sizeof(int));
        }
    }

    file.close();
}

/*
void Lattice::saveArrayToFile()
{


    int rows = Lat.size();
    int cols = Lat[0].size();

    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        // Write the dimensions of the Lat
        file.write(reinterpret_cast<const char*>(&rows), sizeof(int));
        file.write(reinterpret_cast<const char*>(&cols), sizeof(int));

        // Write the Lat data
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                bool value = Lat[i][j];
                file.write(reinterpret_cast<const char*>(&value), sizeof(bool));
            }
        }

        std::cout<< "File Saved"<< std::endl;

        file.close();
    }
}
*/
template<const std::size_t L>
template<typename T>
std::vector<double>* Lattice<L>::autocorrelation(std::vector<T>* x, int tmax, double* tau) {
    double v = var(x);
    double m = mean(x);
    double m2 = m * m;
    int xsize = x->size();
    std::vector<double>* aut_vec = new std::vector<double>;
    aut_vec->reserve(tmax);
    double aut;

    if (tau){
        *tau = 0.5;
    }

    for (int t = 0; t < tmax; t++) {
        T aut_sum = 0;
        for (int i = 0; i < (xsize - t); i++) {
            aut_sum += (*x)[i] * (*x)[i + t];
        }
        aut = (static_cast<double>(aut_sum) / static_cast<double>(xsize - t) - m2) / v;
        aut_vec->emplace_back(aut);
        if (tau) {
            *tau += aut * (1 - t / xsize);
            }
    }
    return aut_vec;
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::true_var(std::vector<T>* x, double tau){
    double sig2_N = var(x) / x->size();
    double tvar = 2 * tau * sig2_N;
    return tvar; 
}

template<const std::size_t L>
int Lattice<L>::cycle(int x) const {
    if (x >= L_int) {
        return (x - L_int);
    }
    if (x < 0) {
        return (x + L_int);
    }
    return x;
}

template<const std::size_t L>
int Lattice<L>::get_prob(int r) {
    int x = r % L_int;
    int y = r / L_int;
    bool spin = Lat[x][y];

    int index = 0;

    index += static_cast<int>(spin == Lat[cycle(x + 1)][y]);
    index += static_cast<int>(spin == Lat[cycle(x - 1)][y]);
    index += static_cast<int>(spin == Lat[x][cycle(y + 1)]);
    index += static_cast<int>(spin == Lat[x][cycle(y - 1)]);

    return index;
}

template<const std::size_t L>
double Lattice<L>::total_energy() {
    int ss = 0;

    for (int i = 0; i < L2; i++) {
        ss -= 2 * static_cast<int>(Lat[i % L_int][i / L_int] == Lat[cycle(i % L_int - 1)][i / L_int]) - 1;
        ss -= 2 * static_cast<int>(Lat[i % L_int][i / L_int] == Lat[i % L_int][cycle(i / L_int - 1)])-1;
    }

    return J * static_cast<double>(ss);
}

template<const std::size_t L>
int Lattice<L>::total_magnetization()
{
    int spin = 0;
    for (int i = 0; i < L2; i++) {
        spin += static_cast<int>(Lat[i % L_int][i / L_int]);
    }
    return 2 * spin - L2;
}

template<const std::size_t L>
void Lattice<L>::sweep() {
    for (int i = 0; i < L2; i++)
    {
        int r = rand() % L2;
        int index = get_prob(r);
        double prob = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

        if (prob > e_table[index]) {
            continue;
        }
        Lat[r % L_int][r / L_int] = !Lat[r % L_int][r / L_int];
        Energy += 2 * J * (2 * index - 4);
        Magnetization += 2 * (2 * static_cast<int>(Lat[r % L_int][r / L_int]) - 1);
    }
    Energy_vec->emplace_back(Energy);
    Magnet_vec->emplace_back(Magnetization);
}

template<const std::size_t L>
std::string Lattice<L>::SetFilename() {
    std::string input;
    std::cout << "Name your Simulation: ";
    std::getline(std::cin, input);
    return input;
}



template<const std::size_t L>
void Lattice<L>::saveLogFile(const std::string& filename, const std::string& content)
{
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    file << folderPath << std::endl;
    file << "Lattice Size:  " << L  << "x" << L << std::endl;
    file << "Beta:          " << std::fixed << std::setprecision(2) << beta << std::endl;
    file << "Iterations:    " << content << std::endl;
    file.close();

}

template<const std::size_t L>
void Lattice<L>::createFolder(const std::string& folderName) {
    std::filesystem::path folderPath(folderName);

    if (std::filesystem::create_directory(folderPath)) {
        std::cout << "Folder created: " << folderName << std::endl;
    } else {
        std::cout << "Failed to create folder: " << folderName << std::endl;
    }
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::mean(std::vector<T>* vec) {
    double sum = 0;
    for (auto i = vec->begin(); i != vec->end(); i++) {
        sum += static_cast<double>(*i);
    }
    return sum / static_cast<double>(vec->size());
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::mean(std::vector<T>* vec, int start) {
    double sum = 0;
    for (int i = start; i < vec->size(); i++) {
        sum += static_cast<double>((*vec)[i]);
    }
    return sum / static_cast<double>(vec.size() - start);
}

template<const std::size_t L>
template<typename T>
std::vector<T>* Lattice<L>::vec_sq(std::vector<T>* vec) {
    std::vector<T>* sq = new std::vector<T>;
    sq->reserve(vec->size());
    for (auto i = vec->begin(); i != vec->end(); i++) {
        sq->emplace_back(*i * *i);
    }
    return sq;
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::var(std::vector<T>* vec) {
    double m = mean(vec);
    double m2 = mean(vec_sq(vec));
    return m2 - m * m;
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::var(std::vector<T>* vec, int start) {
    double m = mean(vec, start);
    double m2 = mean(vec_sq(vec), start);
    return m2 - m * m;
}

template<const std::size_t L>
double Lattice<L>::ex_value(std::vector<double> vec) {
    std::vector<double>* vec_exp = new std::vector<double>;
    vec_exp->reserve(vec.size());
    double Z = 0;
    double e;
    for (double& i : vec) {
        e = std::exp(-beta * i);
        Z += e;
        vec_exp->emplace_back(e);
    }
    double ex_val = 0;
    for (int i = 0; i < vec.size(); i++) {
        ex_val += vec[i] * vec_exp->operator[](i);
    }
    ex_val /= Z;
    delete vec_exp;
    return ex_val;
}

template<const std::size_t L>
int Lattice<L>::findDigits(int x) {

    // find digets
    int dig = 1;
    while(x/10 > 0)
    {
        x = x/10;
        dig +=1;
    }
    return dig;
}

template<const std::size_t L>
template<typename T>
double Lattice<L>::running_mean(std::vector<T>* vec, long long old_vec_size, double current_mean, bool squared){
    
    double new_mean;
    if (squared){
        double sum = std::round(static_cast<double>(old_vec_size) * current_mean / J / J) * J * J;
        new_mean = (sum + std::pow(static_cast<double>(vec->back()), 2)) / static_cast<double>(old_vec_size + 1);
        return(new_mean);
    } 
    
    double sum = std::round(static_cast<double>(old_vec_size) * current_mean / J) * J;
    new_mean = (sum + static_cast<double>(vec->back())) / static_cast<double>(old_vec_size + 1);
    return(new_mean);
}


