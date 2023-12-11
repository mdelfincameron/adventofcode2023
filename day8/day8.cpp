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

int main(){
    string filename;
    fstream file;
    int count = 0;
    string instructions;
    map<string, pair<string,string>> nodes;
    string cur = "AAA";
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
        
    }
    
    file.close();

    //Find final node
    while(cur != "ZZZ"){

        //Get current instruction
        char instruction = instructions[i];
        string next;

        //Get next destination
        pair<string,string> dests = (nodes.find(cur))->second;
        if(instruction == 'L'){
            next = dests.first;
        }
        else{
            next = dests.second;
        }

        //Check if instructions string needs to be reset
        if(i == instructions.length() - 1){
            i = 0;
        }
        else{
            i++;
        }

        count++;
        cur = next;
    }

    auto end = chrono::steady_clock::now();

    cout << count << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}