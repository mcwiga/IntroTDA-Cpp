// Author : Michael McGloin
// Student ID: 18357636
// Module : CS319
// About : This is a programme that calculates topological features of a set of points 
// and outputs a barcode of the features. I have included the files Eight.txt , Torus.txt , Sphere.txt
// and Cluster.txt to test out the programme.
// However for larger radii it takes a bit to compute so I reccommend keeping the maximum radius less than 5


#include "VietorisRips.h"
#include <cmath>
#include <algorithm>
#include <iostream>

VietorisRips::VietorisRips(int rows, int cols, std::vector<std::vector<double>>& vec)
{
	std::cout << "Please enter the max radius for the complex < 5 " << std::endl;
	std::cin >> maxEpsilon;
	std::cout << "Computing.... " << std::endl;
	no_points = vec.size();
	featuresCreated = 0;
	featuresDestroyed = 0;
	no_OneSimplices = 0;
	Rows = rows;
	Cols = cols;
	createSimplex(vec);
	createBoundaryMatrix();
	reduceBoundaryMatrix(BoundaryMatrix, no_points, no_OneSimplices);
	reduceBoundaryMatrix(boundaryMatrixTwoSimplices, no_OneSimplices, no_twoSimplices);
}


// finds the distance between 2 points
double VietorisRips::distance_ab(std::vector<double> a, std::vector<double> b)
{
	return (sqrt(pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2)));

}


//create our simplicial complex
void VietorisRips::createSimplex(std::vector<std::vector<double>>& vec)
{
	//std::cout << "Creating Simplex ..." << std::endl;
	for (int i = 0; i < no_points; i++)
	{
		for (int j = i; j < no_points; j++)
		{
			if ((distance_ab(vec[i], vec[j]) <= maxEpsilon) && (distance_ab(vec[i], vec[j]) != 0)) {
				no_OneSimplices++;
				Simplices.push_back({ distance_ab(vec[i], vec[j]), (double)i,(double)j });
			}

		}
	}
	//std::cout << "Simplex Created" << std::endl;
	std::cout << "Number of One Simplices = " << no_OneSimplices<< std::endl;

	findTwoSimplices(vec);
	BubbleSort(Simplices);
	BubbleSort(twoSimplices);

}


// Create the Boundary Matrix
void VietorisRips::createBoundaryMatrix() {

	BoundaryMatrix = new double* [no_points];
	for (int i = 0; i < no_points; ++i) {
		BoundaryMatrix[i] = new double[no_OneSimplices];
	}


	for (int i = 0; i < no_OneSimplices; i++)
	{
		for (int j = 0; j < no_points; j++) {

			if ((Simplices[i][1] == j)) {
				BoundaryMatrix[j][i] = Simplices[i][0];
			}
			else if ((Simplices[i][2] == j)) {
				BoundaryMatrix[j][i] = Simplices[i][0];
			}

			else {
				BoundaryMatrix[j][i] = 0;
			}
		}
	}
	createBoundaryMatrixTwoSimplices();
}


// Reduce our Boundary Matrix
void VietorisRips::reduceBoundaryMatrix(double** matrix, int rows, int columns) {

	for (int i = rows - 1; i >= 0; i--)
	{
		for (int k = 0; k < columns; k++)
		{
			if (matrix[i][k] != 0 && isLow(matrix, i, k, rows)) {
				for (int j = k + 1; j < columns; j++) {
					if (matrix[i][j] != 0 && isLow(matrix, i, j, rows)) {
						AddBoundaryRows(matrix, k, j, rows);
					}
				}
			}
		}
	}
}



// sort vector by first number in each vector
void VietorisRips::BubbleSort(std::vector<std::vector<double>>& Simplex) {
	if (Simplex.size() != 0) {
		//std::cout << "Bubble Sorting ..." << std::endl;
		for (unsigned int i = 0; i < Simplex.size() - 1; i++)

			// Last i elements are already in place 
			for (unsigned int j = 0; j < Simplex.size() - i - 1; j++)
				if (Simplex[j][0] > Simplex[j + 1][0])
					std::swap(Simplex[j], Simplex[j + 1]);
	}
}

//prints out matrix 
void VietorisRips::PrintMatrix(double** matrix, int rows, int columns)
{
	std::cout << std::endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++) {
			std::cout << matrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}

//Takes a vector of vectors representing a simplex and prints the simplex & its associated weight
// type 1 prints 1-simplices otherwise 2-simplices are printed.

