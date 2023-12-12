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
    vector<int> emptyRows;
    vector<int> emptyCols;

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
        space.push_back(line);
    }

    file.close();

    //Find rows without galaxies, push to emptyRows
    for(int i = 0; i < space.size(); i++){
        if(space[i].find('#') == string::npos){
            emptyRows.push_back(i);
            //cout << i << " ";
        }
    }
    //cout << endl;

    //Find columns without galaxies, push to emptyCols
    for(int j = 0; j < space[0].size(); j++){
        bool noGalaxies = true;

        for(int i = 0; i < space.size(); i++){
            if(space[i][j] == '#'){
                noGalaxies = false;
                break;
            }
        }

        if(noGalaxies){
            //cout << j << " ";
            emptyCols.push_back(j);
        }
    }
    //cout << endl;

    /*
    for(auto i : space){
        for(auto j : i){
            cout << j;
        }
        cout << endl;
    }
    cout << endl;
    */

   ///Find position of every galaxy
   for(int i = 0; i < space.size(); i++){
        for(int j = 0; j < space[0].size(); j++){
            if(space[i][j] == '#'){
                galaxies.push_back({i, j});
            }
        }
    }

    //Get final sum
    for(int i = 0; i < galaxies.size(); i++){
        for(int j = i + 1; j < galaxies.size(); j++){
            //Find distance between galaxies
            long long dist = abs(galaxies[j].first - galaxies[i].first) + abs(galaxies[j].second - galaxies[i].second);

            int minRow = min(galaxies[j].first, galaxies[i].first);
            int minCol = min(galaxies[j].second, galaxies[i].second);
            int maxRow = max(galaxies[j].first, galaxies[i].first);
            int maxCol = max(galaxies[j].second, galaxies[i].second);
            //cout << minRow << "->" << maxRow << ", " << minCol << "->" << maxCol << endl;

            //Check empty rows and cols to see if there is one between specified galaxies, add to distance if so
            for(auto r : emptyRows){
                if(minRow <= r && r <= maxRow){
                    //cout << "Row added" << endl;
                    dist += 999999;
                }
            }
            for(auto c : emptyCols){
                if(minCol <= c && c <= maxCol){
                    //cout << "Column added" << endl;
                    dist += 999999;
                }
            }
            //cout << dist << endl;
            sum += dist;
        }
    }

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}