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

int main(){
    string filename;
    fstream file;
    long sum = 0;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        while(getline(file,line)){

            int id = 0;
            int matchCount = 0;

            //Split along colon
            vector<string> splitLine = split(line,":");

            //Get number of card
            smatch m;
            regex digits = regex("\\d+");
            regex_search(splitLine[0],m,digits);
            id = stoi(m.str());

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
            cout << id << " " << matchCount << endl;

            sum += pow(2, matchCount - 1);
        }
    }

    file.close();

    auto end = chrono::steady_clock::now();

    cout << sum << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}