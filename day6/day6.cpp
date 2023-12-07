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

vector<int> convertToInt(vector<string> v){
    vector<int> res;
    for(auto i : v){
        if(i != ""){
            res.push_back(stoi(i));
        }
    }
    return res;
}

int main(){
    string filename;
    fstream file;
    long long total = 1;
    vector<int> winCounts;
    vector<int> times;
    vector<int> distances;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    //Read lines in
    string line;

    getline(file,line);
    vector<string> splitLine = split(line,":");
    times = convertToInt(split(splitLine[1]," "));

    getline(file,line);
    splitLine = split(line,":");
    distances = convertToInt(split(splitLine[1]," "));

    for(int i = 0; i < times.size(); i++){
        int minTime, maxTime;
        int timeAllowed = times[i];
        int record = distances[i];
        for(int j = 1; j < timeAllowed; j++){
            if(j * (timeAllowed - j) > record){
                minTime = j;
                break;
            }
        }

        for(int j = timeAllowed - 1; j > 0; j--){
            if(j * (timeAllowed - j) > record){
                maxTime = j;
                break;
            }
        }

        winCounts.push_back(maxTime - minTime + 1);

    }
    file.close();

    for(auto win : winCounts){
        total *= win;
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}