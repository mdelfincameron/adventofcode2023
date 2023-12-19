#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <map>
#include <set>

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

enum MirrorDirs{
    facingWest,
    facingEast
};

struct Mirror{
    MirrorDirs facing;
    Direction reflect(Direction dir);
    vector<bool> visited;
    Mirror(MirrorDirs facing){
        this->facing = facing;
        visited.resize(4);
    }
};

//Give outgoing direction based on incoming direction, mark directions already visited from
Direction Mirror::reflect(Direction dir){
    visited[dir] = true;
    if(facing == facingWest){
        switch(dir){
            case north:
                return east;
            case east:
                return north;
            case south:
                return west;
            case west:
                return south;
        }
    }
    else{
        switch(dir){
            case north:
                return west;
            case east:
                return south;
            case south:
                return east;
            case west:
                return north;
        }
    }
    return north;
}

enum SplitterDirs{
    eastWest,
    northSouth
};

struct Splitter{
    SplitterDirs facing;
    vector<Direction> split(Direction dir);
    vector<bool> visited;
    Splitter(SplitterDirs facing){
        this->facing = facing;
        visited.resize(4);
    }
};

//Give split directions if needed, mark incoming direction and direction across from it
vector<Direction> Splitter::split(Direction dir){
    vector<Direction> res;
    visited[dir] = true;
    visited[(dir + 2) % 4] = true;
    if(facing == northSouth){
        if(dir == north || dir == south){
            res.push_back(dir);
            return res;
        }
        else{
            res.push_back(north);
            res.push_back(south);
            return res;
        }
    }
    else{
        if(dir == east || dir == west){
            res.push_back(dir);
            return res;
        }
        else{
            res.push_back(east);
            res.push_back(west);
            return res;
        }
    }
}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    vector<string> layout;
    vector<pair<Position,Direction>> paths = {{Position(0,0), east}};
    map<Position,Mirror> mirrors;
    map<Position,Splitter> splitters;
    set<Position> visited;

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

    //Create mirror/splitter objects at the appropriate positions, put in map for easier lookup
    for(int y = 0; y < layout.size(); y++){
        for(int x = 0; x < layout[0].length(); x++){
            if(layout[y][x] == '/'){
                Mirror m(facingWest);
                Position p(x,y);
                mirrors.insert({p, m});
            }
            else if(layout[y][x] == '\\'){
                Mirror m(facingEast);
                Position p(x,y);
                mirrors.insert({p, m});
            }
            else if(layout[y][x] == '-'){
                Splitter s(eastWest);
                Position p(x,y);
                splitters.insert({p,s});
            }
            else if(layout[y][x] == '|'){
                Splitter s(northSouth);
                Position p(x,y);
                splitters.insert({p,s});
            }
        }
    }

    //Trace paths, reflecting from mirrors and splitting from splitters as needed
    while(!paths.empty()){
        //cout << paths.size() << endl;
        auto pos = paths.front().first;
        auto dir = paths.front().second;
        //Insert position into set
        visited.insert(pos);
        char pathsPos = layout[pos.y][pos.x];
        //cout << pos.y << "," << pos.x << " moving " << dir << endl;
        if(pathsPos == '/' || pathsPos == '\\'){
            //Check if mirror exists, if visited from current direction end path, else reflect and continue
            if(auto it = mirrors.find(pos); it != mirrors.end()){
                if(it->second.visited[dir]){
                    paths.erase(paths.begin());
                    continue;
                }
                else{
                    //cout << "Mirroring " << it->second.facing << " from " << dir << endl;
                    dir = it->second.reflect(dir);
                }
            }
        }
        else if(pathsPos == '-' || pathsPos == '|'){
            //Check if splitter exists, if visited from current direction end path, else split and continue
            if(auto it = splitters.find(pos); it != splitters.end()){
                if(it->second.visited[dir]){
                    paths.erase(paths.begin());
                    continue;
                }
                else{
                    //cout << "Splitting " << it->second.facing << endl;
                    vector<Direction> splits = it->second.split(dir);
                    dir = splits[0];
                    if(splits.size() > 1){
                        auto splitPos = pos;
                        auto splitDir = splits[1];
                        if(splitPos.move(splitDir, layout)){
                            paths.push_back({splitPos, splitDir});
                        }
                    }
                }
            }
        }

        //Move in current direction(may be altered by mirror/splitter)
        if(!pos.move(dir, layout)){
            paths.erase(paths.begin());
            continue;
        }
        paths.front().first = pos;
        paths.front().second = dir;
    }

    total = visited.size();

    /*
    for(auto i : visited){
        layout[i.y][i.x] = '#';
    }

    for(auto i : layout){
        cout << i << endl;
    }
    */

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}