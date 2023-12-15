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

    vector<vector<pair<string, int>>> boxes;
    boxes.resize(256);

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
        
        vector<string> strs;
        smatch m;

        //Split string, comma delimiters, add to vector
        while(regex_search(line, m, chars)){
            strs.push_back(m.str());
            line = line.substr(m.position() + m.length());
        }

        //Get hash of each string, add to total
        for(auto str : strs){
            string label = str.substr(0, 2);
            int boxNum = getHash(label);
            //cout << label << endl;
            if(str[2] == '='){
                int focalLength = str[3] - '0';
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
                for(auto jt = boxes[boxNum].begin(); jt != boxes[boxNum].end(); jt++){
                    if(jt->first == label){
                        boxes[boxNum].erase(jt);
                        break;
                    }
                }
            }

            //cout << str << endl;
            
        }
    }

    file.close();

    for(int i = 0; i < boxes.size(); i++){
        for(int j = 0; j < boxes[i].size(); j++){
            //cout << boxes[i][j].first << endl;
            //cout << i + 1 << ":" << j + 1 << ":" << boxes[i][j].second << endl; 
            total += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}