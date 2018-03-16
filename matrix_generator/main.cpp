//
//  main.cpp
//  matrix_generator
//
//  Created by Micheli  Knechtel on 15/12/2015.
//  Copyright © 2015 Micheli Knechtel. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision


using namespace std;

const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d.%m.%Y %X", &tstruct);
    
    return buf;
}

void swap(int array[], int pos1, int pos2)
{
    int tmp = array[pos1];
    array[pos1] = array[pos2];
    array[pos2] = tmp;
}

void printMatrix(int *a,int rows,int cols, char name, string fileName)
{
    
    ofstream outfile;
    outfile.open (fileName.c_str(), std::ofstream::out | std::ofstream::app);
    
    outfile<<endl<<name<<" = ["<<endl;
    
    for(int i=0;i<rows;i++)
    {
        outfile<<"\t[ ";
        for(int j=0;j<cols;j++)
        {
            if (j+1 == cols)
                outfile<<*(a+ i*rows + j)<<" ";
            else
                outfile<<*(a+ i*rows + j)<<",";

        }
        
        outfile<<"]"<<endl;
    }
    outfile<<"\t];"<<endl;
}


void generateMatrixIdentity(int K, string fileName)
{
    int matrix[K][K];
    for (int i = 0; i<K; i++)
    {
        matrix[i][i]=1;
    }
    
    for (int i = 0; i<K; i++)
    {
        for (int j = 0; j<K; j++)
            if (i!=j)matrix[i][j] = 0;
    }

    printMatrix(matrix[0], K, K, 'B', fileName);
}

void generateMatrixIdentityF(int K, string fileName)
{
    int matrix[K][K];
    for (int i = 0; i<K; i++)
    {
        matrix[i][i]=1;
    }
    
    for (int i = 0; i<K; i++)
    {
        for (int j = 0; j<K; j++)
        {
            if (i!=j)matrix[i][j] = 0;
            if (i==0 ||j==0) {
                matrix[i][j] = 1;
            }
        }
        
    }
    
    printMatrix(matrix[0], K, K,'B',fileName);
}

void generateMatrixIdentityEN(int K, string fileName)
{
    int matrix[K][K];
    
    for (int i = 0; i<K-1; i++)
        matrix[i][i]=1;
    
    matrix[K-1][K-1] = 0;
    
    for (int i = 0; i<K; i++)
        for (int j = 0; j<K; j++)
            if (i!=j)matrix[i][j] = 0;

    
    
    printMatrix(matrix[0], K, K,'B', fileName);
}

void generateMatrixAntiDiagonal(int K, string fileName)
{
    int matrix[K][K];
    
    for (int i = 0; i<K; i++)
        matrix[i][K-i-1]=0;
    
    for (int i = 0; i<K; i++)
        for (int j = 0; j<K; j++)
            if (j!=K-i-1)
                matrix[i][j] = 1;
    
    
    printMatrix(matrix[0], K, K, 'B', fileName);
}

void generateImageMatrix(int K, char T, string fileName )
{
    switch (T) {
        case 'I':
            generateMatrixIdentity(K, fileName);
            break;
        case 'A':
            generateMatrixAntiDiagonal(K, fileName);
            break;
        case 'E':
            generateMatrixIdentityEN(K, fileName);
            break;
        case 'F':
            generateMatrixIdentityF(K, fileName);
            break;

        default:
            break;
    }
    
    
}

void generateRelationshipMatrix(int N, int lenght, float d, int K, string fileNameA, string fileNameB, string fileNameC, string fileNameD)
{
    srand((unsigned)time(0));
    
    int numbers[lenght];
    int matrix[N][N];
    
    float s = N * (N-1) * d;
    
    // fill in the numbers
    for (int i = 0; i < lenght; ++i)
    {
        if (i<=s)
        {
            numbers[i] = 1;
        }
        else
            numbers[i] = 0;
    }
    
    // a random number of swaps
    int repeat = rand() % lenght + lenght;
    
    // perform the swaps...
    for (int i = 0; i < repeat; ++i)
    {
        // ... with random positions
        swap(numbers, rand() % lenght, rand() % lenght);
    }
    
    // fill in the matrix
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix[i][j] = numbers[i * N + j];
        }
    }
    
    printMatrix(matrix[0], N, N, 'S', fileNameA);
    printMatrix(matrix[0], N, N, 'S', fileNameB);
    printMatrix(matrix[0], N, N, 'S', fileNameC);
    printMatrix(matrix[0], N, N, 'S', fileNameD);

}

void generatePenaltyMatrix(int K, string fileName)
{
    int matrix[K][K];

    for (int i = 0; i<K; i++)
        for (int j = 0; j<K; j++)
                matrix[i][j] = 1;
    
    printMatrix(matrix[0], K, K, 'P', fileName);
    printMatrix(matrix[0], K, K, 'O', fileName);
    printMatrix(matrix[0], K, K, 'L', fileName);
}

void generateDescription(string fileName)
{
    
    ofstream outfile(fileName.c_str());
   
    outfile<<"/*********************************************"<<endl;
    outfile<<"* OPL 12.6.1.0 Data"<<endl;
    outfile<<"* Author: Micheli Knechtel"<<endl;
    outfile<<"* Creation Date: "<< currentDateTime()<<endl;
    outfile<<"*********************************************/"<<endl;
    outfile.close();

    
};

void generateVariable(int N, int K, string fileName)
{
    ofstream outfile;
    outfile.open (fileName.c_str(), std::ofstream::out | std::ofstream::app);
    outfile<<"N = "<<N<<";"<<endl;
    outfile<<"K = "<<K<<";"<<endl;
    outfile.close();

};

void generateReader(int N, int K, string fileName)
{
    generateDescription(fileName);
    generateVariable(N, K, fileName);
    generatePenaltyMatrix(K,fileName);
    
}

string generateFileName(int N, float d, int K, char T )
{
    string date = currentDateTime();

    std::string stringD(16, '\0');
    auto written = snprintf(&stringD[0], stringD.size(), "%.1f", d);
    stringD.resize(written);
    
    string path = "/Users/micheliknechtel/Documents/xCode/generate_data/";
    string extension = ".dat";
    
    string fileName = path +"N=" +to_string(N) + " " +" K="+ to_string(K)+ " " +"D="+ stringD + " " + "B="+ T + " " + date + extension;
    return fileName;

}

void generateFile(int N, float d, int K)
{
    string fileNameA = generateFileName(N,d,K,'1');
    generateReader(N, K, fileNameA.c_str());
    generateImageMatrix(K,'I', fileNameA.c_str());
    
    string fileNameB =  generateFileName(N,d,K,'2');
    generateReader(N, K, fileNameB);
    generateImageMatrix(K,'A', fileNameB);
    
    string fileNameC = generateFileName(N,d,K,'3');
    generateReader(N, K, fileNameC);
    generateImageMatrix(K,'E',fileNameC);
    
    std::string fileNameD = generateFileName(N,d,K,'4');
    generateReader(N, K, fileNameD);
    generateImageMatrix(K,'F', fileNameD);

    generateRelationshipMatrix(N, N * N , d, K, fileNameA,fileNameB, fileNameC,fileNameD);
    
    
    
}

int main(int argc, const char * argv[])
{
    
    
    
    
    int N, K;
   
    N = 10;
    K = 3;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);
    
    
    K = 4;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);
    
    N = 20;
    K = 3;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);

    K = 4;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);
    
    N = 40;
    K = 3;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);
    
    K = 4;
    
    generateFile(N, 0.2,K);
    generateFile(N, 0.5,K);
    generateFile(N, 0.8,K);
    
    return 0;
}
