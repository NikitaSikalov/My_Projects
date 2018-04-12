//
// Created by sikalovnikita on 12.04.18.
//

#ifndef MY_PROJECTS_MATRIX_H
#define MY_PROJECTS_MATRIX_H

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <stdlib.h>

using namespace std;

template <typename T>
class SquareMatrix;

class MatrixAllocationError {
};

class MatrixWrongSizeError {
};

class MatrixIndexError {
};

class MatrixIsDegenerateError {
};

template<typename T> T getZero() {
    return T(0);
}

template<typename T> T getOne() {
    return T(1);
}

template <typename T>
class Matrix {
private:
    int rowCnt;
    int colsCnt;
    T **array;
protected:
    void free_memory();
    void asign(const Matrix<T>&);
    void new_matrix(int n = 1, int m = 1);
    void change_value(int i, int j, T x) {this->array[i][j] = x; };
    T get_value(int i, int j) const {return this->array[i][j]; };
    void swap_strings(int i, int j);
public:
    Matrix(int n = 0, int m = 0);
    Matrix(const Matrix<T> &that) {this->asign(that);};
    Matrix& operator=(const Matrix<T>&);
    virtual ~Matrix();

    template <typename C>
    friend istream& operator>>(istream &is, Matrix<C> &that);
    template <typename C>
    friend ostream& operator<<(ostream &os, const Matrix<C> &that);

    Matrix operator+(const Matrix<T> &that) const;
    Matrix operator-(const Matrix<T> &that) const;
    Matrix operator*(const Matrix<T> &that) const;
    Matrix operator*(T) const;
    template <typename D>
    friend Matrix<D> operator*(D, const Matrix<D>&);

    Matrix& operator*=(T);
    Matrix& operator*=(const Matrix&);
    Matrix& operator-=(const Matrix<T> &that);
    Matrix& operator+=(const Matrix<T> &that);

    Matrix& transpose();
    Matrix getTransposed() const;
    T operator()(int i , int j) const;

    int getRowsNumber() const {return this->rowCnt; };
    int getColumnsNumber() const {return this->colsCnt; };
};

template <typename T>
class SquareMatrix: public Matrix<T> {
public:
    SquareMatrix(int n) {this->new_matrix(n, n);};
    SquareMatrix(const Matrix<T> &that) {this->asign(that); };
    SquareMatrix& operator=(const SquareMatrix &that);

    template <typename C>
    friend istream& operator>>(istream &is, SquareMatrix<C> &that);
    template <typename C>
    friend ostream& operator<<(ostream &os, const SquareMatrix<C> &that);

    SquareMatrix operator+(const SquareMatrix<T> &that) const;
    SquareMatrix operator-(const SquareMatrix<T> &that) const;
    SquareMatrix operator*(const SquareMatrix<T> &that) const;
    SquareMatrix operator*(T) const;
    SquareMatrix& operator*=(T k);
    SquareMatrix& operator*=(const SquareMatrix &that);
    SquareMatrix& operator-=(const SquareMatrix<T> &that);
    SquareMatrix& operator+=(const SquareMatrix<T> &that);
    T operator()(int i , int j) const;

    template <typename D>
    friend SquareMatrix<D> operator*(D, const SquareMatrix<D>&);

    int getSize() const {return this->getRowsNumber(); };
    T getDeterminant() const;
    T getTrace() const;
    T getMinor(int i, int j) const;

    SquareMatrix& transpose();
    SquareMatrix getTransposed() const;
    SquareMatrix getInverse() const;
    SquareMatrix<T>& invert();
};

//----------------------------------------------------------------------------------------------------------------------

template <typename T>
void Matrix<T>::new_matrix(int n, int m){
    this->rowCnt = n;
    this->colsCnt = m;
    this->array = new T*[n];
    for (int i = 0; i < n; ++i) {
        this->array[i] = new T[m];
    }
}

template <typename T>
Matrix<T>::Matrix(int n, int m) {
    new_matrix(n, m);
}

template <typename T>
void Matrix<T>::asign(const Matrix<T> &m) {
    this->new_matrix(m.rowCnt, m.colsCnt);
    this->rowCnt = m.rowCnt;
    this->colsCnt = m.colsCnt;
    for (int i = 0; i < rowCnt; ++i) {
        memcpy(this->array[i], m.array[i], colsCnt * sizeof(T));
    }
}

