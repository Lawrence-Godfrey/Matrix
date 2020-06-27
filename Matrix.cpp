#include "Matrix.h"

Matrix::Matrix() : values(nullptr), num_cols(0), num_rows(0) {}

Matrix::Matrix(const unsigned int & rows, const unsigned int & cols) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {}

Matrix::Matrix(const unsigned int & rows, const unsigned int & cols, const double & init_value) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {
    for (unsigned int i = 0; i < num_cols * num_rows; i++) {
        values[i] = init_value;
    }
}

Matrix::Matrix(const std::pair<unsigned int, unsigned int> & shape) : Matrix(shape.first, shape.second) {}

Matrix::Matrix(const std::pair<unsigned int, unsigned int> & shape, const double & init_value) : Matrix(shape.first, shape.second) {
    for (unsigned int i = 0; i < num_cols * num_rows; i++) {
        values[i] = init_value;
    }
}

Matrix::Matrix(const std::vector<std::vector<double>> & vec) : Matrix(vec.size(), vec.at(0).size()) {
    int i = 0;
    for (auto & row : vec) {
        for (auto & value : row) {
            values[i++] = value;
        }
    }
}

Matrix::Matrix(const double * matrix, const unsigned int & rows, const unsigned int & cols) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {  
    memcpy(values, matrix, sizeof(values[0]) * num_rows * num_cols);
}


Matrix::Matrix(const Matrix & right) : num_cols(right.columns()), num_rows(right.rows()), values(new double [num_rows * num_cols]) {   
    std::cout << "in copy constructor" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;
    memcpy(values, right.values, sizeof(values[0]) * num_rows * num_cols);
}

Matrix::Matrix(Matrix && right) : values(right.values), num_cols(right.columns()), num_rows(right.columns()) {
    right.values = nullptr;
    std::cout << "in move constructor" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> list) {
    num_rows = list.size();
    num_cols = list.begin()->size();
    
    if (num_cols == 0 || num_rows == 0) 
        throw MatrixDimensionError();

    for (auto row : list) 
        if (row.size() != num_cols) 
            throw MatrixDimensionError(); 
    
    values = new double [num_rows * num_cols];

    unsigned int i = 0;

    for (auto row : list) 
        for(auto value : row) 
            values[i++] = value;
    
    std::cout << "in init constructor" << std::endl;

}

Matrix::~Matrix() {
    delete [] this->values;
    values = nullptr;
}

Matrix & Matrix::operator = (const Matrix & right) {
    if (this == & right) 
        return *this;
    
    this->num_cols = right.columns();
    this->num_rows = right.rows();
    this->values = new double [right.size()];

    memcpy(values, right.values, sizeof(values[0]) * right.size());

    std::cout << "in copy assignment" << std::endl;
    
    return *this;
}

Matrix & Matrix::operator = (Matrix && right) {
    if (this == & right) 
        return *this;
    
    this->num_cols = right.columns();
    this->num_rows = right.rows();

    delete this->values;
    values = right.values;
    right.values = nullptr;

    std::cout << "in move assignment" << std::endl;
    
    return *this;
}


Matrix & Matrix::operator = (const std::initializer_list<std::initializer_list<double>> & list) {   
    std::cout << "in init assignment " << std::endl;
    
    this->num_rows = list.size();
    this->num_cols = list.begin()->size();
    
    if (this->num_cols == 0 || this->num_rows == 0) 
        throw MatrixDimensionError();

    for (auto row : list) 
        if (row.size() != this->num_cols) 
            throw MatrixDimensionError(); 
    
    this->values = new double [this->size()];

    unsigned int i = 0;

    for (auto row : list) 
        for(auto value : row) 
            this->operator()(i++) = value;
    
    std::cout << "in init constructor" << std::endl;

    return *this;
}


inline const double * Matrix::getValues() const {
    return this->values;
}

