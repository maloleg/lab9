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
 //   Matrix<double>::SetParallel(p);
    Matrix<double>::SetParallel(p);
   // std::cout << Matrix<double>::GetParallel();
    m1.LoadMatrix(file);
    m2.LoadMatrix(file);

    m1 = m1 * m2;

    //std::cout << m1.rows_get()*m1.cols_get();

    for (uint_fast64_t i = 0; i < m1.rows_get()*m1.cols_get(); i++){
        std::cout << m1.a_get_elem(i) << " !" << std::endl;
    }



    return 0;
}
