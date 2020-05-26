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

template<typename U>
void ElementComputation(Matrix<U> lhs, Matrix<U> rhs, Matrix<U> result, uint_fast64_t Element_n) {
    U temp = 0;
    for (uint_fast64_t i = 0; i < lhs.cols; i++){
        //std::cout << (Element_n / rhs.cols) * lhs.cols + i << " " << Element_n % rhs.cols + i*rhs.cols << std::endl;
        temp += lhs.a[(Element_n / rhs.cols) * lhs.cols + i] * rhs.a[Element_n % rhs.cols + i*rhs.cols];
    }//ne sure
    m_lock.lock();
    result.Done[Element_n] = true;
    result.a[Element_n] = temp;
    m_lock.unlock();
}

template <typename T>
Matrix<T> operator* (const Matrix<T>& lhs, const Matrix<T>& rhs){
    if (lhs.cols != rhs.rows) throw std::invalid_argument("You cannot multiply these matrices");
    else{
        std::vector<T> multiplied_matrix;
        Matrix<T> temp;
        std::vector<std::thread> threads;
        std::vector<bool> Done;
        uint_fast64_t rows = lhs.rows;
        uint_fast64_t cols = rhs.cols;
        threads.resize(Matrix<T>::parallel);
        temp.Done.resize(rows * cols, false);


        multiplied_matrix.resize(rows * cols);
        while (std::find(temp.Done.begin(), temp.Done.end(), false) != temp.Done.end()){
            for (size_t i = 0; i < Matrix<T>::parallel; i++){
                uint_fast64_t j = 0;
                while (temp.Done[j] == true){
                    j++;
                }
                temp.Done[j] = true;
                threads[i] = std::thread(ElementComputation, lhs, rhs, temp, j);
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
        parallel = 0;
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

    template <typename U>
    friend void ElementComputation(Matrix<U> lhs, Matrix<U> rhs, Matrix<U> result, uint_fast64_t Element_n);
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





