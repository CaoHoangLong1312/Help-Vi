#ifndef TSM_H
#define TSM_H

#include "string"
#include "vector"
using namespace std;

const int INF = 1e9;

int getVindex(char v, const vector<char>& sv);
char getCharfromidx(int i, const vector<char>& sv);
void sortAscending(vector<char>& arr);
void uniqueInplace(vector<char>& arr);
void reverseVector(vector<char>& arr);
string Traveling(int edgeList[][3], int numEdges, char startVertex);

#endif
    