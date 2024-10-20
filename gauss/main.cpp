#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <format>
#include <fstream>

double** readFile(const std::string& filePath, int& rows, int& cols) 
{
    double** mat;
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) 
        throw std::runtime_error("Unable to open input file");
    

    inFile >> cols;
    rows = cols - 1;
    mat = new double*[rows];
    
    for(int i = 0; i < rows; ++i) {
        mat[i] = new double[cols];
        for(int j = 0; j < cols; ++j) {
            inFile >> mat[i][j];
        }
        inFile.ignore();
    }

    inFile.close();
    return mat;
}

void printMatrix(double** mat, int size)
{
    for(int i = 0; i < size - 1; ++i)
    {
        for(int j = 0; j < size; ++j)
            std::cout << std::setw(10) << mat[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void gauss(double** mat, int size)
{
    double mult;
    std::cout << std::endl;
    for(int k = size-2; k >= 0; --k)
    {
        for(int i = k - 1; i >= 0; --i)
        {
            mult = mat[i][k] / mat[k][k];
            for(int j = size-1; j >= 0; --j)
                mat[i][j] -= mult * mat[k][j];
        }
        printMatrix(mat, size);
        std::cout << "Mult: " << mult << std::endl;
    }

    double result[size - 1];
    for(int i = 0; i < size - 1; ++i)
    {
        result[i] = 0;
    }
    double x;

    for(int i = 0; i < size-1; ++i)
    {
        x = mat[i][size - 1];
        for(int j = 0; j < size - 1; ++j)
            x -= mat[i][j] * result[j];
        result[i] = x / mat[i][i];
    }

    for(int i = 0; i < size - 1; ++i)
        std::cout << "x" << i + 1 << " = " << result[i] << std::endl;
}

void gaussMod(double** mat, int size)
{
    double mult;

    int rowIndexMax;

    for(int k = 0; k < size - 1; ++k)
    {
        rowIndexMax = k;
        for(int i = k + 1; i < size - 1; ++i)
            if(std::abs(mat[i][k]) > std::abs(mat[rowIndexMax][k]))
                rowIndexMax = i;

        double* temp = mat[k];
        mat[k] = mat[rowIndexMax];
        mat[rowIndexMax] = temp;

        for(int i = k + 1; i < size - 1; ++i)
        {
            mult = mat[i][k] / mat[k][k];
            for(int j = k; j < size; ++j)
                mat[i][j] -= mult * mat[k][j];
        }
        printMatrix(mat, size);
        std::cout << "Mult: " << mult << std::endl;
    }

    double result[size - 1];
    double x;

    for(int i = size - 2; i >= 0; --i)
    {
        x = mat[i][size - 1];
        for(int j = i; j < size - 1; ++j)
            x -= mat[i][j] * result[j];
        result[i] = x / mat[i][i];
    }

    for(int i = 0; i < size - 1; ++i)
        std::cout << "x" << i + 1 << " = " << result[i] << std::endl;
}

int main(int argc, char* argv[])
{
    int cols;
    int rows;

    double** mat;
    
    mat = readFile("input.txt", rows, cols);
    printMatrix(mat, cols);
    gauss(mat, cols);

    // mat = readFile("input.txt", rows, cols);
    // printMatrix(mat, cols);
    // gaussMod(mat, cols);

    return 0;
}
