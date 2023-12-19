#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;

enum Direction{
    north,
    east,
    south,
    west
};

struct Position{
    int x, y;
    Position(){
        x = 0;
        y = 0;
    }
    Position(int x, int y){
        this->x = x;
        this->y = y;
    }
    //Move 1 spot in given direction, checking layout to see if it's valid
    bool move(Direction dir, vector<string>& layout){
        switch(dir){
            case north:
                if(y == 0){
                    return false;
                }
                y--;
                break;
            case east:
                if(x == layout[0].size() - 1){
                    return false;
                }
                x++;
                break;
            case south:
                if(y == layout.size() - 1){
                    return false;
                }
                y++;
                break;
            case west:
                if(x == 0){
                    return false;
                }
                x--;
                break;
        }
        return true;
    }
};

bool operator<(Position const& lhs, Position const& rhs){
    if(lhs.y != rhs.y){
        return lhs.y < rhs.y;
    }
    else{
        return lhs.x < rhs.x;
    }
};

int main(){
    string filename;
    fstream file;

    long long total = 0;
    vector<string> layout;

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
        
        layout.push_back(line);
        
    }

    file.close();

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}
