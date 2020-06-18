#include "Matrix.h"

Matrix::Matrix() : values(nullptr), num_cols(0), num_rows(0) {}

Matrix::Matrix(const int & rows, const int & cols) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {}

Matrix::Matrix(const double & init_value, const int & rows, const int & cols) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {
    for (int i = 0; i < num_cols * num_rows; i++) {
        values[i] = init_value;
    }
}

Matrix::Matrix(const double * matrix, const int & rows, const int & cols) : num_rows(rows), num_cols(cols), values(new double [num_rows * num_cols]) {  
    memcpy(values, matrix, sizeof(values[0]) * num_rows * num_cols);
}


Matrix::Matrix(const Matrix & right) : num_cols(right.columns()), num_rows(right.columns()), values(new double [num_rows * num_cols]) {   
    std::cout << "in copy constructor" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;
    memcpy(values, right.values, sizeof(values[0]) * num_rows * num_cols);
}

Matrix::Matrix(Matrix && right) : values(right.values), num_cols(right.columns()), num_rows(right.columns()) {
    right.values = nullptr;
    std::cout << "in move constructor" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> list) : num_rows(list.size()), num_cols(list.begin()->size()), values(new double [num_rows * num_cols]) {
    int i = 0;

    for (auto row : list) 
        for(auto value : row) 
            values[i++] = value;

}

Matrix::~Matrix() {
    delete [] this->values;
}

Matrix & Matrix::operator = (const Matrix & right) {
    if (this == & right) 
        return *this;
    
    num_cols = right.columns();
    num_rows = right.rows();

    std::cout << "in copy assignment" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;

    memcpy(values, right.values, sizeof(values[0]) * num_rows * num_cols);
    return *this;
}

Matrix & Matrix::operator = (Matrix && right) {
    if (this == & right) 
        return *this;
    
    this->num_cols = right.columns();
    this->num_rows = right.rows();

    std::cout << "in move assignment" << std::endl << "rows: " << num_rows << " cols: " << num_cols << std::endl << std::flush;

    delete values;
    values = right.values;
    right.values = nullptr;

    return *this;
}


Matrix && Matrix::operator = (const std::initializer_list<std::initializer_list<double>> & list) {   
    return Matrix (list);
}


const double * Matrix::getValues() const {
    return this->values;
}

// Matrix Multiplication 
Matrix Matrix::operator * (const Matrix & right) const {
    const int rows = this->rows();
    const int columns = this->columns();
    const double * values = this->getValues();

    const int right_rows = right.rows();
    const int right_columns = right.columns();
    const double * right_values = right.getValues();

    if (columns != right_rows) 
        throw MultiplyMatrixError();
    

    double * output = new double [rows * right_columns];

    for (int i = 0; i < rows; i++) {
		for(int j = 0; j < right_columns; j++)
		{
			output[i * right_columns + j] = 0;

			for (int k = 0; k < this->num_cols; k++)
				output[right_columns * i + j] += values[i * columns + k] * right_values[right_columns * k + j];
		}
    }

    Matrix output_matrix (output, rows, right_columns);

    delete [] output;
    output = nullptr;

    return output_matrix;
}

