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
    long sum = 0;
    int MAX_RED = 12;
    int MAX_GREEN = 13;
    int MAX_BLUE = 14;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        while(getline(file,line)){

            int matchCount = 0;
            bool possible = true;

            //Split along colon
            vector<string> splitLine = split(line,":");

            //Split along pipe symbol
            vector<string> splitNums = split(splitLine[1],"|");

            //Extract numbers, put them in a vector
            vector<int> winningNums = convertToInt(split(splitNums[0]," "));
            vector<int> haveNums = convertToInt(split(splitNums[1]," "));

            /*
            cout << "Winning Numbers: ";
            for(auto i : winningNums){
                cout << i << " ";
            }
            cout << endl;
            cout << "Have Numbers: ";
            for(auto i : haveNums){
                cout << i << " ";
            }
            cout << endl;
            */

           //Find how many matches there are in winning number vector, add appropriate amount to sum
            for(auto i : haveNums){
                if(find(winningNums.begin(), winningNums.end(), i) != winningNums.end()){
                    matchCount++;
                }
            }

            sum += pow(2, matchCount - 1);
        }
    }

    file.close();

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}