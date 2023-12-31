#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <map>
#include <set>

using namespace std;

//Extracts individual x,m,a,s values from part
int extractInt(string& str){
    smatch m;
    regex digits("\\d+");
    regex_search(str,m,digits);
    string res = m.str();
    str = str.substr(m.position() + m.length());
    return stoi(res);
}

//Part class, only needs x,m,a,s values
struct Part{
    int x;
    int m;
    int a;
    int s;
    Part(int x, int m, int a, int s){
        this->x = x;
        this->m = m;
        this->a = a;
        this->s = s; 
    }
};

//Workflow class, has vectors of tests, corresponding destinations, the amount of both, and the next label if no tests are passed
struct Workflow{
    vector<string> tests;
    vector<string> dests;
    size_t testCount;
    string next;
    Workflow(vector<string> tests, vector<string> dests, string next){
        this->tests = tests;
        this->dests = dests;
        this->next = next;
        this->testCount = tests.size();
    }
    //Check part against all tests, return appropriate next destination
    string test(Part& p){
        regex op("[<>]");
        smatch m;
        string res;
        for(int i = 0; i < this->testCount; i++){
            //Extract test's category, operation, and value to be tested against
            //Can be hardcoded because category and operation are always 1 value
            char cat = tests[i][0];
            char op = tests[i][1];
            int value = stoi(tests[i].substr(2));
            int partNum = 0;
            bool passesTest = false;
            //Assign value to be checked based on what the test wants
            switch(cat){
                case 'x':
                    partNum = p.x;
                    break;
                case 'm':
                    partNum = p.m;
                    break;
                case 'a':
                    partNum = p.a;
                    break;
                case 's':
                    partNum = p.s;
                    break;
            }

            //cout << "Testing " << partNum << " against " << cat << " " << op << " " << value << endl; 

            //Check if test is passed
            if(op == '<'){
                passesTest = partNum < value;
            }
            else{
                passesTest = partNum > value;
            }

            //Return appropriate destination if test is passed
            if(passesTest){
                return dests[i];
            }
        }
        //No tests are passed, return default destination
        return next;
    }
};

int main(){
    string filename;
    fstream file;
    long long total = 0;

    vector<Part> parts;
    map<string, Workflow> workflows;
    smatch m;
    smatch split;
    regex insns("\\{.*\\}");
    regex notComma("[^,]+");
    regex colon(":");

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;

    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    //Read in workflows
    while(getline(file,line) && line != ""){
        regex_search(line,m,insns);
        string label = line.substr(0, m.position());
        string noBraces = m.str().substr(1, m.length() - 2);
        vector<string> tests;
        vector<string> dests;
        string next; 
        //Extract tests and default next destination
        while(regex_search(noBraces,m,notComma)){
            string str = m.str();
            //If string is a test
            if(regex_search(str,split,colon)){
                string test = str.substr(0,split.position());
                string dest = str.substr(split.position() + split.length());
                tests.push_back(test);
                dests.push_back(dest);
            }
            //Not a test, string is the default destination
            else{
                next = str;
            }
            noBraces = noBraces.substr(m.position() + m.length());
        }
        workflows.insert(make_pair(label,Workflow(tests,dests,next)));
    }

    /*
    for(auto w : workflows){
        cout << "Label is " << w.first << endl;
        for(int i = 0; i < w.second.tests.size(); i++){
            cout << "Test is " << w.second.tests[i] << ", dest is " << w.second.dests[i] << endl;
        }
        cout << "Next is " << w.second.next << endl << endl;
    }
    */

    //Read in parts
    while(getline(file, line)){
        //Extract integers from string, add to vector of parts
        int x = extractInt(line);
        int m = extractInt(line);
        int a = extractInt(line);
        int s = extractInt(line);
        parts.push_back(Part(x,m,a,s));
    }

    /*
    for(auto p : parts){
        cout << p.x << " " << p.m << " " << p.a << " " << p.s << endl;
    }
    */

    file.close();

    //Check if each part passes or not
    for(auto part : parts){
        string cur = "in";
        while(cur != "R" && cur != "A"){
            if(auto it = workflows.find(cur); it != workflows.end()){
                cur = it->second.test(part);
            }
        }
        //cout << "Part ended up at " << cur << endl;
        //Add total of part values to final total
        if(cur == "A"){
            total += part.x + part.m + part.a + part.s;
        }
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}
