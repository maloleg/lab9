#include "Matrix.hpp"

template <typename T>
size_t Matrix<T>::parallel = 0;

int main() {
    Matrix<double> m1;
    Matrix<double> m2;
    Matrix<double> m3;
    Matrix<double> m4;
    Matrix<double> m5;

    //Parallels_init<double>();
    //std::ifstream file;
    //file.open("in.txt");

    //size_t p;
    //file >> p;
 //   Matrix<double>::SetParallel(p);
    //m1.LoadMatrix(file);
    //m2.LoadMatrix(file);
    //m1.GenerateMatrix(20, 30);
    //m2.GenerateMatrix(30, 10);
    //Matrix<double>::SetParallel(10);

    //std::cout << m1 << std::endl << m2;

    //m3 = m1 * m2;

    //std::cout << m1 << std::endl << m2;
    //std::cout << m1 << std::endl << m2 << std::endl << m3;

    //m3.SaveMatrix("out1.txt");

    //std::cout << m1.rows_get()*m1.cols_get();


    m4.GenerateMatrix(100, 200);
    m5.GenerateMatrix(200, 500);
    //m4 = m4 * m5;
    //m4.SaveMatrix("Out_Big.txt");

    Time_testing(m4, m5, 1, 100);



    return 0;
}
