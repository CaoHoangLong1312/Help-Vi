#ifndef BELLMAN_H
#define BELLMAN_H

#include <vector>
#include <string>

using namespace std;

const long long INF = 1e18;


void BF(int edgeList[][3], int numEdges, char startVertex, int valueArray[], int prevArray[]);

int getVindex(char v, const vector<char>& sv);
char getCharfromindex(int vi, const vector<char>& sv);
string BF_Path(int edgeList[][3], int numEdges, char startVertex, char endVertex);

#endif