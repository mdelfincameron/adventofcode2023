#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main(){
    string filename;
    fstream file;
    long sum = 0;
    smatch m;
    vector<regex> regexes = {regex("one"), regex("two"), regex("three"), regex("four"), regex("five"), regex("six"), regex("seven"), regex("eight"), regex("nine")};

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    file.open(filename,ios::in);

    if(file.is_open()){

        //Read line by line through file
        string line;
        while(getline(file,line)){

            int firstNum = -1;
            int lastNum = -1;
            int firstPos = -1;
            int lastPos = -1;
            int length = line.length();
            int regexLength = regexes.size();

            //Check for digit(s) in line, save their positions as well
            for(int i = 0; i < length; i++){
                if(isdigit(line[i])){
                    if(firstNum == -1){
                        firstNum = line[i] - '0';
                        firstPos = i;
                    }
                    lastNum = line[i] - '0';
                    lastPos = i;
                }
            }


            //Regex search for instances of alphabetical number representations
            string initialLine = line;
            for(int i = 0; i < regexLength; i++){

                int startingPos = 0;

                while(regex_search(line,m,regexes[i])){

                    int matchPos = m.position() + startingPos;

                    if(firstPos == -1 || firstPos > matchPos){
                        firstNum = i + 1;
                        firstPos = matchPos;
                    }

                    if(lastPos < matchPos){
                        lastNum = i + 1;
                        lastPos = matchPos;
                    }

                    startingPos += m.position() + m.length();
                    line = initialLine.substr(startingPos);
                }
                line = initialLine;
            }

            //cout << firstNum << " " << lastNum << endl;

            //Add 2-digit number made of first number and last number to final sum
            if(firstNum != -1){
                sum += (firstNum * 10) + lastNum;
            }
        }
    }

    file.close();

    cout << sum << endl;

    exit(0);
}