void VietorisRips::PrintSimplices(std::vector<std::vector<double>>& Simplex, int type)
{
	std::cout << std::endl;
	for (unsigned int i = 0; i < Simplex.size(); i++)
	{
		if (type == 1) {
			std::cout << "A simplex (" << Simplex[i][1] << " , " << Simplex[i][2] << ") of Weight = " << Simplex[i][0] << std::endl;
		}
		else {
			std::cout << "A simplex (" << Simplex[i][1] << " , " << Simplex[i][2] << " , " << Simplex[i][3] << ") of Weight = " << Simplex[i][0] << std::endl;
		}
	}
}

// Add together Row one to row 2
void VietorisRips::AddBoundaryRows(double** matrix, int rowOne, int rowTwo, int noRows) {
	for (int i = 0; i < noRows; i++) {
		if ((matrix[i][rowTwo] != 0) && (matrix[i][rowOne] != 0)) {
			matrix[i][rowTwo] = 0;
		}

		else if ((matrix[i][rowTwo] == 0) && (matrix[i][rowOne] == 0)) {
			matrix[i][rowTwo] = 0;
		}

		else {
			matrix[i][rowTwo] = 1;
		}
	}

}


// check if a non-zero entry in a column is a lowest non-zero entry
bool VietorisRips::isLow(double** matrix, int rows, int col, int size)
{
	if (matrix[rows][col] == 0) {
		return false;
	}

	for (int i = rows + 1; i < size; i++) {
		if ((matrix[i][col] != 0)) {
			return false;
		}
	}
	return true;
}


// find the maximum distance between 3 points
double VietorisRips::max_distance_ab(std::vector<double> a, std::vector<double> b, std::vector<double> c)
{
	if ((distance_ab(a, b) >= distance_ab(b, c)) && (distance_ab(a, b) >= distance_ab(a, c))) {
		return distance_ab(a, b);
	}

	else if ((distance_ab(a, c) >= distance_ab(a, b)) && (distance_ab(a, c) >= distance_ab(c, b))) {
		return distance_ab(a, c);
	}
	else {
		return distance_ab(b, c);
	}
}

// Create out 2-simplices
void VietorisRips::findTwoSimplices(std::vector<std::vector<double>> vec)
{

	for (int i = 0; i < no_points - 2; i++) {
		for (int j = i + 1; j < no_points - 1; j++) {
			for (int k = j + 1; k < no_points; k++) {
				if ((k != j) && (max_distance_ab(vec[i], vec[j], vec[k]) <= maxEpsilon)) {
					twoSimplices.push_back({ max_distance_ab(vec[i], vec[j], vec[k]) , (double)i,(double)j,(double)k });
				}
			}
		}
	}
	no_twoSimplices = twoSimplices.size();

}


// Creat a boundary MAtrix for our 2-simplices
void VietorisRips::createBoundaryMatrixTwoSimplices()
{

	boundaryMatrixTwoSimplices = new double* [no_OneSimplices];
	for (int i = 0; i < no_OneSimplices; ++i) {
		boundaryMatrixTwoSimplices[i] = new double[(int)twoSimplices.size()];
	}

	for (int i = 0; i < (int)twoSimplices.size(); i++) {
		for (int j = 0; j < no_OneSimplices; j++) {
			if (((twoSimplices[i][1] == Simplices[j][1]) && (twoSimplices[i][2] == Simplices[j][2])) || (((twoSimplices[i][1] == Simplices[j][2]) && (twoSimplices[i][2] == Simplices[j][1])))) {
				boundaryMatrixTwoSimplices[j][i] = twoSimplices[i][0];
			}
			else if ((twoSimplices[i][2] == Simplices[j][1]) && (twoSimplices[i][3] == Simplices[j][2]) || (twoSimplices[i][2] == Simplices[j][2]) && (twoSimplices[i][3] == Simplices[j][1])) {
				boundaryMatrixTwoSimplices[j][i] = twoSimplices[i][0];
			}
			else if (((twoSimplices[i][1] == Simplices[j][1]) && (twoSimplices[i][3] == Simplices[j][2])) || ((twoSimplices[i][2] == Simplices[j][2]) && (twoSimplices[i][3] == Simplices[j][1]))) {
				boundaryMatrixTwoSimplices[j][i] = twoSimplices[i][0];
			}
			else {
				boundaryMatrixTwoSimplices[j][i] = 0;
			}
		}
	}

}


