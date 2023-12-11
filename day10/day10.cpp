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

    long long dist = 0;
    vector<string> tiles;
    queue<vector<int>> check;
    int lineCount = 0;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in input and pad every side with periods
    getline(file,line);
    int length = line.length() + 2;
    file.seekg(0);

    //Add padding to top
    tiles.push_back(string(length,'.'));

    //Read in lines, add padding on either side
    while(getline(file,line)){

        tiles.push_back("." + line + ".");

    }

    //Add padding to bottom
    tiles.push_back(string(length,'.'));

    file.close();

    //Find starting point
    bool startFound = false;
    for(int i = 0; i < tiles.size(); i++){
        for(int j = 0; j < tiles[i].size(); j++){
            if(tiles[i][j] == 'S'){
                check.push({i, j, 0});
                startFound = true;
                break;
            }
            //cout << tiles[i][j];
        }

        if(startFound){
            break;
        }
        //cout << endl;
    }
    //cout << endl;

    while(!check.empty()){
        int i = check.front()[0];
        int j = check.front()[1];
        int count = check.front()[2];

        if(tiles[i][j] == '.'){
            check.pop();
            continue;
        }

        //Check if each adjacent tile is traversable
        if(tiles[i - 1][j] == '|' || tiles[i - 1][j] == '7' || tiles[i - 1][j] == 'F'){
            //cout << "North: " << tiles[i - 1][j] << endl;
            check.push({i - 1, j, count + 1 });
        }
        if(tiles[i][j + 1] == '-' || tiles[i][j + 1] == 'J' || tiles[i][j + 1] == '7'){
            //cout << "East: " << tiles[i][j + 1] << endl;
            check.push({i, j + 1, count + 1});
        }
        if(tiles[i + 1][j] == '|' || tiles[i + 1][j] == 'L' || tiles[i + 1][j] == 'J'){
            //cout << "South: " << tiles[i + 1][j] << endl;
            check.push({i + 1, j, count + 1});
        }
        if(tiles[i][j - 1] == '-' || tiles[i][j - 1] == 'L' || tiles[i][j - 1] == 'F'){
            //cout << "West: " << tiles[i][j - 1] << endl;
            check.push({i, j - 1, count + 1});
        }
        
        //Change current tile to period so it can't be traversed again
        tiles[i][j] = '.';

        //Last tile has been found, save final count
        if(check.front() == check.back()){
            dist = count;
        }
        lineCount++;
        check.pop();
    }

    auto end = chrono::steady_clock::now();

    cout << dist << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}