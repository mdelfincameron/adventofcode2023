#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

using namespace std;

bool checkForGear(vector<vector<char>> arr, int i, int j, pair<int,int> &gear){
    if(i >= 0 && i < arr.size() && j >= 0 && j < arr[i].size() && arr[i][j] == '*'){
        gear = {i, j};
        return true;
    }
    return false;
}

bool checkAdjacent(vector<vector<char>> arr, int i, int j, pair<int,int> &gear){
    return checkForGear(arr, i - 1, j, gear) || checkForGear(arr, i - 1, j + 1, gear)
        || checkForGear(arr, i, j + 1, gear) || checkForGear(arr, i + 1, j + 1, gear)
        || checkForGear(arr, i + 1, j, gear) || checkForGear(arr, i + 1, j - 1, gear)
        || checkForGear(arr, i, j - 1, gear) || checkForGear(arr, i - 1, j - 1, gear);
}

int main(){
    auto start = chrono::steady_clock::now();
    string filename;
    fstream file;
    long sum = 0;
    vector<vector<char>> schematic;
    map<pair<int,int>,vector<int>> gears;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file and add all chars to schematic 2D vector
        string line;
        while(getline(file,line)){
            vector<char> ln;
            for(int i = 0; i < line.length(); i++){
                ln.push_back(line[i]);
            }
            schematic.push_back(ln);
        }
    }

    file.close();

    bool gearFound = false;
    int num = 0;
    pair<int,int> gear = {-1,-1};
    for(int i = 0; i < schematic.size(); i++){
        for(int j = 0; j < schematic[i].size(); j++){

            //Check for symbol in all adjacent spots if digit, starting from directly above and going clockwise
            if(isdigit(schematic[i][j])){
                if(!gearFound && checkAdjacent(schematic, i, j, gear)){
                    gearFound = true;
                }
                num = (num * 10) + (schematic[i][j] - '0');
            }

            //Check if last digit of number
            if(num > 0 && (j + 1 >= schematic[i].size() || !isdigit(schematic[i][j + 1]))){
                if(gearFound){
                    if(auto search = gears.find(gear); search != gears.end()){
                        search->second.push_back(num);
                    }
                    else{
                        vector<int> v = {num};
                        gears.insert({gear,v});
                    }
                    gear = {-1,-1};
                    gearFound = false;
                }
                num = 0;
            }

            //cout << schematic[i][j] << ":" << checkForGear(schematic,i,j) << " ";
        }
        //cout << endl;
    }

    
    for(auto it = gears.begin(); it != gears.end(); it++){
        cout << it->first.first << ", " << it->first.second << endl;
        for(auto jt = it->second.begin(); jt != it->second.end(); jt++){
            cout << *jt << " ";
        }
        cout << endl;
        if(it->second.size() == 2){
            cout << it->second[0] * it->second[1] << endl;
            sum += it->second[0] * it->second[1];
        }
    }
    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}