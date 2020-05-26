#pragma once
#include <vector>
#include <fstream>
#include <thread>

template <typename T>
class Matrix{
private:
    uint_fast64_t rows, cols;
    std::vector<T> a;
    static size_t parallel;
    static uint_fast64_t WhatElement;
public:

    Matrix(std::vector<T> new_m, uint_fast64_t new_rows, uint_fast64_t new_cols){
        a = new_m;
        rows = new_rows;
        cols = new_cols;
        WhatElement = 0;
    }

    static void SetParallel(size_t num) {parallel = num;}

    static size_t GetParallel() {return parallel;}

    void LoadMatrix(const char * filename);

    void LoadMatrix(std::ifstream& mat);

    friend Matrix<T>& operator *(const Matrix<T> lhs, const Matrix<T> rhs);

    friend T ElementComputation(Matrix<T> lhs, Matrix<T> rhs, uint_fast64_t Element_n){
        T temp = 0;
        for (uint_fast64_t i = 0; i < lhs.cols; i++){
            temp += lhs.a[Element_n / lhs.rows + i] * rhs[Element_n / lhs.cols + i*lhs.rows]
        }//ne sure
    }
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

template <typename T>
Matrix<T>& operator* (const Matrix<T> lhs, const Matrix<T> rhs){
    if (lhs.cols != rhs.rows) throw std::invalid_argument("You cannot multiply these matrices");
    else{
        std::vector<T> multiplied_matrix;
        uint_fast64_t rows = lhs.rows;
        uint_fast64_t cols = rhs.cols;



    }

}