// Find topological features created/destroyed
void VietorisRips::findFeatures()
{

	for (int i = 0; i < no_OneSimplices; i++) {
		double isZero = 0;
		for (int j = 0; j < no_points; j++) {
			isZero += BoundaryMatrix[j][i];


		}

		if ((int)isZero == 0) {

			featuresCreated++;
			Simplices[i].push_back(0);
		}
		else {
			featuresDestroyed++;
			Simplices[i].push_back(1);
		}

	}
	std::cout << "\nFeatures Created from One Simplices Boundary matrix = " << featuresCreated << std::endl;
	std::cout << "Features Destroyed from One Simplices Boundary matrix = " << featuresDestroyed <<"\n\n";

	int twoSimplicesCreated = 0;
	int twoSimplicesDestroyed = 0;

	for (int i = 0; i < (int) twoSimplices.size(); i++) {
		double isZero = 0;
		for (int j = 0; j < no_OneSimplices; j++) {
			isZero += boundaryMatrixTwoSimplices[j][i];


		}

		if ((int)isZero != 0) {

			twoSimplicesDestroyed++;
			twoSimplices[i].push_back(1);
		}
		else {
			twoSimplicesCreated++;
			twoSimplices[i].push_back(0);
		}

	}
	std::cout << "Features Created from Two Simplices Boundary matrix = " << twoSimplicesCreated << std::endl;
	std::cout << "Features Destroyed from Two Simplices Boundary matrix = " << twoSimplicesDestroyed << std::endl;



}


// create and display our barcode
void VietorisRips::Barcodes() {

std::cout << "\n Calculating Persistence Barcode..." << std::endl;

std::cout << "\nThe barcodes consist of tuples of the form (Birth Time, Betti Number B_n)\n" << std::endl;

std::vector<std::vector<double>> B0;
B0.push_back({ 0, (double)no_points });
int index = 0;

for (int i = 0; i < no_OneSimplices; i++) {
	if (Simplices[i].back() == 1) {
		if (Simplices[i][0] == B0[index][0]) {
			B0[index].back()--;
		}
		else {
			B0.push_back({ Simplices[i][0], B0[index].back() - 1 });
			index++;
		}
	}
}

std::cout << "B0 = \n" << std::endl;
for (int i = 0; i < (int) B0.size(); i++) {
	std::cout << "( " << B0[i][0] << " , " << B0[i][1] << " ) ---> ";
}
std::cout << std::endl;
std::cout << std::endl;
std::cout << std::endl;

std::vector<std::vector<double>> B1;
int count = 0;
for (int i = 0; i < no_OneSimplices; i++) {
	if (Simplices[i].back() == 0) {
		B1.push_back({ Simplices[i][0], 0 });
		break;
	}

}



for (int i = 0; i < no_OneSimplices; i++) {
	if (Simplices[i].back() == 0) {
		if (Simplices[i][0] == B1[count][0]) {
			B1[count].back()++;

		}

		else {
			B1.push_back({ Simplices[i][0], B1[count].back() + 1 });
			count++;
		}
	}
}

std::vector<std::vector<double>> B2;

for (int i = 0; i < (int)twoSimplices.size(); i++) {
	if (twoSimplices[i].back() == 1) {
		B2.push_back({ twoSimplices[i][0], 0 });
		break;
	}

}
int pointer = 0;

for (int i = 0; i < (int) twoSimplices.size(); i++) {
	if (twoSimplices[i].back() == 1) {
		if (twoSimplices[i][0] == B2[pointer][0]) {
			B2[pointer].back()++;

		}

		else {
			B2.push_back({ twoSimplices[i][0], B2[pointer].back() + 1 });
			pointer++;
		}
	}
}


for (int i = 0; i < (int) B2.size(); i++) {
	for (int j = 0; j < (int) B1.size(); j++) {
		if (B2[i][0] == B1[j][0]) {
			B1[j].back() -= B2[i].back();
		}
	}
}
std::cout << "B1 = \n" << std::endl;
for (int i = 0; i < (int) B1.size(); i++) {
	std::cout << "( " << B1[i][0] << " , " << B1[i][1] << " ) ---> ";
}
std::cout << "\n\n";
}

void VietorisRips::PrintSimplex(int type) {
	if (type == 1) {
		std::cout << " The One Simplices are " << std::endl;
		PrintSimplices(Simplices, 1);
	}
	else if (type == 2){
		std::cout << " The Two Simplices are " << std::endl;
		PrintSimplices(twoSimplices , 2);
	}
}