#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <chrono>
#include <algorithm>

using namespace std;

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

    getline(file,line);
    instructions = line;
    getline(file,line);

    while(getline(file,line)){

        vector<string> node = getNode(line);
        nodes.insert({node[0], {node[1], node[2]}});
        if(cur == ""){
            cur = node[0];
        }
        
    }
    
    file.close();

    while(cur != "ZZZ"){
        char instruction = instructions[i];
        pair<string,string> dests = (nodes.find(cur))->second;
        string next;

        if(instruction == 'L'){
            next = dests.first;
        }
        else{
            next = dests.second;
        }

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