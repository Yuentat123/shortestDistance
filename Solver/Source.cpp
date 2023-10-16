#include<iostream>
#include<fstream>
#include <algorithm> 
#include<vector>
#include<omp.h>
#include<float.h>

using namespace std;

struct Point {
	char name[20];
	float x;
	float y;
	char signal;
};

float dist(Point p, Point p1) { //Calculate the distance between two point
	return sqrt((p.x - p1.x) * (p.x - p1.x) + (p.y - p1.y) * (p.y - p1.y)); //Distance Formula
	                                                                        //=sqrt((x-x1)^2+(y-y2)^2)
}

void swap(Point* xp, Point* yp) { //Temporarily store point x and point y
	Point temp = *xp;
	*xp = *yp;
	*yp = temp;
}

int partition(Point p[], int low, int high) { 
	int pivot = p[high].signal;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		if (p[j].signal <= pivot) {
			i++;
			swap(&p[i], &p[j]);
		}
	}
	swap(&p[i + 1], &p[high]);
	return (i + 1);
}

void quickSort(Point p[], int low, int high) { // Categorize red point and green point using quickSort Algorithm
	if (low < high) {
		int pi = partition(p, low, high);

		quickSort(p, low, pi - 1);
		quickSort(p, pi + 1, high);
	}
}

double bruteforce1(Point P[], int n, int r, int ind[], int red[]) { //Bruteforce algorithm
	double sum1 = 0;
	sum1 += dist(P[0], P[ind[0]]); //Calculate the distance between first starting point and first green

	for (int i = 0; i < r; i++) { //Constraint:Green parcel must connect to red point
		sum1 += dist(P[ind[i]], P[red[i]]);
	}

	for (int i = 0; i < (r - 1); i++) { //Red point to Green parcel
		sum1 += dist(P[red[i]], P[ind[i + 1]]);
	}

	sum1 += dist(P[red[r - 1]], P[0]); // Calculate the total distance between last red point and the starting point
	return sum1;
}

double bruteforce2(Point P[], int n, int r, int ind[], int red[]) { //Bruteforce algorithm
	double sum2 = 0;
	sum2 += dist(P[0], P[ind[0]]); //Calculate the distance between first starting point and first green

	for (int i = 0; i < r; i++) { //Constraint:Green parcel must connect to red point
		sum2 += dist(P[ind[i]], P[red[i]]);
	}

	for (int i = 0; i < (r - 1); i++) { //Red point to Green parcel
		sum2 += dist(P[red[i]], P[ind[i + 1]]);
	}

	sum2 += dist(P[red[r - 1]], P[0]); // Calculate the total distance between last red point and the starting point
	return sum2;
}

double bruteforce3(Point P[], int n, int r, int ind[], int red[]) { //Bruteforce algorithm
	double sum3 = 0;
	sum3 += dist(P[0], P[ind[0]]); //Calculate the distance between first starting point and first green

	for (int i = 0; i < r; i++) { //Constraint:Green parcel must connect to red point
		sum3 += dist(P[ind[i]], P[red[i]]);
	}

	for (int i = 0; i < (r - 1); i++) { //Red point to Green parcel
		sum3 += dist(P[red[i]], P[ind[i + 1]]);
	}

	sum3 += dist(P[red[r - 1]], P[0]); // Calculate the total distance between last red point and the starting point
	return sum3;
}

double bruteforce4(Point P[], int n, int r, int ind[], int red[]) { //Bruteforce algorithm
	double sum4 = 0;
	sum4 += dist(P[0], P[ind[0]]); //Calculate the distance between first starting point and first green

	for (int i = 0; i < r; i++) { //Constraint:Green parcel must connect to red point
		sum4 += dist(P[ind[i]], P[red[i]]);
	}

	for (int i = 0; i < (r - 1); i++) { //Red point to Green parcel
		sum4 += dist(P[red[i]], P[ind[i + 1]]);
	}

	sum4 += dist(P[red[r - 1]], P[0]); // Calculate the total distance between last red point and the starting point
	return sum4;
}


