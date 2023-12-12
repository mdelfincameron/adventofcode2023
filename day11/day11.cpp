#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <queue>

using namespace std;

int main(){
    string filename;
    fstream file;

    long long sum = 0;
    vector<string> space;
    vector<pair<int,int>> galaxies;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in input, get length of each line
    getline(file,line);
    int length = line.length();
    file.seekg(0);

    //Read in lines
    while(getline(file,line)){

        if(line.find('#') == string::npos){
            space.push_back(line);
        }
        space.push_back(line);

    }

    file.close();

    for(int j = 0; j < space[0].size(); j++){
        bool noGalaxies = true;

        for(int i = 0; i < space.size(); i++){
            if(space[i][j] == '#'){
                noGalaxies = false;
                break;
            }
        }

        if(noGalaxies){
            //cout << j << endl;
            for(int i = 0; i < space.size(); i++){
                space[i].insert(space[i].begin() + j, '.');
            }
            j++;
        }
    }

    /*
    for(auto i : space){
        for(auto j : i){
            cout << j;
        }
        cout << endl;
    }
    cout << endl;
    */

   for(int i = 0; i < space.size(); i++){
        for(int j = 0; j < space[0].size(); j++){
            if(space[i][j] == '#'){
                galaxies.push_back({i, j});
            }
        }
    }

    for(int i = 0; i < galaxies.size(); i++){
        for(int j = i + 1; j < galaxies.size(); j++){
            sum += abs(galaxies[j].first - galaxies[i].first) + abs(galaxies[j].second - galaxies[i].second);
        }
    }

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}