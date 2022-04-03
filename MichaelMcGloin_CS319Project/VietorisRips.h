// Author : Michael McGloin
// Student ID: 18357636
// Module : CS319
// About : This is a programme that calculates topological features of a set of points 
// and outputs a barcode of the features. I have included the files Eight.txt , Torus.txt , Sphere.txt
// and Cluster.txt to test out the programme.
// However for larger radii it takes a bit to compute so I reccommend keeping the maximum radius less than 5


#pragma once
#include <vector>


class VietorisRips
{
private:
	double maxEpsilon;
	int Rows;
	int Cols;
	double** AdjMatrix;
	double** BoundaryMatrix;
	double** boundaryMatrixTwoSimplices;
	int featuresCreated;
	int featuresDestroyed;
	std::vector<std::vector<double>> Simplices;
	std::vector<std::vector<double>> twoSimplices;
	void findTwoSimplices(std::vector<std::vector<double>> vec);
	double max_distance_ab(std::vector<double> a, std::vector<double> b, std::vector<double> c);
	void BubbleSort(std::vector<std::vector<double>>& Simplex);
	void PrintSimplices(std::vector<std::vector<double>>& Simplex , int type);
	void createBoundaryMatrix();
	void createBoundaryMatrixTwoSimplices();
	void reduceBoundaryMatrix(double** matrix, int rows, int columns);
	void AddBoundaryRows(double** matrix ,int rowOne, int rowTwo , int noRows);
	bool isLow(double** matrix, int row, int col, int size);
	void PrintMatrix(double** matrix, int rows , int columns);
	double distance_ab(std::vector<double > a, std::vector<double> b);

public:
	int no_points;
	int no_OneSimplices;
	int no_twoSimplices;
	VietorisRips(int rows, int cols, std::vector<std::vector<double>>& vec);
	void PrintSimplex(int type);
	void createSimplex(std::vector<std::vector<double>>& vec);
	void findFeatures();
	void Barcodes();

};

