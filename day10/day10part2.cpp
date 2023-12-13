#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <queue>
#include <set>

using namespace std;

struct pos{
    int x, y;
};

bool operator==(pos a, pos b){ 
    return a.x==b.x && a.y==b.y; 
}
bool operator!=(pos a, pos b){ 
    return !(a == b); 
}
pos operator+(pos a, pos b){ 
    return { a.x+b.x, a.y+b.y }; 
}
pos& operator+=(pos& a, pos b){ 
    return a = a + b; 
}

struct pipeMap{
    char at(pos p) const { 
        return grid[p.y*width+p.x]; 
    }
    char& at(pos p) { 
        return grid[p.y*width+p.x]; 
    }

    std::vector<char> grid;
    int width;
    int height;
};

bool pointInLoop(pair<int,int> pos, vector<pair<int,int>> loopTiles){
    cout << endl;
    cout << pos.first << "," << pos.second << endl;
    int num = loopTiles.size();
    int j = num - 1;
    bool c = false;

    for(int i = 0; i < num; i++){
        if(pos.second == loopTiles[i].second && pos.first == loopTiles[i].first){
            cout << "Corner" << endl;
            return true;
        }
        if((loopTiles[i].first > pos.first) != (loopTiles[j].first > pos.first)){

            int slope = (pos.second - loopTiles[i].second) * (loopTiles[j].first - loopTiles[i].first)
            - (loopTiles[j].second - loopTiles[i].second) * (pos.first - loopTiles[i].first);
            cout << "Slope: " << slope << endl;

            if(slope == 0){
                cout << "Boundary" << endl;
                return true;
            }

            if((slope < 0) != (loopTiles[j].first < loopTiles[i].first)){
                cout << "Crossed" << endl;
                c = !c;
            }
        }
        j = i;
    }

    return c;
}

int main(){
    string filename;
    fstream file;

    long long dist = 0;
    vector<string> tiles;
    queue<vector<int>> check;
    long long tileCount = 0;
    vector<pair<int,int>> loopTiles;

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

        //Check if tile is already traversed
        if(tiles[i][j] == '!'){
            check.pop();
            continue;
        }

        loopTiles.push_back({i, j});

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
        
        //Change current tile to nonspecified char so it can't be traversed again
        tiles[i][j] = '!';

        //Last tile has been found, save final count
        if(check.front() == check.back()){
            dist = count;
        }
        
        check.pop();
    }

    sort(loopTiles.begin(), loopTiles.end());

    for(auto i : tiles){
        for(auto j : i){
            cout << j;
        }
        cout << endl;
    }
    cout << endl;

    /*
    for(auto p : loopTiles){
        cout << p.first << ", " << p.second << endl;
    }
    cout << loopTiles.size() << endl;
    cout << endl;
    */

    for(int y = 1; y < tiles.size() - 1; y++){
        for(int x = 1; x < tiles[0].size() - 1; x++){
            if(tiles[y][x] != '!' && pointInLoop({y, x}, loopTiles)){
                cout << tiles[y].substr(0, x) << endl;
                cout << y << ", " << x << endl; 
                tileCount++;
            }
        }
    }

    auto end = chrono::steady_clock::now();

    cout << tileCount << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}