// Scale Matrix
Matrix operator * (const Matrix & left, const double & right) {
    const int rows = left.rows();
    const int columns = left.columns();
    const double * values = left.getValues();

    std::cout << "in + operator Matrix - scalar \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = values[i * columns + j] * right;

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix operator * (const double & left, const Matrix & right) {
    const int rows = right.rows();
    const int columns = right.columns();
    const double * values = right.getValues();

    std::cout << "in - operator scalar - Matrix \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = left * values[i * columns + j];

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

// Matrix Addition
Matrix Matrix::operator + (const Matrix & right) const {
    const int rows = this->rows();
    const int columns = this->columns();
    const double * values = this->getValues();


    if(rows != right.rows() || columns != right.columns()) 
        throw MatrixDimensionError();

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = values[i * columns + j] + right.getValues()[i * columns + j];

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

// Scalar Addition
Matrix operator + (const Matrix & left, const double & right) {
    const int rows = left.rows();
    const int columns = left.columns();
    const double * values = left.getValues();

    std::cout << "in + operator Matrix - scalar \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = values[i * columns + j] + right;

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix operator + (const double & left, const Matrix & right) {
    const int rows = right.rows();
    const int columns = right.columns();
    const double * values = right.getValues();

    std::cout << "in - operator scalar - Matrix \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = left + values[i * columns + j];

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

// Matrix Subtraction
Matrix Matrix::operator - (const Matrix & right) const {
    const int rows = this->rows();
    const int columns = this->columns();
    const double * values = this->getValues();
    

    if(rows != right.rows() || columns != right.columns()) {
        throw MatrixDimensionError();
    }

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = values[i * columns + j] - right.getValues()[i * columns + j];

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

// Scalar Subtraction
Matrix operator - (const Matrix & left, const double & right) {
    const int rows = left.rows();
    const int columns = left.columns();
    const double * values = left.getValues();

    std::cout << "in - operator Matrix - scalar \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = values[i * columns + j] - right;

    Matrix output_matrix(output, rows, columns);
   
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix operator - (const double & left, const Matrix & right) {
    const int rows = right.rows();
    const int columns = right.columns();
    const double * values = right.getValues();

    std::cout << "in - operator scalar - Matrix \n rows: " << rows << " cols: " << columns << std::endl;

    double * output = new double [rows * columns];

    for (int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			output[i * columns + j] = left - values[i * columns + j];

    Matrix output_matrix(output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix Matrix::row (const int & row) const {
    const int columns = this->columns();
    const double * values = this->getValues();
    

    if(row > this->rows()) {
        throw MatrixDimensionError();
    }

    double * output = new double [columns];
   
    for(int j = 0; j < columns; j++)
        output[j] = values[row * columns + j];

    Matrix output_matrix (output, 1, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

Matrix Matrix::col (const int & col) const {
    const int rows = this->rows();
    const double * values = this->getValues();
    

    if(col > this->columns()) {
        throw MatrixDimensionError();
    }

    double * output = new double [rows];
   
    for(int j = 0; j < rows; j++)
        output[j] = values[j * this->columns() + col];

    Matrix output_matrix (output, rows, 1);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}

int Matrix::rows() const {
    return num_rows;
}

int Matrix::columns() const {
    return num_cols;
}

double Matrix::max () const {
    double maxval = std::numeric_limits<double>::lowest();

    for (int i = 0; i < rows() * columns(); i++) {
        if (values[i] > maxval) {
            maxval = values[i];
        }
    }

    return maxval;
}

double Matrix::min () const {
    double minval = std::numeric_limits<double>::max();

    for (int i = 0; i < rows() * columns(); i++) {
        if (values[i] < minval) {
            minval = values[i];
        }
    }

    return minval;
}


Matrix Matrix::Transpose () const {
    const int rows = this->rows();
    const int columns = this->columns();
    const double * values = this->getValues();

    double * output = new double [rows * columns];

	int i, j;
	for (i = 0; i < rows; i++)
		for(j = 0; j < columns; j++)
			output[j * rows + i] = values[i * columns + j];

    Matrix output_matrix (output, rows, columns);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}


std::ostream & operator << (std::ostream & os, const Matrix & matrix) {
    int width = std::to_string(matrix.max()).size();
    std::cout << "max: " << matrix.max() << std::endl;
    for (int i = 0; i < matrix.rows(); i++) {
        os << "|";
		for(int j = 0; j < matrix.columns(); j++) {
            os << " " << std::setfill(' ') << std::setw(width) << matrix.values[i * matrix.columns() + j];
        }
        os << " |" << std::endl;
    }

    return os;
}

Matrix Identity(const int & size) {
    double * output = new double [size * size];

    for (int i = 0; i < size; i++)
        output[i * size + i] = 1.0;

    Matrix output_matrix (output, size, size);
    
    delete [] output;
    output = nullptr;

    return output_matrix;
}