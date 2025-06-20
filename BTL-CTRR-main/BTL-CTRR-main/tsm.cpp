#include "tsm.h"
// Hàm tìm index đỉnh
int getVindex(char v, const vector<char>& sv) {
    for (size_t i = 0; i < sv.size(); ++i)
        if (sv[i] == v) return (int)i;
    return -1;
}

// Hàm trả lại ký tự từ index
char getCharfromidx(int i, const vector<char>& sv) {
    if (i >= 0 && (size_t)i < sv.size()) return sv[i];
    return '\0';
}

void sortAscending(vector<char>& arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
       for (size_t j = i + 1; j < arr.size(); ++j) {
          if (arr[i] > arr[j]) {
                char t = arr[i]; 
                arr[i] = arr[j]; 
                arr[j] = t;
            }  
       }
    }
}

void uniqueInplace(vector<char>& arr) {
    if (arr.empty()) return;
    size_t write = 1;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] != arr[write - 1]) 
            arr[write++] = arr[i];
    }
    arr.resize(write);
}

void reverseVector(vector<char>& arr) {
    size_t l = 0;
    size_t r = arr.size();
    if (r == 0) return;
    r--;
    while (l < r) {
        char t = arr[l]; 
        arr[l] = arr[r]; 
        arr[r] = t;
        l++; 
        r--;
    }
}

string Traveling(int edgeList[][3], int numEdges, char startVertex) {
    // Lấy sortedV
    vector<char> sortedV;
    if (numEdges > 0) {
        for (int i = 0; i < numEdges; ++i) {
            sortedV.push_back((char)edgeList[i][0]);
            sortedV.push_back((char)edgeList[i][1]);
        }
        sortAscending(sortedV);
        uniqueInplace(sortedV);
    }
    int n = (int)sortedV.size();
    if (n == 0) return "";

    int startidx = getVindex(startVertex, sortedV);
    if (startidx == -1) return "";
    if (n == 1 && sortedV[0] == startVertex)
        return string(1, startVertex) + " " + string(1, startVertex);

    // Khởi tạo ma trận kề
    vector<vector<int>> adj(n, vector<int>(n, INF));
    for (int i = 0; i < numEdges; ++i) {
        int ui = getVindex((char)edgeList[i][0], sortedV);
        int vi = getVindex((char)edgeList[i][1], sortedV);
        int w = edgeList[i][2];
        if (ui != -1 && vi != -1) 
            adj[ui][vi] = min(adj[ui][vi], w);
    }

    if (n < 20) {
        int total_state = (1 << n);
        vector<vector<int>> dp(total_state, vector<int>(n, INF));
        vector<vector<int>> parent(total_state, vector<int>(n, -1));
        dp[1 << startidx][startidx] = 0;

        for (int mask = 1; mask < total_state; ++mask) {
            for (int u = 0; u < n; ++u) {
                if (mask & (1 << u)) {
                    if (dp[mask][u] < INF) {
                        for (int v = 0; v < n; ++v) {
                            if (!(mask & (1 << v)) && adj[u][v] < INF) {
                                int next_mask = mask | (1 << v);
                                int cand = dp[mask][u] + adj[u][v];
                                if (cand < dp[next_mask][v]) {
                                    dp[next_mask][v] = cand;
                                    parent[next_mask][v] = u;
                                }
                            }
                        }
                    }
                }
            }
        }

        int final_mask = total_state - 1;
        int min_cost = INF, last_v = -1;
        for (int u = 0; u < n; ++u) {
            if (dp[final_mask][u] < INF && adj[u][startidx] < INF) {
                int cand = dp[final_mask][u] + adj[u][startidx];
                if (cand < min_cost) {
                    min_cost = cand;
                    last_v = u;
                }
            }
        }
        if (min_cost == INF) return "";

        // Tìm đường đi
        vector<char> path;
        int curr_mask = final_mask, curr_v = last_v;
        while (curr_v != -1) {
            path.push_back(getCharfromidx(curr_v, sortedV));
            int prev_v = parent[curr_mask][curr_v];
            curr_mask ^= (1 << curr_v);
            curr_v = prev_v;
        }
        reverseVector(path);

        // Build chuỗi kết quả
        string res = "";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) res += " ";
            res += path[i];
        }
        res += " " + string(1, startVertex);
        return res;
    }
    else {
        vector<int> best;
        int min_cost = INF;
        for (int i = 0; i < n; ++i) {
            vector<int> curr;
            vector<bool> visited(n, false);
            int curr_cost = 0, curr_vi = i;
            curr.push_back(curr_vi);
            visited[curr_vi] = true;
            for (int cnt = 0; cnt < n - 1; ++cnt) {
                int hang_xom_i = -1, min_dist = INF;
                for (int next_vi = 0; next_vi < n; ++next_vi) {
                    if (!visited[next_vi] && adj[curr_vi][next_vi] < min_dist) {
                        min_dist = adj[curr_vi][next_vi];
                        hang_xom_i = next_vi;
                    }
                }
                if (hang_xom_i != -1) {
                    curr_cost += min_dist;
                    curr_vi = hang_xom_i;
                    visited[curr_vi] = true;
                    curr.push_back(curr_vi);
                }
                else {
                    curr_cost = INF;
                    break;
                }
            }
            if (curr_cost < INF && adj[curr_vi][i] < INF) {
                curr_cost += adj[curr_vi][i];
                if (curr_cost < min_cost) {
                    min_cost = curr_cost;
                    best = curr;
                }
            }
        }
        if (best.empty()) return "";

        // Xoay cho startidx nằm đầu path
        size_t bpos = 0;
        for (size_t i = 0; i < best.size(); ++i)
            if (best[i] == startidx) { bpos = i; break; }

        string res = "";
        for (size_t i = 0; i < best.size(); ++i) {
            int idx = (bpos + i) % best.size();
            if (i > 0) res += " ";
            res += getCharfromidx(best[idx], sortedV);
        }
        res += " " + string(1, startVertex);
        return res;
    }
}
