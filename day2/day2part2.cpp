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

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        while(getline(file,line)){

            int id = 0;
            int maxRed = 0;
            int maxGreen = 0;
            int maxBlue = 0;

            //Split along colon
            vector<string> splitLine = split(line,":");

            //Split subsets by semicolons
            vector<string> subsets = split(splitLine[1],";");

            //Count reds, greens, and blues from each subset and get max values of each
            smatch m;
            regex digits = regex("\\d+");
            regex red = regex("red");
            regex green = regex("green");
            regex blue = regex("blue");
            for(int i = 0; i < subsets.size(); i++){
                vector<string> colors = split(subsets[i],",");
                for(int j = 0; j < colors.size(); j++){
                    if(regex_search(colors[j],m,red)){
                        regex_search(colors[j],m,digits);
                        int redCount = stoi(m.str());
                        if(redCount > maxRed){
                            maxRed = redCount;
                        }
                    }
                    else if(regex_search(colors[j],m,green)){
                        regex_search(colors[j],m,digits);
                        int greenCount = stoi(m.str());
                        if(greenCount > maxGreen){
                            maxGreen = greenCount;
                        }
                    }
                    else if(regex_search(colors[j],m,blue)){
                        regex_search(colors[j],m,digits);
                        int blueCount = stoi(m.str());
                        if(blueCount > maxBlue){
                            maxBlue = blueCount;
                        }
                    }
                }
            }

            //cout << maxRed << " " << maxGreen << " " << maxBlue << " " << maxRed * maxGreen * maxBlue << endl;
            sum += maxRed * maxGreen * maxBlue;
        }
    }

    file.close();

    cout << sum << endl;

    exit(0);
}