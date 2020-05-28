#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <future>
#include <algorithm>
#include <cstdlib>
#include <chrono>

std::mutex m_lock;

template <typename T>
class Matrix;

template <typename T>
void Time_testing(Matrix<T> first, Matrix<T> second, size_t thread1, size_t thread2){
    Matrix<T> result;
    uint_fast64_t time;
    std::ofstream Graph_points;
    Graph_points.open("Graph_points.txt");

    for (size_t i = thread1; i < thread2 + 1; i++){
        Matrix<T>::SetParallel(i);
        std::cout << "Testing with " << i << " threads:";
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        result = first * second;
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time << "ms\n";
        Graph_points << i << ";" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time << std::endl;
    }
    Graph_points.close();

}

template <typename T>
void Time_testing(Matrix<T> first, Matrix<T> second, size_t thread1){
    Matrix<T> result;
    uint_fast64_t time;

    Matrix<T>::SetParallel(thread1);
    std::cout << "Testing with " << thread1 << " threads:";
    time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    result = first * second;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - time << "ms\n";

}

template<typename T>
void ElementComputation(Matrix<T> lhs, Matrix<T> rhs, Matrix<T>& result, std::vector<bool>& Done, size_t threadN) {
    T temp = 0;
    uint_fast64_t Elements_count = 0;
//    m_lock.lock();
//    std::cout << "!@#  " << threadN << "\n";
//    m_lock.unlock();
    while (Done[Done.size() - 1] == false){
        temp = 0;

        uint_fast64_t element;
        m_lock.lock();
        for (uint_fast64_t i = 0; i < Done.size(); i++){
            //std::cout << i << std::endl;
            if (Done[i] == false){
                element = i;
                Done[i] = true;
                break;
            }
        }
        m_lock.unlock();
        for (uint_fast64_t i = 0; i < lhs.cols_get(); i++){
            //std::cout << (element / rhs.cols_get()) * lhs.cols_get() + i << " " << element % rhs.cols_get() + i*rhs.cols_get() << std::endl;
            temp += lhs.a_get_elem(((element)/ rhs.cols_get()) * lhs.cols_get() + i) * rhs.a_get_elem((element) % rhs.cols_get() + i*rhs.cols_get());
        }
        m_lock.lock();
        //std::cout << "elem =" << (element) << " data =" << temp << " Done =" << element << " Size =" << result.rows_get() * result.cols_get() << std::endl;
        m_lock.unlock();

        result.a_set_elem((element), temp);
        Elements_count++;
    }
    m_lock.lock();
    std::cout << "Thread #" << threadN + 1 << " ended his job. " << Elements_count << " elements was computed\n";
    m_lock.unlock();
}


template <typename T>
Matrix<T> operator* (const Matrix<T>& lhs, const Matrix<T>& rhs){
    if (lhs.cols != rhs.rows) throw std::invalid_argument("You cannot multiply these matrices");
    else{
        Matrix<T> temp;
        std::vector<std::thread> threads;
        threads.resize(Matrix<T>::GetParallel());
        temp.rows_set(lhs.rows);
        temp.cols_set(rhs.cols);
        temp.a_resize(lhs.rows * rhs.cols);
        std::vector<bool> Done;
        Done.resize(lhs.rows * rhs.cols, false);

        for (size_t i = 0; i < Matrix<T>::GetParallel(); i++){
            threads[i] = std::thread(ElementComputation<T>, lhs, rhs, std::ref(temp), std::ref(Done), i);
        }

            for (size_t i = 0; i < Matrix<T>::GetParallel(); i++){
                if (threads[i].joinable())
                    threads[i].join();
                //threads[i].detach();
            }
        return temp;
    }

}

template <typename T>
std::ostream& operator <<(std::ostream& out, const Matrix<T>& rhs){
    out << rhs.rows << " " << rhs.cols << std::endl;
    for (uint_fast64_t j = 0; j < rhs.rows; j++){
        for (uint_fast64_t i = 0; i < rhs.cols; i++){
            //std::cout << j * rhs.cols + i;
            out << rhs.a[j * rhs.cols + i];

            if (i != rhs.cols - 1){
                out << " ";
            }
        }
        if (j != rhs.rows - 1){
            out << std::endl;
        }
    }
    return out;
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
        if (numb < a.size()) return a[numb];
        else return -1;
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
        if (numb < a.size()) a[numb] = data;
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

    friend std::ostream& operator <<<T>(std::ostream& out, const Matrix& rhs);

    void SaveMatrix(const char* filename);

    void GenerateMatrix(uint_fast64_t _rows, uint_fast64_t _cols){
        srand(1234);
        this->a.resize(_rows * _cols);
        this->rows = _rows;
        this->cols = _cols;
        for (uint_fast64_t i = 0; i < rows * cols; i++){
            //std::cout << rand();
            this->a[i] = rand()%1000;
        }
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

template<typename T>
void Matrix<T>::SetParallel(size_t num) {
    parallel = num;
}

template <typename T>
void Parallels_init() {
    Matrix<T>::SetParallel(0);
}

template<typename T>
void Matrix<T>::SaveMatrix(const char *filename) {
    std::ofstream out;
    out.open(filename);
    out << this->rows << " " << this->cols << std::endl;
    for (uint_fast64_t j = 0; j < this->rows; j++){
        for (uint_fast64_t i = 0; i < this->cols; i++){
            out << this->a[j * this->cols + i];
            if (i != this->cols - 1){
                out << " ";
            }
        }
        if (j != this->rows - 1){
            out << std::endl;
        }
    }

    out.close();

}




