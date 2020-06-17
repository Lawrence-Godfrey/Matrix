#include "Matrix.h"

int main() {
    using namespace std;

    double * mat1 = new double[4];

    mat1[0] = 1;
    mat1[1] = 2;
    mat1[2] = 3;
    mat1[3] = 4;

    Matrix M1(mat1, 2, 2);
    cout << "M1,  using array" << endl << M1 << endl;

    Matrix M2 = {{1, 2,},
                 {3, 4}};
                 
    cout << "M2, using list initializer" << endl << M2 << endl;

    Matrix M3 = M1 * M2;
    cout << "M1 * M2" << endl << M3 << endl;

    Matrix M4 = M1 + M2;
    cout << "M1 + M2" << endl << M4 << endl;

    Matrix M5 = M1 - M2;
    cout << "M1 - M2" << endl << M5 << endl;

    Matrix M6 = M1.Transpose();
    cout << "Transpose M1 = " << endl << M6 << endl;

    Matrix M7(2, 2);
    M7 = M6;
    cout << "Transpose copy using copy assignment = " << endl << M7 << endl;

    Matrix M8 = M7;
    cout << "Transpose copy using copy constructor = " << endl << M8 << endl;

    Matrix M9 (M8);
    cout << "Transpose copy using copy constructor = " << endl << M9 << endl;

    Matrix M10(Matrix(2, 2));
    cout << "Transpose move using copy constructor = " << endl << M10 << endl;

    Matrix M11 = M10 - 1;
    cout << "M10 - 1" << endl << M11 <<endl;

    Matrix I = Identity(10);
    cout << "Identity of 10 x 10\n" << I << endl;

    Matrix row = I.row(4);
    cout << "row 4 of I" << row << endl;

    Matrix col = I.col(4);
    cout << "column 4 of I" << col << endl;


    return 0;
}