#pragma once

#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <exception>
#include <memory.h>
#include <limits>

class Matrix {
    int num_rows;
    int num_cols;

    double * values;

    public: 
        Matrix();
        Matrix(const int & rows, const int & cols);
        Matrix(const double & init_value, const int & rows, const int & cols);
        Matrix(const double * matrix, const int & rows, const int & cols);
        Matrix(const Matrix & right);
        Matrix(Matrix && right);
        Matrix(std::initializer_list<std::initializer_list<double>> list);

        ~Matrix();
        
        Matrix & operator = (const Matrix & right);

        Matrix & operator = (Matrix && right);

        Matrix && operator = (const std::initializer_list<std::initializer_list<double>> & list);
        
        const double * getValues() const;

        // Matrix Multiplication 
        Matrix operator * (const Matrix & right) const;

        // Matrix Addition
        Matrix operator + (const Matrix & right) const;

         // Matrix Subtraction
        Matrix operator - (const Matrix & right) const;

        Matrix row (const int & row) const;
        Matrix col (const int & col) const;
        
        int rows() const;
        int columns() const;

        int rows(const double * matrix) const;
        int columns(const double * matrix) const;
        
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

Matrix Identity(const int & size);

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