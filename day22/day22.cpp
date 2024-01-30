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
    int x, y, z;
    Position(){
        x = 0;
        y = 0;
        z = 0;
    }
    Position(int x, int y, int z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Brick{
    vector<Position> positions;
    Brick(){
        positions.push_back(Position(0,0,0));
    }
    Brick(Position start, Position end){
        if(start.x != end.x){
            for(int i = start.x; i <= end.x; i++){
                positions.push_back(Position(i,start.y,start.z));
            }
        }
        else if(start.y != end.y){
            for(int i = start.y; i <= end.y; i++){
                positions.push_back(Position(start.x,i,start.z));
            }
        }
        else{
            for(int i = start.z; i <= end.z; i++){
                positions.push_back(Position(start.x,start.y,i));
            }
        }
    }
    bool fall(set<Position>& layout){
        for(auto position : positions){
            if(position.z != 0){

            }
        }
    }
};

bool operator<(Position const& lhs, Position const& rhs){
    if(lhs.z != rhs.z){
        return lhs.z < rhs.z;
    }
    else if(lhs.x != rhs.x){
        return lhs.x < rhs.x;
    }
    else{
        return lhs.y < rhs.y;
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