#include <iostream>
#include <fstream>
#include "symbolic_image.hpp"

int main()
{
    cout << "Calculating..." << endl;
    SymbolicImage s(-2, 2, -2, 2, 100, 0.005, 0, 100, 0.5, -0.5);

    vector<double> measures = s.find_invariant_measures();
    cout << "Calculated!" << endl;
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