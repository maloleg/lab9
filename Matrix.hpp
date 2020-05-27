#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <future>
#include <algorithm>

std::mutex m_lock;

template <typename T>
class Matrix;

template<typename T>
void ElementComputation(Matrix<T> lhs, Matrix<T> rhs, Matrix<T>& result, uint_fast64_t Element_n) {
    T temp = 0;
    for (uint_fast64_t i = 0; i < lhs.cols_get(); i++){
        //std::cout << (Element_n / rhs.cols) * lhs.cols + i << " " << Element_n % rhs.cols + i*rhs.cols << std::endl;
        temp += lhs.a_get_elem((Element_n / rhs.cols_get()) * lhs.cols_get() + i) * rhs.a_get_elem(Element_n % rhs.cols_get() + i*rhs.cols_get());
    }//ne sure
    m_lock.lock();
    //result.Done[Element_n] = true;
    result.a_set_elem(Element_n, temp);
    m_lock.unlock();
}

template <typename T>
Matrix<T> operator* (const Matrix<T>& lhs, const Matrix<T>& rhs){
    if (lhs.cols != rhs.rows) throw std::invalid_argument("You cannot multiply these matrices");
    else{
        Matrix<T>::parallel = 5; // иначе оно обнуляется, почему
        std::cout << "Number of threads is " << Matrix<T>::parallel << "\n";
        std::vector<T> multiplied_matrix;
        Matrix<T> temp;
        std::vector<std::thread> threads;
        std::vector<bool> Done;
        uint_fast64_t rows = lhs.rows;
        uint_fast64_t cols = rhs.cols;
        std::cout << "Number of threads is " << Matrix<T>::GetParallel() << "\n"; //оно обнуляется, почему так
        threads.resize(5);
        temp.Done.resize(rows * cols, false);
        temp.rows_set(rows);
        temp.cols_set(cols);
        temp.a_resize(rows * cols);
        Done.resize(rows * cols, false);

        //std::cout << Matrix<T>::parallel;
//        if (Done[Done.size() - 1] == false)
//            std::cout << "123123123123123123123";
        multiplied_matrix.resize(rows * cols);


        while (Done[Done.size() - 1] == false){

//            for (const auto& j : Done){
//                std::cout << j << " ";
//            }
//            std::cout << std::endl;


            for (size_t i = 0; i < Matrix<T>::GetParallel(); i++){
                uint_fast64_t j = 0;
                j = 0;
                while (Done[j] == true){
                    j++;
                }
                //std::cout << "\nj=" << j << "\n";
                Done[j] = true;
                threads[i] = std::thread(ElementComputation<T>, lhs, rhs, std::ref(temp), j);
            }

            //std::cout << "Number of threads is " << Matrix<T>::parallel << "\n"; //оно обнуляется, почему так... нахуй?(
            for (size_t i = 0; i < Matrix<T>::GetParallel(); i++){
                if (threads[i].joinable())
                    threads[i].join();
            }
        }
        return temp;
    }

}

template <typename T>
class Matrix{
private:
    uint_fast64_t rows, cols;
    std::vector<T> a;
    static size_t parallel;
    std::vector<bool> Done;
public:

    Matrix(){
        a.clear();
        rows = 0;
        cols = 0;
        //parallel = 0;
    }

    uint_fast64_t cols_get(){
        return this->cols;
    }

    uint_fast64_t rows_get(){
        return this->rows;
    }


    T a_get_elem(uint_fast64_t numb){
        return a[numb];
    }

    void a_resize(uint_fast64_t size){
        a.resize(size);
    }

    void rows_set(T data){
        rows = data;
    }

    void cols_set(T data){
        cols = data;
    }

    void a_set_elem(uint_fast64_t numb, T data){
        a[numb] = data;
    }

    friend void Parallels_init();

    Matrix(std::vector<T> new_m, uint_fast64_t new_rows, uint_fast64_t new_cols){
        a = new_m;
        rows = new_rows;
        cols = new_cols;
    }

    static void SetParallel(size_t num);

    static size_t GetParallel() {return parallel;}

    void LoadMatrix(const char * filename);

    void LoadMatrix(std::ifstream& mat);

    friend Matrix<T> operator *<T>(const Matrix& lhs, const Matrix& rhs);
};

template <typename T>
void Matrix<T>::LoadMatrix(std::ifstream& mat) {
    mat >> rows >> cols;
    T x = 0;
    a.clear();
    for (size_t i = 0; i < rows * cols; i++){
        mat >> x;
        a.push_back(x);
    }
}

template <typename T>
void Matrix<T>::LoadMatrix(const char * filename) {
    std::ifstream mat;
    mat.open(filename);
    mat >> rows >> cols;
    T x = 0;
    a.clear();
    for (size_t i = 0; i < rows * cols; i++){
        mat >> x;
        a.push_back(x);
    }
}

template<typename T>
void Matrix<T>::SetParallel(size_t num) {parallel = num;}

template <typename T>
void Parallels_init() {
    Matrix<T>::SetParallel(0);
}




