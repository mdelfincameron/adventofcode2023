#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

bool checkForSymbol(vector<vector<char>> arr, int i, int j){
    if(i >= 0 && i < arr.size() && j >= 0 && j < arr[i].size()){
        return !isdigit(arr[i][j]) && (arr[i][j] != '.');
    }
    return false;
}

bool checkAdjacent(vector<vector<char>> arr, int i, int j){
    return checkForSymbol(arr, i - 1, j) || checkForSymbol(arr, i - 1, j + 1)
        || checkForSymbol(arr, i, j + 1) || checkForSymbol(arr, i + 1, j + 1)
        || checkForSymbol(arr, i + 1, j) || checkForSymbol(arr, i + 1, j - 1)
        || checkForSymbol(arr, i, j - 1) || checkForSymbol(arr, i - 1, j - 1);
}

int main(){
    auto start = chrono::steady_clock::now();
    string filename;
    fstream file;
    long sum = 0;
    vector<vector<char>> schematic;

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

    bool symbolFound = false;
    int num = 0;
    for(int i = 0; i < schematic.size(); i++){
        for(int j = 0; j < schematic[i].size(); j++){

            //Check for symbol in all adjacent spots if digit, starting from directly above and going clockwise
            if(isdigit(schematic[i][j])){
                if(!symbolFound && checkAdjacent(schematic,i,j)){
                    symbolFound = true;
                }
                num = (num * 10) + (schematic[i][j] - '0');
            }

            //Check if last digit of number
            if(true){
                if(symbolFound){
                    sum += num;
                    symbolFound = false;
                    //cout << num << endl;
                }
                num = 0;
            }

            //cout << schematic[i][j] << ":" << checkForSymbol(schematic,i,j) << " ";
        }
        //cout << endl;
    }
    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}