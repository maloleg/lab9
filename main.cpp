#include "Matrix.hpp"

template <typename T>
size_t Matrix<T>::parallel = 0;

int main() {
    Matrix<double> m1;
    Matrix<double> m2;

    Parallels_init<double>();
    std::ifstream file;
    file.open("in.txt");

    size_t p;
    file >> p;
//    Matrix<double>::SetParallel(p);
    Matrix<double>::SetParallel(p);
    m1.LoadMatrix(file);
    m2.LoadMatrix(file);

    m1 = m2 * m1;





    return 0;
}
