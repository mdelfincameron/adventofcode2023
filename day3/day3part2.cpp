#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <regex>
#include <map>

using namespace std;

int main(){
    string filename;
    fstream file;
    long sum = 0;
    vector<string> schematic;
    regex num("\\d+");
    regex gear("\\*");
    map<pair<int,int>,vector<int>> gears;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file and add all chars to schematic 2D vector with padding on each side

        string line;

        getline(file,line);
        int length = line.length() + 2;
        file.seekg(0);

        schematic.push_back(string(length,'.'));

        while(getline(file,line)){
            
            schematic.push_back("." + line + ".");
        }

        schematic.push_back(string(length,'.'));
    }

    file.close();

    for(int i = 1; i < schematic.size() - 1; i++){

        auto begin = sregex_iterator(schematic[i].begin(),schematic[i].end(),num);
        auto end = sregex_iterator();
        smatch m;

        //Check adjacent chars to each regex found in line
        for(sregex_iterator j = begin; j != end; j++){
            int start = j->position();
            int length = j->length();

            vector<string> adjacentChars = {schematic[i - 1].substr(start - 1, length + 2), 
                                            schematic[i + 1].substr(start - 1, length + 2),
                                            schematic[i].substr(start - 1, 1),
                                            schematic[i].substr(start + length, 1)};

            //If there is a gear, locate gear and add {gear position, number} to map                                
            for (int k = 0; k < adjacentChars.size(); k++){
                if(regex_search(adjacentChars[k], m, gear)){

                    pair<int,int> pos;
                    int x = stoi(j->str());

                    switch(k){
                        case 0:
                            pos = {i - 1, start - 1 + m.position()};
                            break;
                        case 1: 
                            pos = {i + 1, start - 1 + m.position()};
                            break;
                        case 2:
                            pos = {i, start - 1};
                            break;
                        case 3:
                            pos = {i, start + length};
                            break;
                    }

                    if(auto search = gears.find(pos); search != gears.end()){
                        search->second.push_back(x);
                    }
                    else{
                        vector<int> v = {x};
                        gears.insert({pos,v});
                    }

                    break;
                }
            }
        } 
    }

    //Check if 2 adjacent numbers to gear, add product if so
    for(auto it = gears.begin(); it != gears.end(); it++){
        if(it->second.size() == 2){
            sum += it->second[0] * it->second[1];
        }
    }
    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}