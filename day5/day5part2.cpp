#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <cmath>

using namespace std;

vector<string> split(string str, string delimiter){

    vector<string> res;
    size_t pos = 0;
    
    while((pos = str.find(delimiter)) != string::npos){
        res.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    
    res.push_back(str);
    return res;
}

vector<long long> convertToLongLong(vector<string> v){
    vector<long long> res;
    for(auto i : v){
        if(i != ""){
            res.push_back(stoll(i));
        }
    }
    return res;
}


int main(){
    string filename;
    fstream file;
    vector<int> matches;
    regex colon(":");
    vector<vector<vector<long long>>> vecs;
    vector<long long> splitSeeds;
    long long minLocation = LONG_LONG_MAX;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        getline(file,line);
        //Split along colon
        vector<string> seeds = split(line,":");
        splitSeeds = convertToLongLong(split(seeds[1], " "));
        smatch m;
        
        while(getline(file,line)){

            if(regex_search(line, m, colon)){
                vector<vector<long long>> m;
                while(getline(file,line)){
                    if(line == ""){
                        break;
                    }

                    vector<long long> v = convertToLongLong(split(line, " "));
                    m.push_back(v);

                }
                vecs.push_back(m);
            }
            
        }
        
    }

    file.close();
    
    for(auto location : splitSeeds){
        //cout << location << endl;
        for(auto i : vecs){
            //cout << location << " ";
            for(auto j : i){
                long long dest = j[0];
                long long source = j[1];
                long long range = j[2];
                if (location >= source && location < source + range){
                    location = dest + (location - source);
                    break;
                }
            }
            //cout << location << endl;
        }
        if(location < minLocation){
            minLocation = location;
        }
        //cout << location << endl;
    }

    auto end = chrono::steady_clock::now();

    cout << minLocation << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}