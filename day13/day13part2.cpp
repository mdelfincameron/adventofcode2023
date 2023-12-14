#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>

using namespace std;

int checkDiffs(string a, string b){
    int diffCount = 0;
    for(int i = 0; i < a.length(); i++){
        if(a[i] != b[i]){
            diffCount++;
        }
    }
    return diffCount;
}

int main(){
    string filename;
    fstream file;

    long long sum = 0;
    vector<vector<string>> terrains;
    vector<pair<char,int>> reflections;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in lines, add to terrains if end of map is reached
    vector<string> terrain;
    while(getline(file,line)){

        if(line == ""){
            terrains.push_back(terrain);
            terrain.clear();
        }
        else{
            terrain.push_back(line);
        }

    }

    //No newline after last terrain, push it into terrains
    terrains.push_back(terrain);

    file.close();

    /*
    for(auto t : terrains){
        for(auto u : t){
            cout << u << endl;
        }
        cout << endl;
    }
    */

    //Locate reflections
    for(auto terrain : terrains){
        bool rowFound = false;
        bool colFound = false;

        //Find reflection's row if it exists, there can be at most 1 difference in compared rows(the smudge)
        for(int row = 0; row < terrain.size() - 1; row++){
            //cout << "Rows:" << endl << terrain[row] << endl << terrain[row + 1] << endl << endl;

            //If possible row reflection start is found, check each side
            if(checkDiffs(terrain[row], terrain[row + 1]) <= 1){
                int diffCount = 0;
                int upper = row;
                int lower = row + 1;
                while(upper >= 0 && lower < terrain.size()){
                    diffCount += checkDiffs(terrain[upper], terrain[lower]);
                    //cout << terrain[upper] << endl << terrain[lower] << endl << endl;
                    if(diffCount > 1){
                        break;
                    }
                    else if((upper == 0 || lower == terrain.size() - 1) && diffCount == 1){
                        rowFound = true;
                        reflections.push_back({'r',row + 1});
                    }
                    upper--;
                    lower++;
                }
            }
            if(rowFound){
                break;
            }
        }

        //No row reflection found, must be column reflection, do the same difference check
        if(!rowFound){
            for(int col = 0; col < terrain[0].length() - 1; col++){
                string leftCol = "";
                string rightCol = "";
                for(auto str : terrain){
                    leftCol += str[col];
                    rightCol += str[col + 1];
                }
                //cout << "Cols:" << endl << leftCol << endl << rightCol << endl << endl;

                //If possible column reflection start is found, check each side
                if(checkDiffs(leftCol, rightCol) <= 1){
                    int diffCount = 0;
                    int left = col;
                    int right = col + 1;
                    while(left >= 0 && right < terrain[0].size()){
                        leftCol = "";
                        rightCol = "";
                        for(auto str : terrain){
                            leftCol += str[left];
                            rightCol += str[right];
                        }
                        diffCount += checkDiffs(leftCol, rightCol);
                        //cout << leftCol << endl << rightCol << endl << endl;
                        if(diffCount > 1){
                            break;
                        }
                        else if((left == 0 || right == terrain[0].size() - 1) && diffCount == 1){
                            colFound = true;
                            reflections.push_back({'c',col + 1});
                            break;
                        }
                        left--;
                        right++;
                    }
                }
                if(colFound){
                    break;
                }
            }
        }
    }

    for(auto r : reflections){
        //cout << c << endl;
        if(r.first == 'c'){
            sum += r.second;
        }
        else{
            sum += 100 * r.second;
        }
    }
    //cout << endl;

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}