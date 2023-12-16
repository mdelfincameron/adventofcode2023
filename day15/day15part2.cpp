#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
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
    regex operations("[=-]");

    vector<vector<pair<string, int>>> boxes(256);

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

    //Iterate through strings and add to/remove from box
    for(auto str : strs){

        //Find labeland operation
        regex_search(str, m, operations);
        string label = str.substr(0, m.position());
        string operation = m.str();

        //Get box number by hashing label
        int boxNum = getHash(label);
        //cout << label << endl;

        //Check if adding or removing label
        if(operation == "="){
            //Get focal length, if label already exists change focal length to new one, add to box if new label
            int focalLength = stoi(str.substr(m.position() + m.length()));
            bool labelFound = false;
            for(auto jt = boxes[boxNum].begin(); jt != boxes[boxNum].end(); jt++){
                if(jt->first == label){
                    jt->second = focalLength;
                    labelFound = true;
                    break;
                }
            }
            if(!labelFound){
                boxes[boxNum].push_back({label, focalLength}); 
            }          
        }
        else{
            //Remove label from appropriate box if it exists
            for(auto jt = boxes[boxNum].begin(); jt != boxes[boxNum].end(); jt++){
                if(jt->first == label){
                    boxes[boxNum].erase(jt);
                    break;
                }
            }
        }

        //cout << str << endl;
            
    }

    for(int i = 0; i < boxes.size(); i++){
        for(int j = 0; j < boxes[i].size(); j++){
            //cout << boxes[i][j].first << endl;
            //cout << i + 1 << ":" << j + 1 << ":" << boxes[i][j].second << endl;
            //Add box number * slot * focal length to total 
            total += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}