// Matrix Multiplication 
Matrix Matrix::dot(const Matrix & right) const {
    if (this->columns() != right.rows()) 
        throw MultiplyMatrixError();
    
    Matrix output_matrix (this->rows(), right.columns(), 0);


    for (unsigned int i = 0; i < this->rows(); i++) 
        for (unsigned int k = 0; k < this->columns(); k++)
    		for(unsigned int j = 0; j < right.columns(); j++) 
				output_matrix(i, j) += this->operator()(i, k) * right(k, j);
            
		
    return output_matrix;
}

Matrix Matrix::multiply(const Matrix & right) const {
    if (this->columns() != right.columns() || this->rows() != right.rows())
        throw MatrixDimensionError();

    Matrix output_matrix(*this);

    for (unsigned int i = 0; i < this->columns() * this->rows(); i++) 
        output_matrix(i) *= right(i);

    return output_matrix;
}

// Scale Matrix
Matrix operator * (const Matrix & left, const double & right) {
    Matrix output_matrix(left);

    for (unsigned int i = 0; i < left.columns() * left.rows(); i++) 
        output_matrix(i) *= left(i);

    return output_matrix;
}

Matrix operator * (const double & left, const Matrix & right) {
    Matrix output_matrix(right);

    for (unsigned int i = 0; i < right.columns() * right.rows(); i++) 
        output_matrix(i) *= right(i);

    return output_matrix;
}

// Matrix Addition
Matrix Matrix::operator + (const Matrix & right) const {
    if(this->rows() != right.rows() || this->columns() != right.columns()) 
        throw MatrixDimensionError();

    Matrix output_matrix(right);

    for (unsigned int i = 0; i < this->rows() * this->columns(); i++)
			output_matrix(i) += this->operator()(i);

    return output_matrix;
}

// Scalar Addition
Matrix operator + (const Matrix & left, const double & right) {

    Matrix output_matrix(left);

    for (unsigned int i = 0; i < left.rows() * left.columns(); i++)
			output_matrix(i) += right;

    return output_matrix;
}

Matrix operator + (const double & left, const Matrix & right) {
    Matrix output_matrix(right);

    for (unsigned int i = 0; i < right.rows() * right.columns(); i++)
			output_matrix(i) += left;

    return output_matrix;
}

// Matrix Subtraction
Matrix Matrix::operator - (const Matrix & right) const {
    if(this->rows() != right.rows() || this->columns() != right.columns()) 
        throw MatrixDimensionError();

    Matrix output_matrix(*this);

    for (unsigned int i = 0; i < this->rows() * this->columns(); i++)
			output_matrix(i) -= right(i);

    return output_matrix;
}

// Scalar Subtraction
Matrix operator - (const Matrix & left, const double & right) {
    Matrix output_matrix(left);

    for (unsigned int i = 0; i < left.rows() * left.columns(); i++)
			output_matrix(i) -= right;

    return output_matrix;
}

Matrix operator - (const double & left, const Matrix & right) {
    Matrix output_matrix(right);

    for (unsigned int i = 0; i < right.rows() * right.columns(); i++)
			output_matrix(i) -= left;

    return output_matrix;
}