int main(void) {
	vector<string> output1;
	vector<string> output2;
	vector<string> output3;
	vector<string> output4;
	ifstream inFile;
	inFile.open("tsp.txt");
	Point* a = NULL;
	const int number_of_input = 100;
	int totalpoint;

	if (!inFile) 
	{
		cout << "File does not exist." << endl;
	}
	else
	{
		a = new Point[number_of_input];
		int i = 0;
		while (!inFile.eof()) { //Read the tsp file until end of file
			inFile >> a[i].name >> a[i].x >> a[i].y; 
			i++;
		}

		inFile.clear();
		inFile.close();

		totalpoint = i - 1; //Record the total point, (i-1) to remove the empty line
		int count = 0;

		a[0].signal = 'O';//Depot
		for (int i = 1; i < totalpoint; i++) {
			if (a[i].name[1] == 'o') {
				a[i].signal = 'R';//Red point
				count++; //Calculate total red point
			}
			else
				a[i].signal = 'G'; //Green parcel
		}

		quickSort(a, 1, (totalpoint - 1)); //Call quickSort function

		ofstream outfile;
		outfile.open("solution.txt");

		int* index1 = new int[totalpoint - count]; //Dynamic array to store green parcel
		int* redindex1 = new int[count];  //Dynamic array to store red point
		int* index2 = new int[totalpoint - count]; //Dynamic array to store green parcel
		int* redindex2 = new int[count];  //Dynamic array to store red point
		int* index3 = new int[totalpoint - count]; //Dynamic array to store green parcel
		int* redindex3 = new int[count];  //Dynamic array to store red point
		int* index4 = new int[totalpoint - count]; //Dynamic array to store green parcel
		int* redindex4 = new int[count];  //Dynamic array to store red point


		double min1 = DBL_MAX, min2 = DBL_MAX, min3 = DBL_MAX, min4 = DBL_MAX;

#pragma omp parallel sections num_threads(4)
		{
#pragma omp section
			{
				double sum1 = 0;
				for (int i = 1; i < totalpoint - count; i++) index1[i - 1] = i; //Calculate green parcels

				int j1 = 0;
				for (int i = totalpoint - count; i < totalpoint; i++) redindex1[j1++] = i; //Calculate red points
				do {
					sum1 = bruteforce1(a, totalpoint, count, index1, redindex1); //Call brute force function

					if (sum1 < min1) { //Find the best minimal path
						min1 = sum1;
						output1.~vector(); //Record the solution path
						output1.push_back(a[0].name);

						for (int i = 0; i < count; i++) {
							output1.push_back(a[index1[i]].name);
							output1.push_back(a[redindex1[i]].name);
						}
						output1.push_back(a[0].name);
					}
				} while (next_permutation(index1, index1 + totalpoint - count - 1) || next_permutation(redindex1, redindex1 + count)); //Permutation algorithm																									  //(next_permutation(start,end)
			}
#pragma omp section
			{
				double sum2 = 0;
				for (int i = 1; i < totalpoint - count; i++) index2[i - 1] = i; //Calculate green parcels

				int j2 = 0;
				for (int i = totalpoint - count; i < totalpoint; i++) redindex2[j2++] = i; //Calculate red points
				do {
					sum2 = bruteforce2(a, totalpoint, count, index2, redindex2); //Call brute force function

					if (sum2 < min2) { //Find the best minimal path
						min2 = sum2;
						output2.~vector(); //Record the solution path
						output2.push_back(a[0].name);

						for (int i = 0; i < count; i++) {
							output2.push_back(a[index2[i]].name);
							output2.push_back(a[redindex2[i]].name);
						}
						output2.push_back(a[0].name);
					}
				} while (next_permutation(index2, index2 + totalpoint - count - 1) || next_permutation(redindex2, redindex2 + count)); //Permutation algorithm																									  //(next_permutation(start,end)
			}
#pragma omp section
			{
				double sum3 = 0;
				for (int i = 1; i < totalpoint - count; i++) index3[i - 1] = i; //Calculate green parcels

				int j3 = 0;
				for (int i = totalpoint - count; i < totalpoint; i++) redindex3[j3++] = i; //Calculate red points
				do {
					sum3 = bruteforce3(a, totalpoint, count, index3, redindex3); //Call brute force function

					if (sum3 < min3 ){ //Find the best minimal path
						min3 = sum3;
						output3.~vector(); //Record the solution path
						output3.push_back(a[0].name);

						for (int i = 0; i < count; i++) {
							output3.push_back(a[index3[i]].name);
							output3.push_back(a[redindex3[i]].name);
						}
						output3.push_back(a[0].name);

					}
				} while (next_permutation(index3, index3 + totalpoint - count - 1) || next_permutation(redindex3, redindex3 + count)); //Permutation algorithm					
			}
#pragma omp section
			{
				double sum4 = 0;
				for (int i = 1; i < totalpoint - count; i++) index4[i - 1] = i; //Calculate green parcels

				int j1 = 0;
				for (int i = totalpoint - count; i < totalpoint; i++) redindex4[j1++] = i; //Calculate red points
				do {
					sum4 = bruteforce4(a, totalpoint, count, index4, redindex4); //Call brute force function

					if (sum4 < min4) { //Find the best minimal path
						min4 = sum4;
						output4.~vector(); //Record the solution path
						output4.push_back(a[0].name);

						for (int i = 0; i < count; i++) {
							output4.push_back(a[index4[i]].name);
							output4.push_back(a[redindex4[i]].name);
						}
						output4.push_back(a[0].name);
					}
				} while (next_permutation(index4, index4 + totalpoint - count - 1) || next_permutation(redindex4, redindex4+ count)); //Permutation algorithm																									  //(next_permutation(start,end)
			}
		}
#pragma omp barrier
		if ((min1 < min2) && (min1 < min3) && (min1 < min4))
		{
			for (int i = 0; i < output1.size(); i++) {
				outfile << output1[i] << '\n';
			}
			output1.clear();
			output2.clear();
			output3.clear();
			output4.clear();
			outfile.close();
		}
		else if ((min2 < min1) && (min2 < min3) && (min2 < min4))
		{
			for (int i = 0; i < output2.size(); i++) {
				outfile << output2[i] << '\n';
			}
			output1.clear();
			output2.clear();
			output3.clear();
			output4.clear();
			outfile.close();
		}

		else if ((min3 < min1) && (min3 < min2) && (min3 < min4))
		{
			for (int i = 0; i < output3.size(); i++) {
				outfile << output3[i] << '\n';
			}
			output1.clear();
			output2.clear();
			output3.clear();
			output4.clear();
			outfile.close();
		}

		else
		{
			for (int i = 0; i < output4.size(); i++) {
				outfile << output4[i] << '\n';
			}
			output1.clear();
			output2.clear();
			output3.clear();
			output4.clear();
			outfile.close();
		}

	}
	return 0;
}