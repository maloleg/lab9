#include "Matrix.hpp"

template <typename T>
size_t Matrix<T>::parallel = 0;

int main() {
    Matrix<double> m1;
    Matrix<double> m2;
    Matrix<double> m3;
    Matrix<double> m4;
    Matrix<double> m5;

    Parallels_init<double>();
    std::ifstream file;
    file.open("in.txt");

    size_t p;
    file >> p;
 //   Matrix<double>::SetParallel(p);
    Matrix<double>::SetParallel(p);
   // std::cout << Matrix<double>::GetParallel();
//    m1.LoadMatrix(file);
//    m2.LoadMatrix(file);
//
//    m3 = m1 * m2;
//
//    std::cout << m1 << std::endl << m2 << std::endl << m3;
//
//    m3.SaveMatrix("out.txt");
//
//    std::cout << m1.rows_get()*m1.cols_get();


    m4.GenerateMatrix(10, 20);
    m5.GenerateMatrix(20, 50);
//    m4 = m4 * m5;
//    m4.SaveMatrix("Out_Big.txt");

    Time_testing(m4, m5, 1, 20);



    return 0;
}
