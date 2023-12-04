#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string filename;
    fstream file;
    long sum = 0;

    cout << "Please input filename: ";
    cin >> filename;
    
    file.open(filename,ios::in);

    if(file.is_open()){
        string line;
        while(getline(file,line)){

            int firstnum = -1;
            int lastnum = -1;
            int length = line.length();

            for(int i = 0; i < length; i++){
                if(isdigit(line[i])){
                    if(firstnum == -1){
                        firstnum = line[i] - '0';
                    }
                    lastnum = line[i] - '0';
                }
            }
            //cout << firstnum << lastnum << endl;
            if(firstnum != -1){
                sum += (firstnum * 10) + lastnum;
            }
        }
    }

    file.close();

    cout << sum << endl;

    exit(0);
}