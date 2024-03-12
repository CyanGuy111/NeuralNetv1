#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>

using namespace std;

string fileGen(int i){
    string ii;
    while(i > 0){
        ii += (i % 10) + '0';
        i /= 10;
    }
    reverse(ii.begin(), ii.end());
    return ii + ".txt";
}

vector<float> stringSplit(string s){
    vector<float> vf;
    string tmp;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == ','){
            float a = stof(tmp);
            tmp.clear();
            vf.push_back(a);
        }
        else{
            tmp.push_back(s[i]);
        }
    }
    vf.push_back(stof(tmp));
    return vf;
}

int main(){
    iostream::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    // filesystem::current_path("C:/neuralnetithink/trainingTest");
    freopen("mnist_testALLSCALEDDOWN.csv", "r", stdin);
    for(int i = 1; i <= 1000; i++){
        string temp = fileGen(i);
        const char * name = temp.c_str();
        ofstream file;
        file.open(name);
        vector<float> vf;
        for(int j = 0; j < 10; j++){
            string s;
            getline(cin, s);
            vf.clear();
            vf = stringSplit(s);
            file << vf[0] << "\n";
            for(int k = 1; k < vf.size(); k++){
                file << vf[k] << " ";
            }
            file << "\n";
        }
        file.close();
    }
    return 0;
}