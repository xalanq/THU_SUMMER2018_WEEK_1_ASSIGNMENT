#include <iostream>
#include <algorithm>
#include <cmath>

#include "Core.h"

namespace Core {

const int N = 8;
const int M = 200;
const double eps = 1e-6;

int n, tot, inID[2], outID[3];
double inFlow[2];
double mat[M][M], R[M], V[M];
vector<int> adjIn[M], adjOut[M];
int tp[M];
int row, col;

int getID(int x, int y) {
    return x * n + y;
}

double Left(int x, int y, int& ID) {
    if (x == 0)
        return 0;
    ID = getID(x - 1, y);
    return R[n * (n - 1) + (x - 1) * n + y];
}

double Right(int x, int y, int& ID) {
    if (x == n - 1)
        return 0;
    ID = getID(x + 1, y);
    return R[n * (n - 1) + x * n + y];
}

double Up(int x, int y, int& ID) {
    if (y == 0) {
        if (x != inID[0] && x != inID[1])
            return 0;
        int tmp = (x == inID[1]);
        ID = n * n + tmp;
        return R[tot - 5 + tmp];
    }
    ID = getID(x, y - 1);
    return R[x * (n - 1) + y - 1];
}

double Down(int x, int y, int& ID) {
    if (y == n - 1) {
        if (x != outID[0] && x != outID[1] && x != outID[2])
            return 0;
        int tmp = (x == outID[0] ? 0 : (x == outID[1] ? 1 : 2));
        ID = n * n + 3;
        return R[tot - 3 + tmp];
    }
    ID = getID(x, y + 1);
    return R[x * (n - 1) + y];
}

void initMatrix() {
    row = n * n + 2;
    col = row;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int ID, itID = getID(i, j);
            double r;
            if ((r = Left(i, j, ID)) > eps)
                mat[itID][ID] -= 1.0 / r, mat[itID][itID] += 1.0 / r;
            if ((r = Right(i, j, ID)) > eps)
                mat[itID][ID] -= 1.0 / r, mat[itID][itID] += 1.0 / r;
            if ((r = Up(i, j, ID)) > eps)
                mat[itID][ID] -= 1.0 / r, mat[itID][itID] += 1.0 / r;
            if ((r = Down(i, j, ID)) > eps)
                mat[itID][ID] -= 1.0 / r, mat[itID][itID] += 1.0 / r;
        }
    mat[n * n][n * n + 2] -= inFlow[0];
    mat[n * n][n * n] += 1.0 / R[tot - 5];
    mat[n * n][getID(inID[0], 0)] -= 1.0 / R[tot - 5];

    mat[n * n + 1][n * n + 2] -= inFlow[1];
    mat[n * n + 1][n * n + 1] += 1.0 / R[tot - 4];
    mat[n * n + 1][getID(inID[1], 0)] -= 1.0 / R[tot - 4];
}

void guass() {
    for (int o = 0; o < col; ++o) {
        bool ok = false;
        for (int i = o; i < row; i++)
            if (abs(mat[i][o]) > eps) {
                ok = true;
                for (int j = 0; j <= col; ++j)
                    swap(mat[i][j], mat[o][j]);
                break;
            }
        if (!ok)
            continue;
        for (int i = 0; i < row; ++i)
            if (abs(mat[i][o]) > eps && i != o) {
                double tmp = mat[i][o] / mat[o][o];
                for (int j = 0; j <= col; ++j)
                    mat[i][j] -= mat[o][j] * tmp;
            }
    }
}