Matrix Matrix::row (const unsigned int & row) const {
    const int columns = this->columns();
    const double * values = this->getValues();
    

    if(row > this->rows()) {
        throw MatrixDimensionError();
    }

    double * output = new double [columns];
   
    for(unsigned int j = 0; j < columns; j++)
        output[j] = values[row * columns + j];

    Matrix output_matrix (output, 1, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix Matrix::col (const unsigned int & col) const {
    const unsigned int rows = this->rows();
    const double * values = this->getValues();
    

    if(col > this->columns()) {
        throw MatrixDimensionError();
    }

    double * output = new double [rows];
   
    for(unsigned int j = 0; j < rows; j++)
        output[j] = values[j * this->columns() + col];

    Matrix output_matrix (output, rows, 1);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

inline unsigned int Matrix::rows() const {
    return num_rows;
}

inline unsigned int Matrix::columns() const {
    return num_cols;
}

inline
unsigned int Matrix::size() const {
    return rows()*columns();
}

std::pair<unsigned int, unsigned int> Matrix::shape() const {
    return std::make_pair(this->num_rows, this->num_cols);
}

void Matrix::addRow(const Matrix & rowMatrix) {
    if (rowMatrix.columns() != rowMatrix.columns())
        throw MatrixDimensionError();
    
    double * temp = values;

    values = nullptr;
    values = new double [size() + rowMatrix.columns()];

    memcpy(values, temp, sizeof(temp[0]) * size());
    memcpy(values + size(), rowMatrix.values, sizeof(temp[0]) * size());

    num_rows++;

}

void Matrix::addColumn(const Matrix & columnMatrix) {
    // TODO - this needs to be updated. needs to add element from columnMatrix to end of every row
    if (columnMatrix.rows() != columnMatrix.columns())
        throw MatrixDimensionError();
    
    double * temp = values;

    values = nullptr;
    values = new double [size() + columnMatrix.rows()];

    memcpy(values, temp, sizeof(temp[0]) * size());
    memcpy(values + size(), columnMatrix.values, sizeof(temp[0]) * size());

    num_rows++;
}

double Matrix::max() const {
    double maxval = std::numeric_limits<double>::lowest();

    for (unsigned int i = 0; i < size(); i++) {
        if (values[i] > maxval) {
            maxval = values[i];
        }
    }

    return maxval;
}

double Matrix::min() const {
    double minval = std::numeric_limits<double>::max();

    for (unsigned int i = 0; i < size(); i++) {
        if (values[i] < minval) {
            minval = values[i];
        }
    }

    return minval;
}


Matrix Matrix::Transpose() const {

    Matrix output_matrix (this->columns(), this->rows());

	for (unsigned int i = 0; i < this->rows(); i++)
		for(unsigned int j = 0; j < this->columns(); j++)
			output_matrix(j, i) = this->operator()(i, j);

    return output_matrix;
}


std::vector<std::vector<double>> Matrix::toVector() const {
    std::vector<std::vector<double>> output_vector(size());
    for (auto & row : output_vector) 
        row = std::vector<double> (rows());
    
    for (unsigned int i = 0; i < rows(); i++)
        for(unsigned int j = 0; j < columns(); j++)
            output_vector.at(i).at(j) = this->operator()(i, j);

    return output_vector;
}


std::ostream & operator << (std::ostream & os, const Matrix & matrix) {
    int width_max = std::to_string(matrix.max()).size();
    int width_min  = std::to_string(matrix.min()).size();

    for (unsigned int i = 0; i < matrix.rows(); i++) {
        os << "|";
		for(unsigned int j = 0; j < matrix.columns(); j++) {
            os << " " << std::setfill(' ') << std::setw(std::max(width_max, width_min)) << matrix(i, j);
        }
        os << " |" << std::endl;
    }

    return os;
}

bool operator == (const Matrix & left, const Matrix & right) {
    const double epsilon1 = 1e-5;
    const double epsilon2 = 1e-8;
    
    const double * rightValues = right.getValues();
    const double * leftValues  = left.getValues();
    
    if (left.rows() != right.rows() || left.columns() != right.columns())
        return false;
    
    for (unsigned int i = 0; i < left.size(); i++) 
        if (std::abs(leftValues[i] - rightValues[i]) >= epsilon1 + std::max(std::abs(rightValues[i]), std::abs(leftValues[i])) * epsilon2) 
            return false;

    return true;
}

Matrix Identity(const unsigned int & size) {
    Matrix output_matrix (size, size);

    for (unsigned int i = 0; i < size; i++)
        output_matrix(i, i) = 1.0;

    return output_matrix;
}

Matrix RandomMatrix(const unsigned int & rows, const unsigned int & columns) {
    Matrix output_matrix (rows, columns);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution (-1, 1);

    for (unsigned int i = 0; i < rows * columns; i++)
        output_matrix(i) = distribution(generator);

    return output_matrix;
}


Matrix RandomNormalMatrix(const unsigned int & rows, const unsigned int & columns, const double & mean, const double & standard_deviation){
    Matrix output_matrix (rows, columns);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution (mean, standard_deviation);

    for (unsigned int i = 0; i < rows * columns;) {
        double randnum = distribution(generator);
        if (randnum < 1 && randnum > -1) 
            output_matrix(i++) = randnum;

    }
    
    return output_matrix;
}

