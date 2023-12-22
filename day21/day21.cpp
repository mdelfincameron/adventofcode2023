#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <map>
#include <set>

using namespace std;

//Position on map with number of steps remaining
struct Position{
    int x, y, count;
    Position(){
        x = 0;
        y = 0;
        count = 0;
    }
    Position(int x, int y, int count){
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
    vector<Position> moves;
    //cout << x <<  " " << y  << endl;
    if(y > 0 && layout[y - 1][x] != '#'){
        Position north(x,y - 1, nextCount);
        if(auto check = visited.insert(north); check.second){
            moves.push_back(north);
            steps.push_back(north);
        }
    }
    if(x < layout[0].size() - 1 && layout[y][x + 1] != '#'){
        Position east(x + 1, y, nextCount);
        if(auto check = visited.insert(east); check.second){
            moves.push_back(east);
            steps.push_back(east);
        }
    }
    if(y < layout.size() - 1 && layout[y + 1][x] != '#'){
        Position south(x, y + 1, nextCount);
        if(auto check = visited.insert(south); check.second){
            moves.push_back(south);
            steps.push_back(south);
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