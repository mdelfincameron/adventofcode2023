#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <map>
#include <set>

using namespace std;

//Position on map with number of steps remaining + which map relative to starting position is on
struct Position{
    int mapX, mapY;
    int x, y, count;
    long long count;
    Position(){
        mapX = 0;
        mapY = 0;
        x = 0;
        y = 0;
        count = 0;
    }
    Position(int x, int y, int count){
        mapX = 0;
        mapY = 0;
        this->x = x;
        this->y = y;
        this->count = count;
    }
    Position(int x, int y, int count, int mapX, int mapY){
        this->mapX = mapX;
        this->mapY = mapY;
        this->x = x;
        this->y = y;
        this->count = count;
    }
};

//From position, move in all directions if possible, if not visited previously add to vector of positions
//to check and add to valid ending spot set if possible
void moveAll(Position& init, vector<string>& layout, set<Position>& visited, set<Position>& valid, vector<Position>& steps){
    int nextCount = init.count - 1;
    int x = init.x;
    int y = init.y;
    int maxX = layout[0].size() - 1;
    int maxY = layout.size() - 1;
    vector<Position> moves;
    //cout << x <<  " " << y  << endl;
    if(y > 0 && layout[y - 1][x] != '#'){
        Position north(x,y - 1, nextCount);
        if(auto check = visited.insert(north); check.second){
            moves.push_back(north);
            steps.push_back(north);
        }
    }
    else if(y == 0 && layout[maxY][x] != '#'){
        Position north(x, maxY, nextCount, init.mapX, init.mapY - 1);
        if(auto check = visited.insert(north); check.second){
            moves.push_back(north);
            steps.push_back(north);
        }
    }
    if(x < maxX && layout[y][x + 1] != '#'){
        Position east(x + 1, y, nextCount);
        if(auto check = visited.insert(east); check.second){
            moves.push_back(east);
            steps.push_back(east);
        }
    }
    else if(x == maxX && layout[y][0] != '#'){
        Position east(0, y, nextCount, init.mapX + 1, init.mapY);
        if(auto check = visited.insert(east); check.second){
            moves.push_back(east);
            steps.push_back(east);
        }
    }
    if(y < maxY && layout[y + 1][x] != '#'){
        Position south(x, y + 1, nextCount);
        if(auto check = visited.insert(south); check.second){
            moves.push_back(south);
            steps.push_back(south);
        }
    }
    else if(y == maxY && layout[0][x] != '#'){
        Position north(x, maxY, nextCount, init.mapX, init.mapY - 1);
        if(auto check = visited.insert(north); check.second){
            moves.push_back(north);
            steps.push_back(north);
        }
    }
    if(x > 0 && layout[y][x - 1] != '#'){
        Position west(x - 1, y, nextCount);
        if(auto check = visited.insert(west); check.second){
            moves.push_back(west);
            steps.push_back(west);
        }
    }

    //Valid ending spot if count is even, insert into valid ending set
    if(nextCount % 2 == 0){
        for(auto move : moves){
            valid.insert(move);
        }
    }
}

bool operator<(Position const& lhs, Position const& rhs){
    if(lhs.mapY != rhs.mapY){
        return lhs.mapY < rhs.mapY;
    }
    else if(lhs.mapX != rhs.mapX){
        return lhs.mapX < rhs.mapX;
    }
    else if(lhs.y != rhs.y){
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
    vector<Position> steps;
    set<Position> visited;
    set<Position> valid;
    smatch m;
    regex startPos("S");
    const int MAX_STEPS = 64;

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

        //Find starting spot, have it be the first in positions to check
        if(regex_search(line,m,startPos)){
            steps.push_back(Position(m.position(), layout.size() - 1, MAX_STEPS));
        }
        
    }

    file.close();

    /*
    for(auto str : layout){
        cout << str << endl;
    }
    */

    //Check all positions reachable in number of steps
    while(!steps.empty()){
        auto cur = steps.begin();

        //Move to possible adjacent spots if there are remaining steps
        if(cur->count > 0){
            moveAll(*cur, layout, visited, valid, steps);
            //cout << steps.size() << endl;
        }

        steps.erase(steps.begin());
    }

    total = valid.size();

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}