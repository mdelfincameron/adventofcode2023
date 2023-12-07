#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <set>

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
    istringstream lineStream("");
    vector<vector<vector<long long>>> vecs;
    vector<long long> splitSeeds;
    set<pair<long long, long long>> cur, temp, next;
    long long minLocation = LONG_LONG_MAX;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    //Read first line
    string line;
    smatch m;

    getline(file,line);

    //Split along colon
    vector<string> seeds = split(line,":");
    splitSeeds = convertToLongLong(split(seeds[1], " "));
    for(int i = 0; i < splitSeeds.size(); i += 2){
        cur.insert({splitSeeds[i], splitSeeds[i + 1]});
    }
        
    //Read until map is reached, then parse
    while(getline(file,line)){

        //Get next set of ranges if there are any
        if(line.empty()){
            cur.insert(next.begin(),next.end());
            next.clear();
            getline(file, line);
            continue;
        }

        long long dest, start, length;
        lineStream.clear();
        lineStream.str(line);
        lineStream >> dest >> start >> length;

        for (auto p : cur){

            long long rangeStart = p.first;
            long long rangeLength = p.second;
            long long rangeEnd = p.first + p.second;
            
            //Check if ranges overlap
            long long lower = max(rangeStart, start);
            long long upper = min(rangeEnd, start + length);
            //cout << upper << ":" << lower << endl;
            if (upper > lower){

                // Assign overlapping range its appropriate destination range
                next.insert({dest - start + lower, upper - lower});

                // Check what parts of the range aren't overlapping and keep them
                if (rangeEnd > start + length) {
                    temp.insert({start + length, rangeEnd - start - length});
                }
                if (rangeStart < start){
                    temp.insert({rangeStart, start - rangeStart});
                }
            }
            //No overlap, keep entire range
            else{
                temp.insert({rangeStart, rangeLength});
            }
        }
        //Put kept parts of range back into ranges to be checked
        cur.swap(temp);
        temp.clear();
    }

    cur.insert(next.begin(), next.end());            
        

    file.close();

    auto end = chrono::steady_clock::now();
    minLocation = cur.begin()->first;

    cout << minLocation << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}