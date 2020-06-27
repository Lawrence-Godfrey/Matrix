#pragma once

#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <exception>
#include <memory.h>
#include <limits>
#include <random>
#include <chrono>



class Matrix {
    unsigned int num_rows;
    unsigned int num_cols;

    double * values;

    public: 
        Matrix();
        Matrix(const unsigned int & rows, const unsigned int & cols);
        Matrix(const unsigned int & rows, const unsigned int & cols, const double & init_value);

        Matrix(const std::pair<unsigned int, unsigned int> & shape);
        Matrix(const std::pair<unsigned int, unsigned int> & shape, const double & init_val = 0);

        Matrix(const double * matrix, const unsigned int & rows, const unsigned int & cols);
        Matrix(const Matrix & right);
        Matrix(Matrix && right);
        Matrix(std::initializer_list<std::initializer_list<double>> list);

        ~Matrix();
        
        Matrix & operator = (const Matrix & right);

        Matrix & operator = (Matrix && right);

        Matrix & operator = (const std::initializer_list<std::initializer_list<double>> & list);
        

        double & operator () (const unsigned int & row, const unsigned int & column);
        double   operator () (const unsigned int & row, const unsigned int & column) const;
        double & operator () (const unsigned int & index);
        double   operator () (const unsigned int & index) const;
        
        // Matrix Multiplication 
        Matrix dot(const Matrix & right) const;

        // Hadamard Product
        Matrix multiply(const Matrix & right) const;

        // Matrix Addition
        Matrix operator + (const Matrix & right) const;

         // Matrix Subtraction
        Matrix operator - (const Matrix & right) const;

        const double * getValues() const;
        
        Matrix row (const unsigned int & row) const;
        Matrix col (const unsigned int & col) const;
        
        unsigned int rows() const;
        unsigned int columns() const;
        unsigned int size() const;
        std::pair<unsigned int, unsigned int> shape() const;

        void addRow(const Matrix & rowMatrix);
        void addColumn(const Matrix & columnMatrix);
        
        double max() const;
        double min() const;

        Matrix Transpose () const;

        friend std::ostream & operator << (std::ostream & os, const Matrix & matrix);
};

// Scale Matrix
Matrix operator * (const Matrix & left, const double & right);
Matrix operator * (const double & left, const Matrix & right);

// Scalar Addition
Matrix operator + (const Matrix & left, const double & right);
Matrix operator + (const double & left, const Matrix & right);

// Scalar Subtraction
Matrix operator - (const Matrix & left, const double & right);
Matrix operator - (const double & left, const Matrix & right);

bool operator == (const Matrix & left, const Matrix & right);

inline
double & Matrix::operator () (const unsigned int & row, const unsigned int & column) {
    return this->values[row * this->num_cols + column];
}

inline
double Matrix::operator () (const unsigned int & row, const unsigned int & column) const {
    return this->values[row * this->num_cols + column];
}

inline
double & Matrix::operator () (const unsigned int & index) {
    return this->values[index];
}

inline
double Matrix::operator () (const unsigned int & index) const {
    return this->values[index];
}

Matrix Identity(const unsigned int & size);

Matrix RandomMatrix(const unsigned int & rows, const unsigned int & columns);
Matrix RandomNormalMatrix(const unsigned int & rows, const unsigned int & columns, const double & mean = 0, const double & sigma = 1);


class MatrixDimensionError : public std::exception {
    public:
        const char * what () const throw () {
            return "Matrix Dimension Error";
        }
};

class MultiplyMatrixError : public MatrixDimensionError {
     public:
        const char * what () const throw () {
            return "Error Multiplying Matrices due to dimension error";
        }
};

class EmptyMatrixError : public MatrixDimensionError {
     public:
        const char * what () const throw () {
            return "Error: Empty Matrix";
        }
};