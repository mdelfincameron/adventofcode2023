#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <algorithm>

using namespace std;

//Extracts node strings from line
vector<long long> getLongs(string str){

    vector<long long> res;
    smatch m;
    regex text("[-\\d]+");
    
    while(regex_search(str, m, text)){
        res.push_back(stoll(m.str()));
        str.erase(m.position(),m.length());
    }

    return res;
}

long long extrapolateHistory(vector<long long> v){
    vector<vector<long long>> diffs = {v};
    bool allZeros = false;

    while(!allZeros){
        vector<long long> check = diffs[diffs.size() - 1];
        vector<long long> diff;

        for(int i = 1; i < check.size(); i++){
            diff.push_back(check[i] - check[i - 1]);
        }

        for(int i = 0; i < diff.size(); i++){
            if(diff[i] != 0){
                break;
            }
            else if(i == diff.size() - 1){
                allZeros = true;
            }
        }
        diffs.push_back(diff);
    }

    /*
    for(auto d : diffs){
        for(auto j : d){
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
    */

    for(int i = diffs.size() - 2; i >= 0; i--){
        diffs[i].push_back(diffs[i][diffs[i].size() - 1] + diffs[i + 1][diffs[i + 1].size() - 1]);
    }

    /*
    for(auto d : diffs){
        for(auto j : d){
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
    */

    //cout << diffs[0][diffs[0].size() - 1] << endl << endl;

    return diffs[0][diffs[0].size() - 1];
}

int main(){
    string filename;
    fstream file;

    long long sum = 0;
    vector<vector<long long>> histories;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in lines, insert node into map
    while(getline(file,line)){

        vector<long long> history = getLongs(line);
        histories.push_back(history);

        /*
        for(auto h : history){
            cout << h << " ";
        }
        cout << endl;
        */

    }
    
    file.close();

    for(auto history : histories){
        sum += extrapolateHistory(history);
    }

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}