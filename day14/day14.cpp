#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>

using namespace std;

void roll(vector<string>& dish, pair<int,int> pos, pair<int,int> dir){
    int i = pos.first;
    int j = pos.second;
    //cout << "Rolling " << i << ", " << j << endl;
    //Check if current rock position is safe
    while((i >= 0 && i < dish.size()) && (j >= 0 && j < dish[0].size())){
        int iNext = i + dir.first;
        int jNext = j + dir.second;
        //cout << "Checking " << iNext << ", " << jNext << endl;
        //Check if next spot for rock is possible to roll to, break if not
        if((iNext >= 0 && iNext < dish.size()) && (jNext >= 0 && jNext < dish[0].size()) && dish[iNext][jNext] == '.'){
            //cout << "Roll successful" << endl;
            dish[iNext][jNext] = 'O';
            dish[i][j] = '.';
        }
        else{
            break;
        }
        i = iNext;
        j = jNext;
    }
}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    vector<string> dish;
    pair<int, int> north = {-1 , 0};

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;

    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in lines
    while(getline(file,line)){
        dish.push_back(line);
    }

    file.close();

    /*
    for(auto i : dish){
        cout << i << endl;
    }
    cout << endl;
    */

   //Roll each rock north
    for(int i = 0; i < dish.size(); i++){
        for(int j = 0; j < dish[0].size(); j++){
            if(dish[i][j] == 'O'){
                roll(dish, {i, j}, north);
            }
        }
    }

    /*
    for(auto i : dish){
        cout << i << endl;
    }
    cout << endl;
    */

    //Multiply the rock count of each line by the appropriate weight, add to total weight
    for(int i = 0; i < dish.size(); i++){
        total += count(dish[i].begin(), dish[i].end(), 'O') * (dish.size() - i);
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}