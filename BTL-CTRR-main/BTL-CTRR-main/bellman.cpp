#include "bellman.h"

void BF(int edgeList[][3], int numEdges, char startVertex, int valueArray[], int prevArray[]) {
    // Lấy danh sách các đỉnh phân biệt
    vector<char> sortedV; 
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        // Thêm u nếu chưa có
        bool found = false;
        for (size_t j = 0; j < sortedV.size(); j++)
            if (sortedV[j] == u) { 
                found = true; 
                break; 
        }
        
        if (! found) sortedV.push_back(u);
        // Thêm v nếu chưa có
        found = false;
        for (size_t j = 0; j < sortedV.size(); j++)
            if (sortedV[j] == v) { 
                found = true; 
                break; 
        }
        
        if (! found) sortedV.push_back(v);
    }
    
    // Sort
    for (size_t i = 0; i < sortedV.size(); i++)
        for (size_t j = i + 1; j < sortedV.size(); j++)
            if (sortedV[i] > sortedV[j]) {
                char tmp = sortedV[i]; 
                sortedV[i] = sortedV[j]; 
                sortedV[j] = tmp;
            }
            
    int n = sortedV.size();
    if (n == 0) return;

    // Hàm tìm index của đỉnh v trong sortedV
    auto getVindex = [&](char v) -> int {
        for (int i = 0; i < n; i++)
            if (sortedV[i] == v) return i;
        return -1;
    };

    // Khởi tạo dist và prev
    vector<long long> dist(n);
    for (int i = 0; i < n; i++)
        dist[i] = (valueArray[i] == -1) ? INF : valueArray[i];
        
    int startidx = getVindex(startVertex);
    if (startidx == -1) {
        prevArray[n - 1] = 0; // không tìm được đỉnh xuất phát
        return;
    }
    dist[startidx] = 0;

    vector<long long> next_dist = dist;
    vector<int> next_prev(n);
    for (int i = 0; i < n; i++) next_prev[i] = prevArray[i];

    // Relaxation 
    for (int ui = 0; ui < n; ui++) {
        if (dist[ui] != INF) {
            char u = sortedV[ui];
            for (int i = 0; i < numEdges; i++) {
                if ((char)edgeList[i][0] == u) {
                    char v = (char)edgeList[i][1];
                    long long w = edgeList[i][2];
                    int vi = getVindex(v);
                    if (vi != -1 && dist[ui] + w < next_dist[vi]) {
                        next_dist[vi] = dist[ui] + w;
                        next_prev[vi] = ui;
                    }
                }
            }
        }
    }

    // Gán lại kết quả vào valueArray, prevArray
    for (int i = 0; i < n; i++) {
        valueArray[i] = (next_dist[i] == INF) ? -1 : (int)next_dist[i];
        prevArray[i] = next_prev[i];
    }
}

// Hàm tìm chỉ số đỉnh
int getVindex(char v, const vector<char>& sv) {
    for (size_t i = 0; i < sv.size(); i++)
        if (sv[i] == v) return (int)i;
    return -1;
}

// Hàm trả về đỉnh theo index
char getCharfromindex(int vi, const vector<char>& sv) {
    if (vi >= 0 && (size_t)vi < sv.size())
        return sv[vi];
    return '\0';
}

string BF_Path(int edgeList[][3], int numEdges, char startVertex, char endVertex) {
    // Tìm sortedV
    vector<char> sortedV;
    for (int i = 0; i < numEdges; i++) {
        char u = (char)edgeList[i][0];
        char v = (char)edgeList[i][1];
        
        bool found = false;
        for (size_t j = 0; j < sortedV.size(); j++)
            if (sortedV[j] == u) { 
                found = true; 
                break; 
            }
        if (!found) sortedV.push_back(u);
        
        found = false;
        for (size_t j = 0; j < sortedV.size(); j++)
            if (sortedV[j] == v) { 
                found = true; 
                break; 
            }
        if (!found) sortedV.push_back(v);
    }
    
    // Sort
    for (size_t i = 0; i < sortedV.size(); i++)
        for (size_t j = i + 1; j < sortedV.size(); j++)
            if (sortedV[i] > sortedV[j]) {
                char tmp = sortedV[i]; 
                sortedV[i] = sortedV[j]; 
                sortedV[j] = tmp;
            }
    if (sortedV.size() == 0)
        return "";

    if (startVertex == endVertex) {
        bool exist = false;
        for (size_t i = 0; i < sortedV.size(); i++)
            if (sortedV[i] == startVertex) { 
                exist = true; 
                break; 
            }
        if (exist) return string(1, startVertex);
        return "";
    }

    int startidx = getVindex(startVertex, sortedV);
    int endidx = getVindex(endVertex, sortedV);
    if (startidx == -1 || endidx == -1)
        return "";

    vector<int> curr_value(sortedV.size(), -1);
    vector<int> curr_prev(sortedV.size(), -1);
    curr_value[startidx] = 0;

    // Lặp Bellman-Ford
    for (int i = 0; i < (int)sortedV.size() - 1; i++) {
        vector<int> prev_value = curr_value;
        BF(edgeList, numEdges, startVertex, curr_value.data(), curr_prev.data());

        bool no_change = true;
        for (size_t j = 0; j < sortedV.size(); j++)
            if (prev_value[j] != curr_value[j]) { 
                no_change = false; 
                break; 
            }
        if (no_change)
            break;
    }
    if (curr_value[endidx] == -1)
        return "";

    // Tìm đường đi ngược lại
    vector<char> path;
    int curr = endidx;
    while (curr != -1) {
        char currc = getCharfromindex(curr, sortedV);
        if (currc == '\0') return "";
        path.push_back(currc);
        if (curr == startidx)
            break;
        int preidx = curr_prev[curr];
        if (preidx == -1 && curr != startidx)
            return ""; // Đường đi bị đứt
        if (preidx == curr && curr != startidx)
            return ""; // Chu trình
        curr = preidx;
    }
    if (path.empty() || path.back() != startVertex)
        return "";
        
    // Đảo ngược path
    for (size_t i = 0; i < path.size() / 2; i++) {
        char tmp = path[i]; 
        path[i] = path[path.size() - 1 - i]; 
        path[path.size() - 1 - i] = tmp;
    }
    
    string s = "";
    bool first = true;
    for (size_t i = 0; i < path.size(); i++) {
        if (!first) s += " ";
        s += path[i];
        first = false;
    }
    return s;
}
