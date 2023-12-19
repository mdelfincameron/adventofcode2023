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
    bool move(Direction dir){
        switch(dir){
            case north:
                y--;
                break;
            case east:
                x++;
                break;
            case south:
                y++;
                break;
            case west:
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

Direction charToDir(char c){
    switch(c){
        case 'U':
            return north;
        case 'R':
            return east;
        case 'D':
            return south;
        case 'L':
            return west;
    }
    return north;
}

bool pointInPath(int x, int y, vector<Position>& loop, int maxX){
    auto num = loop.size();
    int j = num - 1;
    bool c = false;
    for(int i = 0; i < num; i++){
        if(x == loop[i].x && y == loop[i].y){
            return true;
        }
        if(loop[i].y > y != loop[j].y > y){
            int slope = (x - loop[i].x) * (loop[j].y - loop[i].y) - (loop[j].x - loop[i].x) * (y - loop[i].y);
            if(slope == 0){
                return true;
            }
            if((slope < 0) != (loop[j].y < loop[i].y)){
                c = !c;
            }
        }
        j = i;
    }
    return c;
}

double polygonArea(vector<Position>& verts){
    double area = 0.0;
    int j = verts.size() - 1;
    for(int i = 0; i < verts.size(); i++){
        area += (verts[j].x + verts[i].x) * (verts[j].y - verts[i].y);
        cout << area << endl;
        j = i;
    }
    return abs(area/2.0);
}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    Position cur = Position(0,0);
    vector<pair<Direction, int>> instructions;
    set<Position> dig;
    vector<Position> verts;
    smatch m1;
    smatch m2;
    regex dirs("[URDL]");
    regex nums("\\d+");

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
        
        regex_search(line, m1, dirs);
        regex_search(line, m2, nums);
        instructions.push_back(make_pair(charToDir(m1.str()[0]),stoi(m2.str())));
        
    }

    file.close();

    while(!instructions.empty()){
        dig.insert(cur);
        cur.move(instructions.front().first);
        instructions.front().second--;
        if(instructions.front().second == 0){
            verts.push_back(cur);
            instructions.erase(instructions.begin());
        }
    }

    sort(verts.begin(), verts.end());

    /*
    for(auto i : verts){
         cout << i.y << ":" << i.x << endl;
    }

    int maxY = 0;
    int maxX = 0;
    for(auto d : dig){
        maxY = max(maxY, d.y);
        maxX = max(maxX, d.x);
    }

    for(int i = 0; i <= maxY; i++){
        for(int j = 0; j <= maxX; j++){
            if(pointInPath(j,i,verts,maxX)){
                //cout << i << ":" << j << endl;
                total++;
            }
        }
    }
    */
    total = polygonArea(verts);

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}
