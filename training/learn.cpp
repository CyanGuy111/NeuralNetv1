#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

using namespace std;

const float N = 0.5;

vector<vector<float>> w01(16, vector<float>(784)), w12(16, vector<float>(16)), w23(10, vector<float>(16));
vector<float> b1(16), b2(16), b3(10);
vector<float> a0(784), a1(16), a2(16), a3(10);
vector<float> z1(16), z2(16), z3(10);
vector<vector<float>> gw01(16, vector<float>(784)), gw12(16, vector<float>(16)), gw23(10, vector<float>(16));
vector<float> ga1(16), ga2(16);
vector<float> gb1(16), gb2(16), gb3(10);
int bs;

vector<float> dotMulti(vector<vector<float>> w, vector<float> a){
    int m = w[0].size();
    int n = w.size();
    vector<float> c(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            c[i] += a[j] * w[i][j];
        }
    }
    return c;
}

vector<float> addition(vector<float> a, vector<float> b){
    vector<float> c = a;
    for(int i = 0; i < a.size(); i++){
        c[i] += b[i];
    }
    return c;
}

vector<float> sigmoid(vector<float> a){
    vector<float> c(a.size());
    for(int i = 0; i < a.size(); i++){
        c[i] = 1 / (1 + exp((a[i] * (float)(-1))));
    }
    return c;
}

float dSigmoid(float a){
    float c = 1 / (1 + exp((a * (float)(-1))));
    return (c * (1 - c));
}

float cost(vector<float> a, int des){
    float c = 0;
    for(int i = 0; i < 10; i++){
        if (i == des){
            c += (1 - a[i]) * (1 - a[i]);
        }
        else c += (a[i]) * (a[i]);
    }
    return c;
}

vector<vector<float>> learn(vector<vector<float>> a, vector<vector<float>> g){
    vector<vector<float>> c = a;
    for(int i = 0; i < a.size(); i++){
        for(int j = 0; j < a[0].size(); j++){
            c[i][j] -= (N * g[i][j] / bs);
        }
    }
    return c;
}

vector<float> learn(vector<float> a, vector<float> g){
    vector<float> c = a;
    for(int i = 0; i < a.size(); i++){
        c[i] -= (N * g[i] / bs);
    }
    return c;
}

string itos(int n){
    if (n == 0) return "0";
    string s1;
    while(n > 0){
        s1 += (n % 10 + '0');
        n /= 10;
    }
    reverse(s1.begin(), s1.end());
    return s1;
}

int main(){
    iostream::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    bs = 1;
    for(int tmp = 1; tmp <= t; tmp++){
        filesystem::current_path("C:/neuralnetithink/WeightAndBias2");
        ifstream input;
        ofstream output;
        string name3 = "WeightBias_" + itos(tmp - 1) + ".txt";
        input.open(name3);
        for(int i = 0; i < 16; i++){
            for(int j = 0; j <= 784; j++){
                if(j == 784){
                    input >> b1[i];
                    gb1[i] = 0;
                }
                else{
                    input >> w01[i][j];
                    gw01[i][j] = 0;
                }
            }
        }
        for(int i = 0; i < 16; i++){
            for(int j = 0; j <= 16; j++){
                if(j == 16){
                    input >> b2[i];
                    gb2[i] = 0;
                }
                else{
                    input >> w12[i][j];
                    gw12[i][j] = 0;
                }
            }
        }
        for(int i = 0; i < 10; i++){
            for(int j = 0; j <= 16; j++){
                if(j == 16){
                    input >> b3[i];
                    gb3[i] = 0;
                }
                else{
                    input >> w23[i][j];
                    gw23[i][j] = 0;
                }
            }
        }
        input.close();
        filesystem::current_path("C:/neuralnetithink/test_batch_of_1");
        string name2 = itos(tmp) + ".txt";
        input.open(name2);
        for(int ii = 0; ii < bs; ii++){
            int ans;
            input >> ans;
            for(int i = 0; i < 784; i++){
                input >> a0[i];
            }
            a1 = sigmoid(z1 = addition(dotMulti(w01, a0), b1));
            a2 = sigmoid(z2 = addition(dotMulti(w12, a1), b2));
            a3 = sigmoid(z3 = addition(dotMulti(w23, a2), b3));
            //w23 gradient
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 16; j++){
                    int y = (i == ans) ? 1 : 0;
                    gw23[i][j] += (a2[j] * dSigmoid(z3[i]) * 2 * (a3[i] - y));
                }
            }
            //b3 gradient
            for(int i = 0; i < 10; i++){
                int y = (i == ans) ? 1 : 0;
                gb3[i] += (dSigmoid(z3[i]) * 2 * (a3[i] - y));
            }
            //a2 gradient
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 16; j++){
                    int y = (i == ans) ? 1 : 0;
                    ga2[j] += (w23[i][j] * dSigmoid(z3[i]) * 2 * (a3[i] - y));
                }
            }
            //w12 gradient
            for(int i = 0; i < 16; i++){
                for(int j = 0; j < 16; j++){
                    gw12[i][j] += (a1[j] * dSigmoid(z2[i]) * ga2[i]);
                }
            }
            //b2 gradient
            for(int i = 0; i < 16; i++){
                gb2[i] += (dSigmoid(z2[i]) * ga2[i]);
            }
            //a1 gradient
            for(int i = 0; i < 16; i++){
                for(int j = 0; j < 16; j++){
                    ga1[j] += (w12[i][j] * dSigmoid(z2[i]) * ga2[i]);
                }
            }
            //w01 gradient
            for(int i = 0; i < 16; i++){
                for(int j = 0; j < 784; j++){
                    gw01[i][j] += (a0[j] * dSigmoid(z1[i]) * ga1[i]);
                }
            }
            //b1 gradient
            for(int i = 0; i < 16; i++){
                gb1[i] += (dSigmoid(z1[i]) * ga1[i]);
            }
            //reset activation gradient
            for(int i = 0; i < 16; i++){
                ga1[i] = ga2[i] = 0;
            }
        }
        input.close();
        filesystem::current_path("C:/neuralnetithink/WeightAndBias2");
        string name = "WeightBias_" + itos(tmp) + ".txt";
        output.open(name);
        w01 = learn(w01, gw01);
        w12 = learn(w12, gw12);
        w23 = learn(w23, gw23);
        b1 = learn(b1, gb1);
        b2 = learn(b2, gb2);
        b3 = learn(b3, gb3);
        for(int i = 0; i < 16; i++){
            for(int j = 0; j <= 784; j++){
                if(j == 784) output << b1[i] << " ";
                else output << w01[i][j] << " ";
            }
            output << "\n";
        }
        output << "\n";
        for(int i = 0; i < 16; i++){
            for(int j = 0; j <= 16; j++){
                if(j == 16) output << b2[i] << " ";
                else output << w12[i][j] << " ";
            }
            output << "\n";
        }
        output << "\n";
        for(int i = 0; i < 10; i++){
            for(int j = 0; j <= 16; j++){
                if(j == 16) output << b3[i] << " "; 
                else output << w23[i][j] << " ";
            }
            output << "\n";
        }
        output.close();
        // cout << tmp << "\n";
    }
    return 0;
}