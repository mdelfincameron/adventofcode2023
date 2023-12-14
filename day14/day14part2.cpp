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

string dishToString(vector<string> dish){
    string res = "";
    for(auto str : dish){
        res += str;
    }
    return res;
}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    vector<string> dish;
    const pair<int,int> north = {-1, 0};
    const pair<int,int> west = {0, -1};
    const pair<int,int> south = {1, 0};
    const pair<int,int> east = {0, 1};
    map<string, int> states;
    const int cycleLimit = 1000000000; 
    bool loopFound = 0;

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

    //Roll in a north->west->south->east cycle until a pattern is found
    for(int x = 0; x < cycleLimit; x++){
        vector<string> prev = dish;

        //Roll north
        for(int i = 0; i < dish.size(); i++){
            for(int j = 0; j < dish[0].size(); j++){
                if(dish[i][j] == 'O'){
                    roll(dish, {i, j}, north);
                }
            }
        }

        //Roll west
        for(int j = 0; j < dish[0].size(); j++){
            for(int i = 0; i < dish.size(); i++){
                if(dish[i][j] == 'O'){
                    roll(dish, {i, j}, west);
                }
            }
        }

        //Roll south
        for(int i = dish.size() - 1; i >= 0; i--){
            for(int j = 0; j < dish[0].size(); j++){
                if(dish[i][j] == 'O'){
                    roll(dish, {i, j}, south);
                }
            }
        }

        //Roll east
        for(int j = dish[0].size() - 1; j >= 0; j--){
            for(int i = 0; i < dish.size(); i++){
                if(dish[i][j] == 'O'){
                    roll(dish, {i, j}, east);
                }
            }
        }

        /*
        cout << x << endl;
        for(auto y : dish){
            cout << y << endl;
        }
        cout << endl;
        
        int load = 0;
        for(int y = 0; y < dish.size(); y++){
            load += count(dish[y].begin(), dish[y].end(), 'O') * (dish.size() - y);
        }   
        cout << x << " : " << load << endl;
        */

        //Convert current state of dish to string
        string str = dishToString(dish);
        
        if(!loopFound){
                //If state already exists in map, pattern is found, otherwise insert current state
                if(auto it = states.find(str); it != states.end()){
                int offset = it->second;
                int loopLength = x - offset;
                //cout << "LOOP FOUND    LENGTH " << loopLength << endl;
                //Jump to the correct number of cycles before the end
                x = cycleLimit - (cycleLimit - offset) % loopLength;
                loopFound = true;
            }
            else{
                states.insert({str, x});
            }
        }

    }

    //Multiply the rock count of each line by the appropriate weight, add to total weight
    for(int i = 0; i < dish.size(); i++){
        total += count(dish[i].begin(), dish[i].end(), 'O') * (dish.size() - i);
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}