void work(int _n, const vector<double> &length, const vector<double> &area, int i1, int i2, int o1, int o2, int o3, double initSpeed, double concen1, double concen2, vector<double> &speed, vector<double> &concen) {
    // printf("go\n");
    n = _n;
    tot = (n - 1) * n * 2 + 5;
    inID[0] = i1;
    inID[1] = i2;
    outID[0] = o1;
    outID[1] = o2;
    outID[2] = o3;
    inFlow[0] = initSpeed * area[tot - 5];
    inFlow[1] = initSpeed * area[tot - 4];

    // speed
    
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            mat[i][j] = 0;
    for (int i = 0; i < tot; ++i)
        R[i] = length[i];

    initMatrix();
    guass();

    for (int i = 0; i < col; ++i)
        if (abs(mat[i][i]) > eps)
            V[i] = mat[i][col] / mat[i][i];
        else {
            if (abs(mat[i][col]) > eps) {
                // puts("gg");
                speed.clear();
                speed.resize(tot, 0);
                concen.clear();
                concen.resize(tot, 0);
                concen[tot - 5] = concen1;
                concen[tot - 4] = concen2;
                return;
            }
            V[i] = 0;
        }

    speed.clear();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n - 1; ++j)
            if (R[i * (n - 1) + j] > eps)
                speed.push_back(V[getID(i, j + 1)] - V[getID(i, j)]);
            else
                speed.push_back(0);

    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n; ++j)
            if (R[n * (n - 1) + i * n + j] > eps)
                speed.push_back(V[getID(i + 1, j)] - V[getID(i, j)]);
            else
                speed.push_back(0);

    speed.push_back(V[getID(inID[0], 0)] - V[n * n]);
    speed.push_back(V[getID(inID[1], 0)] - V[n * n + 1]);
    speed.push_back(abs(V[getID(outID[0], n - 1)]));
    speed.push_back(abs(V[getID(outID[1], n - 1)]));
    speed.push_back(abs(V[getID(outID[2], n - 1)]));

    for (int i = 0; i < tot; ++i)
        if (R[i] > eps)
            speed[i] = speed[i] / R[i]; // flow

    // concen

    for (int i = 0; i < tot; ++i)
        concen.push_back(0);

    for (int i = 0; i < n * n; ++i) {
        tp[i] = i;
        adjIn[i].clear();
        adjOut[i].clear();
    }

    concen[tot - 5] = concen1;
    concen[tot - 4] = concen2;

    for (int i = 0; i < n * n - n; i += n - 1) {
        for (int j = 0; j < n - 1; ++j) {
            int id = i + j;
            if (abs(speed[id]) > eps) {
                int x = i / (n - 1);
                int y1 = j, y2 = j + 1;
                if (speed[id] > eps) {
                    adjOut[getID(x, y1)].push_back(id);
                    adjIn[getID(x, y2)].push_back(id);
                } else {
                    adjIn[getID(x, y1)].push_back(id);
                    adjOut[getID(x, y2)].push_back(id);
                }
            }
        }
    }

    for (int i = n * n - n; i < 2 * (n * n - n); i += n) {
        for (int j = 0; j < n; ++j) {
            int id = i + j;
            if (abs(speed[id]) > eps) {
                int x1 = (i - n * n + n) / n, x2 = x1 + 1;
                int y = j;
                if (speed[id] > eps) {
                    adjOut[getID(x1, y)].push_back(id);
                    adjIn[getID(x2, y)].push_back(id);
                } else {
                    adjIn[getID(x1, y)].push_back(id);
                    adjOut[getID(x2, y)].push_back(id);
                }
            }
        }
    }

    if (speed[tot - 5] > eps)
        adjIn[getID(inID[0], 0)].push_back(tot - 5);
    if (speed[tot - 4] > eps)
        adjIn[getID(inID[1], 0)].push_back(tot - 4);
    if (speed[tot - 3] > eps)
        adjOut[getID(outID[0], n - 1)].push_back(tot - 3);
    if (speed[tot - 2] > eps)
        adjOut[getID(outID[1], n - 1)].push_back(tot - 2);
    if (speed[tot - 1] > eps)
        adjOut[getID(outID[2], n - 1)].push_back(tot - 1);


    auto cmp = [&](int i, int j) {
        return V[i] < V[j];
    };

    sort(tp, tp + n * n, cmp);

    for (int _ = 0; _ < n * n; ++_) {
        int id = tp[_];
        if (adjIn[id].size() + adjOut[id].size()) {
            // printf("id:%d, in:%d, out:%d\n", id, adjIn[id].size(), adjOut[id].size());
            bool flag = true;
            if (adjIn[id].size() == 2 && adjOut[id].size() == 2) {
                for (int i = 0; i < 2 && flag; ++i)
                    for (int j = 0; j < 2 && flag; ++j) {
                        int a = adjIn[id][i], b = adjOut[id][j];
                        int c = adjIn[id][1 - i], d = adjOut[id][1 - j];
                        if ((max(a, b) >= 2 * n * n - 2 * n || min(a, b) < n * n - n) &&
                            ((n * n - n <= a && a < 2 * n * n - 2 * n) ||
                             (n * n - n <= b && b < 2 * n * n - 2 * n)) &&
                            abs(speed[a]) > abs(speed[b])) {
                            concen[b] = concen[a];
                            concen[d] = (concen[a] * abs(speed[a]) + concen[c] * abs(speed[c]) - concen[b] * abs(speed[b])) / abs(speed[d]);
                            flag = false;
                        }
                    }
            }
            if (flag) {
                double f = 0, c = 0;
                for (int i = 0; i < int(adjIn[id].size()); ++i) {
                    int it = adjIn[id][i];
                    c += abs(speed[it]) * concen[it];
                    f += abs(speed[it]);
                }
                // printf("c:%.10f, f:%.10f\n", c, f);
                c /= f;
                for (int i = 0; i < int(adjOut[id].size()); ++i)
                    concen[adjOut[id][i]] = c;
            }
        }
    }

    // last

    for (int i = 0; i < tot; ++i)
        if (area[i] > eps)
            speed[i] /= area[i];

    for (int i = 0; i < tot; ++i)
        if (abs(speed[i]) < eps)
            speed[i] = 0;

    for (int i = 0; i < tot; ++i)
        if (abs(concen[i]) < eps)
            concen[i] = 0;

}

}
