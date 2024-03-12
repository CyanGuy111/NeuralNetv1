#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

using namespace std;

vector<vector<float>> w01(16, vector<float>(784)), w12(16, vector<float>(16)), w23(10, vector<float>(16));
vector<float> b1(16), b2(16), b3(10);
vector<float> a0(784), a1(16), a2(16), a3(10);
vector<float> z1(16), z2(16), z3(10);
vector<vector<float>> gw01(16, vector<float>(784)), gw12(16, vector<float>(16)), gw23(10, vector<float>(16));
vector<float> ga1(16), ga2(16);
vector<float> gb1(16), gb2(16), gb3(10);
int bs;
int ans;
vector<float> dotMulti(vector<vector<float>> w, vector<float> a){
    int m = w[0].size();
    int n = w.size();
    vector<float> c(n, 0);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            c[i] += (a[j] * w[i][j]);
        }
    }
    return c;
}

vector<float> addition(vector<float> a, vector<float> b){
    vector<float> c(a.size());
    for(int i = 0; i < a.size(); i++){
        c[i] = a[i] + b[i];
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
    int t = 10000;
    int num = 1000;
    int cnt = 0;
    bs = 1;
    filesystem::current_path("C:/neuralnetithink/WeightAndBias2");
    ifstream input;
    string name3 = "WeightBias_2980.txt";
    input.open(name3);
    for(int i = 0; i < 16; i++){
        for(int j = 0; j <= 784; j++){
            if(j == 784){
                input >> b1[i];
            }
            else{
                input >> w01[i][j];
            }
        }
    }
    for(int i = 0; i < 16; i++){
        for(int j = 0; j <= 16; j++){
            if(j == 16){
                input >> b2[i];
            }
            else{
                input >> w12[i][j];
            }
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j <= 16; j++){
            if(j == 16){
                input >> b3[i];
            }
            else{
                input >> w23[i][j];
            }
        }
    }
    input.close();
    for(int tmp = t - num + 1; tmp <= t; tmp++){
        filesystem::current_path("C:/neuralnetithink/test_batch_of_1");
        string name2 = itos(tmp) + ".txt";
        // cout << name2 << "\n";
        input.open(name2);
        for(int ii = 0; ii < bs; ii++){
            input >> ans;
            // cout << ans << "\n";
            for(int i = 0; i < 784; i++){
                a0[i] = 0;
                input >> a0[i];
            }
            a1 = sigmoid(addition(dotMulti(w01, a0), b1));
            a2 = sigmoid(addition(dotMulti(w12, a1), b2));
            a3 = sigmoid(addition(dotMulti(w23, a2), b3));
            // for(int i = 0; i < 10; i++){
            //     cout << i << " " << a3[i] << "\n";
            // }
            int con = 0;
            float conc = a3[0];
            for(int i = 1; i < 10; i++){
                if(conc < a3[i]){
                    con = i;
                    conc = a3[i];
                }
            }
            // cout << con << "\n";
            if (con == ans) cnt++;
        }
        input.close();
    }
    cout << ((float)(cnt) / (float)(num) * (float)100) << "%";
    return 0;
}