#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>

using namespace std;

//Perform required hash operation, return
int getHash(string str){
    int res = 0;
    for(auto x : str){
        res = ((res + int(x)) * 17) % 256;
        //cout << res << endl;
    }
    //cout << "Final: " << res << endl << endl;
    return res;
}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    regex chars("[^,]+");

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;

    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;
    getline(file, line);

    file.close();

    vector<string> strs;
    smatch m;

    //Split string, comma delimiters, add to vector
    while(regex_search(line, m, chars)){
        strs.push_back(m.str());
        line = line.substr(m.position() + m.length());
    }

    //Get hash of each string, add to total
    for(auto str : strs){
        //cout << str << endl;
        total += getHash(str);
    }

    file.close();

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}