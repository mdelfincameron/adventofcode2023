#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>

using namespace std;

vector<int> extractDigits(string str){
    vector<int> res;
    regex digits("\\d+");
    smatch m;
    while(regex_search(str,m,digits)){
        res.push_back(stoi(m.str()));
        str = str.substr(m.position() + m.length());
    }
    return res;
}

int resolve(string str){

}

int main(){
    string filename;
    fstream file;

    long long total = 0;
    regex springs("[.#?]+");
    smatch m;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;

    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in lines
    while(getline(file,line)){
        
        regex_search(line, m, springs);
        string row = m.str();
        line = line.substr(m.position() + m.length());
        vector<int> springData = extractDigits(line);
        
    }

    file.close();

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}