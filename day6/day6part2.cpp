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

long long convertToLongLong(vector<string> v){
    string res = "";
    for(auto i : v){
        if(i != ""){
            res += i;
        }
    }
    return stoll(res);
}

int main(){
    string filename;
    fstream file;
    long long total = 0;
    vector<long long> winCounts;
    vector<long long> times;
    vector<long long> distances;

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

    //Get list of times
    getline(file,line);
    vector<string> splitLine = split(line,":");
    times.push_back(convertToLongLong(split(splitLine[1]," ")));

    //Get list of distances
    getline(file,line);
    splitLine = split(line,":");
    distances.push_back(convertToLongLong(split(splitLine[1]," ")));

    //Check each time and distance for possible wins
    for(int i = 0; i < times.size(); i++){

        long long minTime, maxTime;
        long long timeAllowed = times[i];
        long long record = distances[i];

        //Find minimum possible time needed to win
        for(long long j = 1; j < timeAllowed; j++){
            if(j * (timeAllowed - j) > record){
                minTime = j;
                break;
            }
        }

        //Find maximum possible time to win
        for(long long j = timeAllowed - 1; j > 0; j--){
            if(j * (timeAllowed - j) > record){
                maxTime = j;
                break;
            }
        }

        //Add number of winning times to vector
        winCounts.push_back(maxTime - minTime + 1);

    }
    file.close();

    if(!winCounts.empty()){
        total++;
    }

    //Assign product of wins to total
    for(auto win : winCounts){
        total *= win;
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}