#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <regex>

using namespace std;

int main(){
    string filename;
    fstream file;
    long sum = 0;
    vector<string> schematic;
    regex num("\\d+");

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
        regex symbol("[^0-9.]+");

        //Check adjacent chars to each regex found in line
        for(sregex_iterator j = begin; j != end; j++){
            int start = j->position();
            int length = j->length();

            vector<string> adjacentChars = {schematic[i - 1].substr(start - 1, length + 2), 
                                            schematic[i + 1].substr(start - 1, length + 2),
                                            schematic[i].substr(start - 1, 1),
                                            schematic[i].substr(start + length, 1)};

            //If there is an adjacent symbol, add to sum
            for (int k = 0; k < adjacentChars.size(); k++){
                if(regex_search(adjacentChars[k], m, symbol)){
                    sum += stoi(j->str());
                    break;
                }
            }
        } 
    }
    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}