#include <iostream>
#include "Matrix.hpp"



int main() {
    Matrix<double> m1;
    Matrix<double> m2;
    std::ifstream file;
    file.open("in.txt");

    size_t p;
    file >> p;
    Matrix<double>::SetParallel(p);
    m1.LoadMatrix(file);
    m2.LoadMatrix(file);





    return 0;
}
