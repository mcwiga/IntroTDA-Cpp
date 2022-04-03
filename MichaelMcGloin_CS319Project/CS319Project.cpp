// Author : Michael McGloin
// Student ID: 18357636
// Module : CS319
// About : This is a programme that calculates topological features of a set of points 
// and outputs a barcode of the features. I have included the files Eight.txt , Torus.txt , Sphere.txt
// and Cluster.txt to test out the programme.
// However for larger radii it takes a bit to compute so I reccommend keeping the maximum radius less than 5

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "VietorisRips.h"

int createArray(int** M, int& Rows, int& Cols,int& noVertices, std::string fileName, std::vector<std::vector<double>>& vec);
int getArraySize(int& Rows, int& Cols, std::string fileName);
void PrintArray(int** Array, int M, int N);

int main()
{
	// Open File
	int Rows, Cols;
	std::string fileName;
	std::cout << "Please enter filename : " << std::endl;
	std::cin >> fileName;
	std::ifstream InFile;
	InFile.open(fileName);
	if (!InFile.is_open()) {
		while (!InFile.is_open()) {
			std::cout << "File not found!\nPlease enter filename : " << std::endl;
			std::cin >> fileName;
			InFile.open(fileName);
		}
	}
	
	
	int noVertices = 0;
	
	// Create Array 
	std::vector<std::vector<double>> vertices;
	getArraySize(Rows, Cols, fileName);
	int** M = new int* [Rows];
	createArray(M, Rows, Cols, noVertices, fileName, vertices);


	// Display the maze
	std::cout << "Here is the maze: ";
	PrintArray(M, Rows, Cols);

	//Create Vietoris Rips Complex

	VietorisRips complex(Rows, Cols, vertices);

	complex.findFeatures(); // finds topological features
	complex.Barcodes();		// creates barcode

	//complex.PrintSimplex(1); 
	
	return 0;
}



// Create our array
int createArray(int** M, int& Rows, int& Cols,int &noVertices, std::string fileName, std::vector<std::vector<double>>& vec) {
	std::ifstream InFile;
	InFile.open(fileName);
	if (InFile.fail())
	{
		std::cerr << "Error - cannot open file" << std::endl;
		return (1);
	}	
	int rows, cols;
	InFile >> rows >> cols;
	for (int i = 0; i < Rows; ++i)
		M[i] = new int[Cols];

	/// Read in the data
	for (int i = 0; i < Rows; i++)
		for (int j = 0; j < Cols; j++)
		{
			if ((double)(rand() % 10)>=0) { // option to randomly sample points
				char c;
				InFile >> c;
				M[i][j] = (int)(c - '0');
				if (M[i][j] != 0)
				{
					vec.push_back({ 1.0*i,1.0*j }); //convert to double by multiplying by 1.0
					noVertices++;
				}
			}
			else {
				char c;
				InFile >> c;
				M[i][j] = (int)(0);
			}
		}

	InFile.close();
	return(0);
}


//get size of array
int getArraySize(int& Rows, int& Cols, std::string fileName) {
	std::ifstream InFile;
	InFile.open(fileName);
	if (InFile.fail())
	{
		std::cerr << "Error - cannot open file" << std::endl;
		return (1);
	}
	InFile >> Rows >> Cols;
	InFile.close();
}


// Print the array
void PrintArray(int** Array, int M, int N)
{
	std::cout << std::endl;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
			std::cout << Array[i][j];
		std::cout << std::endl;
	}
}