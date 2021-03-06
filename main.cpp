#include "Matrix.hpp"

template <typename T>
size_t Matrix<T>::parallel = 0;

int main() {
    Matrix<double> m1;
    Matrix<double> m2;
    Matrix<double> m3;
    Matrix<double> m4;
    Matrix<double> m5;

    std::ifstream file;
    file.open("in.txt");

    size_t p;
    file >> p;
    Matrix<double>::SetParallel(p);
    m1.LoadMatrix(file);
    m2.LoadMatrix(file);

    m3 = m1 * m2;

    //std::cout << m1 << std::endl << m2 << std::endl << m3;

    m3.SaveMatrix("out.txt");

    //std::cout << m1.rows_get()*m1.cols_get();


    m4.GenerateMatrix(100, 200, 1000);
    m5.GenerateMatrix(200, 500, 1000);

    //Matrix<double>::SetParallel(1);
    //m3 = m4 * m5;
    //m3.SaveMatrix("Out_Big.txt");
    //m4.SaveMatrix("Out_Big2.txt");
    //m5.SaveMatrix("Out_Big3.txt");

    Time_testing(m4, m5, 1, 10);



    return 0;
}
