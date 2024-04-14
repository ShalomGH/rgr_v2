#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

class MatrixCalculator {
private:
    vector<vector<int>> matrix1;
    vector<vector<int>> matrix2;
    vector<vector<int>> productMatrix;
    int columns = 0;
    int rows = 0;
    int type;

public:
    explicit MatrixCalculator(int t) { type = t; }

    void inputSquareMatrix(vector<vector<int>> &matrix, const string &prompt) const {
        matrix.resize(columns, vector<int>(columns));
        cout << prompt << endl;
        for (int i = 0; i < columns; ++i) {
            for (int j = 0; j < columns; ++j) {
                cin >> matrix[i][j];
            }
        }
        cin.get();
    }

    void inputMatrix(vector<vector<int>> &matrix, const string &prompt) const {
        matrix.resize(rows, vector<int>(columns));
        cout << prompt << endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                cin >> matrix[i][j];
            }
        }
        cin.get();
    }

    void addMatrices() {
        productMatrix.resize(columns, vector<int>(columns));
        for (int i = 0; i < columns; ++i) {
            for (int j = 0; j < columns; ++j) {
                productMatrix[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
    }

    void minusMatrices() {
        productMatrix.resize(columns, vector<int>(columns));
        for (int i = 0; i < columns; ++i) {
            for (int j = 0; j < columns; ++j) {
                productMatrix[i][j] = matrix1[i][j] - matrix2[i][j];
            }
        }
    }

    void multiplyMatrixNum(int num) {
        productMatrix.resize(columns, vector<int>(rows));
        for (int i = 0; i < columns; ++i) {
            for (int j = 0; j < rows; ++j) {
                productMatrix[i][j] = matrix1[i][j] * num;
            }
        }
    }

    void multiplyMatrices() {
        productMatrix.resize(columns, vector<int>(columns, 0));
        for (int i = 0; i < columns; ++i) {
            for (int j = 0; j < columns; ++j) {
                for (int k = 0; k < columns; ++k) {
                    productMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
    }

    void transpose() {
        productMatrix.resize(columns, vector<int>(rows));
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                productMatrix[j][i] = matrix1[i][j];
            }
        }
    }

    static void displayMatrix(const vector<vector<int>> &matrix) {
        for (const auto &row: matrix) {
            for (int elem: row) {
                cout << elem << " ";
            }
            cout << endl;
        }
    }

    void run() {
        if (type < 4) {
            cout << "Enter columns number of the matrices: ";
            cin >> columns;
            cin.get();
        } else {
            cout << "Enter columns number of the matrices: ";
            cin >> columns;
            cin.get();
            cout << "Enter the rows number of the matrices: ";
            cin >> rows;
            cin.get();
        }

        switch (type) {
            case 1: {
                inputSquareMatrix(matrix1, "Enter the elements of the first matrix:");
                inputSquareMatrix(matrix2, "Enter the elements of the second matrix:");
                addMatrices();
                cout << "\nSum ";
                break;
            }
            case 2: {
                inputSquareMatrix(matrix1, "Enter the elements of the first matrix:");
                inputSquareMatrix(matrix2, "Enter the elements of the second matrix:");
                minusMatrices();
                cout << "\nDifference ";
                break;
            }
            case 3: {
                inputSquareMatrix(matrix1, "Enter the elements of the first matrix:");
                inputSquareMatrix(matrix2, "Enter the elements of the second matrix:");
                multiplyMatrices();
                cout << "\nMultiply ";
                break;
            }
            case 4: {
                inputMatrix(matrix1, "Enter the elements of matrix:");
                int num;
                cout << "\nEnter num: ";
                cin >> num;
                multiplyMatrixNum(num);
                cout << "\nMultiplication with num ";
                cin.get();
                break;
            }
            case 5: {
                inputMatrix(matrix1, "Enter the elements of matrix:");
                transpose();
                cout << "\nEntered matrix \n";
                displayMatrix(matrix1);

                cout << "\nTransposition ";
                break;
            }
        }

        cout << "of the matrices is:\n" << endl;
        displayMatrix(productMatrix);
        cout << "\n\nPress button to close.." << endl;
        getch();
    }
};
