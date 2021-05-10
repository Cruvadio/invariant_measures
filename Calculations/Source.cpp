#include <iostream>
#include <fstream>
#include "symbolic_image.hpp"
#include <chrono>


int main()
{

    SymbolicImage s(-2, 2, -2, 2, 100, 0.00390625, 0, 100, -0.8, 0.2);

    auto t_start = std::chrono::high_resolution_clock::now();
    vector<double> measures = s.find_invariant_measures();
    auto t_end = std::chrono::high_resolution_clock::now();


    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

    cout << elapsed_time_ms << endl;

    ofstream myfile;

    cout << "Opening file..." << endl;
    myfile.open("balance.txt");

    for (int i = 0; i < measures.size(); i++)
    {
        if (measures[i] != 0)
        myfile << s.return_row(i) << " " << s.return_col(i) << " " << measures[i] << endl;
    }

    myfile.close();
    system("pause");
    return 0;
}