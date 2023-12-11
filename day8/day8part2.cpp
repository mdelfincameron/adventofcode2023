#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>

using namespace std;

//Extracts node strings from line
vector<string> getNode(string str){

    vector<string> res;
    smatch m;
    regex text("\\w+");
    
    while(regex_search(str, m, text)){
        res.push_back(m.str());
        str.erase(m.position(),m.length());
    }

    return res;
}

//Recursive function to get greatest common denominator
long long gcd(long long a, long long b){
    if(b == 0){
        return a;
    }
    return gcd(b, a % b);
}

//Get least common multiple of 2 integers
long long lcm(long long a, long long b){
    return (a / gcd(a, b)) * b;
}

//Get least common multiple of every integer in final vector
long long getLCM(vector<int> v){
    long long final = v[0];
    for(int i = 0; i < v.size(); i++){
        final = lcm(final, v[i]);
    }
    return final;
}

int main(){
    string filename;
    fstream file;
    long long count = 0;
    long long total = 0;
    string instructions;
    map<string, pair<string,string>> nodes;
    vector<string> curNodes;
    bool allZs = false;
    int i = 0;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Get line of instructions to be repeated
    getline(file,line);
    instructions = line;

    //Skip empty line
    getline(file,line);

    //Read in lines, insert node into map
    while(getline(file,line)){

        vector<string> node = getNode(line);
        nodes.insert({node[0], {node[1], node[2]}});

        //Add node to list of starting nodes
        if(node[0][2] == 'A'){
            curNodes.push_back(node[0]);
        }
        
    }
    
    file.close();

    vector<int> zCounts(curNodes.size(), 0);

    while(!allZs){

        //Get current instruction
        char instruction = instructions[i];
        vector<pair<string,string>> dests;
        vector<string> nexts;

        //Get next destination for each node, add to list of next nodes
        for(auto cur : curNodes){
            //cout << cur << " ";
            dests.push_back(nodes.find(cur)->second);
        }
        //cout << endl;
        
        for(auto dest : dests){
            if(instruction == 'L'){
                nexts.push_back(dest.first);
            }
            else{
                nexts.push_back(dest.second);
            }
        }

        //Check for any new endpoints, adjust list of endpoint counts accordingly
        for(int i = 0; i < curNodes.size(); i++){
            if(curNodes[i][2] == 'Z' && zCounts[i] == 0){
                zCounts[i] = count;
            }
        }

        //Check list of endpoint counts; if all endpoints are found, get LCM of them and exit loop
        for(int i = 0; i < zCounts.size(); i++){
            if(zCounts[i] == 0){
                break;
            }
            else if(i == zCounts.size() - 1){
                total = getLCM(zCounts);
                allZs = true;
            }
        }

        //Check if instructions string needs to be reset
        if(i == instructions.length() - 1){
            i = 0;
        }
        else{
            i++;
        }

        count++;
        curNodes = nexts;
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}