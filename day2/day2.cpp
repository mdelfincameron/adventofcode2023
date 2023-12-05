#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

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
    int MAX_RED = 12;
    int MAX_GREEN = 13;
    int MAX_BLUE = 14;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        while(getline(file,line)){

            int id = 0;
            bool possible = true;

            //Split along colon
            vector<string> splitLine = split(line,":");

            //Get ID of game
            smatch m;
            regex digits = regex("\\d+");
            regex_search(splitLine[0],m,digits);
            id = stoi(m.str());

            //Split subsets by semicolons
            vector<string> subsets = split(splitLine[1],";");

            //Count reds, greens, and blues from each subset and check if possible
            regex red = regex("red");
            regex green = regex("green");
            regex blue = regex("blue");
            for(int i = 0; i < subsets.size(); i++){
                vector<string> colors = split(subsets[i],",");
                for(int j = 0; j < colors.size(); j++){
                    if(regex_search(colors[j],m,red)){
                        regex_search(colors[j],m,digits);
                        if(stoi(m.str()) > MAX_RED){
                            possible = false;
                            break;
                        }
                    }
                    else if(regex_search(colors[j],m,green)){
                        regex_search(colors[j],m,digits);
                        if(stoi(m.str()) > MAX_GREEN){
                            possible = false;
                            break;
                        }
                    }
                    else if(regex_search(colors[j],m,blue)){
                        regex_search(colors[j],m,digits);
                        if(stoi(m.str()) > MAX_BLUE){
                            possible = false;
                            break;
                        }
                    }
                }
                //cout << subsets[i] << endl;
            }
            //cout << endl;
            if(possible){
                sum += id;
            }
        }
    }

    file.close();

    cout << sum << endl;

    exit(0);
}