template <typename T>
void Matrix<T>::free_memory() {
    this->colsCnt = 0;
    this->rowCnt = 0;
    for (int i = 0; i < this->rowCnt; ++i) {
        delete []this->array[i];
    }
    delete []this->array;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &that) {
    if(this == &that){
        return *this;
    }
    this->free_memory();
    this->asign(that);
    return (*this);
}

template <typename T>
Matrix<T>::~Matrix() {
    this->free_memory();
}

template <typename T>
void Matrix<T>::swap_strings(int i, int j){
    T *temp_row = this->array[i];
    this->array[i] = this->array[j];
    this->array[j] = temp_row;
    for(int col = 0; col < this->colsCnt; ++col){
        this->array[i][col] *= -1;
    }
}

template <typename C>
istream& operator>>(istream &is, Matrix<C> &matrix) {
    for (int row = 0; row < matrix.rowCnt; ++row) {
        for (int col = 0; col < matrix.colsCnt; ++col) {
            is >> matrix.array[row][col];
        }
    }
    return is;
}

template <typename C>
ostream& operator<<(ostream &os, const Matrix<C> &matrix) {
    for (int row = 0; row < matrix.rowCnt; ++row) {
        for (int col = 0; col < matrix.colsCnt; ++col) {
            os << matrix.array[row][col] << ' ';
        }
        os << '\n';
    }
    return os;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &that) const{
    if(this->colsCnt != that.colsCnt || this->rowCnt != that.rowCnt){
        throw MatrixWrongSizeError();
    }
    Matrix result(this->rowCnt, that.colsCnt);
    for (int row = 0; row < this->rowCnt; ++row) {
        for (int col = 0; col < this->colsCnt; ++col) {
            result.array[row][col] = this->array[row][col] + that.array[row][col];
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &that) const{
    if(this->colsCnt != that.colsCnt || this->rowCnt != that.rowCnt){
        throw MatrixWrongSizeError();
    }
    Matrix result(this->rowCnt, that.colsCnt);
    for (int row = 0; row < this->rowCnt; ++row) {
        for (int col = 0; col < this->colsCnt; ++col) {
            result.array[row][col] = this->array[row][col] - that.array[row][col];
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &that) const {
    if(this->colsCnt != that.rowCnt){
        throw MatrixWrongSizeError();
    }
    Matrix result(this->rowCnt, that.colsCnt);
    if (this->colsCnt == that.rowCnt) {
        for (int i = 0; i < this->rowCnt; ++i) {
            for (int j = 0; j < that.colsCnt; j++) {
                T res = 0;
                for (int n = 0; n < that.rowCnt; ++n) {
                    res += this->array[i][n] * that.array[n][j];
                }
                result.array[i][j] = res;
            }
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T k) const{
    Matrix<T> result(this->rowCnt, this->colsCnt);
    for (int i = 0; i < this->rowCnt; ++i) {
        for (int j = 0; j < this->colsCnt; ++j) {
            result.array[i][j] = this->array[i][j] * k;
        }
    }
    return result;
}

template <typename D>
Matrix<D> operator*(D k, const Matrix<D> &m){
    Matrix<D> result = m * k;
    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(T k) {
    *this = *this * k;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix &that){
    *this = *this * that;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T> &that) {
    *this = *this + that;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T> &that) {
    *this = *this - that;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose(){
    *this = this->getTransposed();
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::getTransposed() const {
    Matrix<T> result(this->colsCnt, this->rowCnt);
    for(int i = 0; i < this->colsCnt; ++i){
        for(int j = 0; j < this->rowCnt; ++j){
            result.array[i][j] = this->array[j][i];
        }
    }
    return result;
}

///---------------------------------------------------------------------------------------------------------------------

template <typename T>
T SquareMatrix<T>::operator()(int i , int j) const{
    if(i >= this->getRowsNumber() || j >= this->getColumnsNumber()){
        throw MatrixIndexError();
    }
    return this->get_value(i, j);
};

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator=(const SquareMatrix<T> &that){
    if(this == &that){
        return *this;
    }
    this->free_memory();
    this->asign(that);
    return (*this);
}

template<typename C>
istream &operator>>(istream &is, SquareMatrix<C> &that) {
    for (int row = 0; row < that.getRowsNumber(); ++row) {
        for (int col = 0; col < that.getColumnsNumber(); ++col) {
            C temp;
            is >> temp;
            that.change_value(row, col, temp);
        }
    }
    return is;
}

template<typename C>
ostream &operator<<(ostream &os, const SquareMatrix<C> &that) {
    for (int row = 0; row < that.getRowsNumber(); ++row) {
        for (int col = 0; col < that.getColumnsNumber(); ++col) {
            os << that(row, col) << ' ';
        }
        os << '\n';
    }
    return os;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator+(const SquareMatrix<T> &that) const{
    Matrix<T> temp = *this;
    return temp + that;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator-(const SquareMatrix<T> &that) const{
    Matrix<T> temp = *this;
    return temp - that;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator*(const SquareMatrix<T> &that) const{
    Matrix<T> temp = *this;
    return temp * that;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::operator*(T k) const{
    Matrix<T> temp = *this;
    return temp * k;
}

template <typename D>
SquareMatrix<D> operator*(D k, const SquareMatrix<D> &that){
    return that * k;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator-=(const SquareMatrix<T> &that) {
    *this = *this - that;
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator+=(const SquareMatrix<T> &that) {
    *this = *this + that;
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator*=(T k) {
    *this = *this * k;
    return *this;
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::operator*=(const SquareMatrix<T> &that) {
    *this = *this * that;
    return *this;
}


template <typename T>
T SquareMatrix<T>::getDeterminant() const{
    T ans = 1;
    SquareMatrix<T> temp = *this;
    for(int col = 0; col < temp.getColumnsNumber(); ++col){
        int row = col;
        while(row < temp.getRowsNumber() && temp.get_value(row, col) == 0){
            row++;
        }
        if(row == temp.getRowsNumber()){
            return 0;
        }
        if(row != col){
            temp.swap_strings(row, col);
        }
        ans *= temp(col, col);
        for(int j = col + 1; j < temp.getRowsNumber(); ++j){
            T k = temp(j, col) / temp(col, col);
            for(int in = col; in < temp.getColumnsNumber(); ++in){
                T x = temp(j, in) - k * temp(col, in);
                temp.change_value(j, in, x);
            }
        }
    }
    return ans;
}

template <typename T>
T SquareMatrix<T>::getMinor(int i, int j) const {
    SquareMatrix<T> temp(this->getSize() - 1);
    int row = 0;
    for(int k = 0; k < this->getSize(); ++k){
        if(k != i){
            int col = 0;
            for(int p = 0; p < this->getSize(); ++p){
                if(p != j){
                    temp.change_value(row, col, this->get_value(k, p));
                    col++;
                }
            }
            row++;
        }
    }
    return temp.getDeterminant();
}

template <typename T>
T SquareMatrix<T>::getTrace() const{
    T ans = 0;
    for(int i = 0; i < this->getRowsNumber(); ++i){
        ans += this->get_value(i, i);
    }
    return ans;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::getInverse() const{
    SquareMatrix<T> ans(this->getSize());
    for(int row = 0; row < this->getSize(); ++row){
        for(int col = 0; col < this->getSize(); ++col){
            ans.change_value(row, col, this->getMinor(row, col));
            if((row + col) % 2 == 1){
                ans.change_value(row, col, -ans(row, col));
            }
        }
    }
    ans.transpose();
    T det = this->getDeterminant();
    if(det == 0){
        throw MatrixIsDegenerateError();
    }
    ans *= 1 / this->getDeterminant();
    return ans;

}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::invert(){
    *this = this->getInverse();
    return *this;
}

template <typename T>
SquareMatrix<T> SquareMatrix<T>::getTransposed() const {
    Matrix<T> temp = *this;
    return temp.getTransposed();
}

template <typename T>
SquareMatrix<T>& SquareMatrix<T>::transpose(){
    *this = this->getTransposed();
    return *this;
}

///---------------------------------------------------------------------------------------------------------------------

#endif //MY_PROJECTS_MATRIX_H
