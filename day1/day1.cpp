#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

            int firstNum = -1;
            int lastNum = -1;
            int length = line.length();

            //Check for digit(s) in line
            for(int i = 0; i < length; i++){
                if(isdigit(line[i])){
                    if(firstNum == -1){
                        firstNum = line[i] - '0';
                    }
                    lastNum = line[i] - '0';
                }
            }

            //cout << firstNum << lastNum << endl;

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