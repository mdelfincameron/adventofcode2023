#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <map>
#include <set>

using namespace std;

const int MIN = 1;
const int MAX = 4000;

struct Range{
    int min, max;
    Range(){
        min = MIN;
        max = MAX;
    }
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};

//Range of each part
struct PartRange{
    string label;
    Range x;
    Range m;
    Range a;
    Range s;
    PartRange(string label, Range x, Range m, Range a, Range s){
        this->label = label;
        this->x = x;
        this->m = m;
        this->a = a;
        this->s = s;
    }
};

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
    string test(PartRange& p){
        regex op("[<>]");
        smatch m;
        string res;
        for(int i = 0; i < this->testCount; i++){
            //Extract test's category, operation, and value to be tested against
            //Can be hardcoded because category and operation are always 1 value
            char cat = tests[i][0];
            char op = tests[i][1];
            int value = stoi(tests[i].substr(2));

            Range * r;
            PartRange passRange(p.label, p.x, p.m, p.a, p.s);
            Range * pass;

            int testMin = MIN;
            int testMax = MAX;

            //Assign value to be checked based on what the test wants
            switch(cat){
                case 'x':
                    pass = &passRange.x;
                    r = &p.x;
                    break;
                case 'm':
                    pass = &passRange.m;
                    r = &p.m;
                    break;
                case 'a':
                    pass = &passRange.a;
                    r = &p.a;
                    break;
                case 's':
                    pass = &passRange.s;
                    r = &p.s;
                    break;
            }

            //cout << "Testing " << partNum << " against " << cat << " " << op << " " << value << endl; 

            //Check if test is passed
            if(op == '<'){
                testMax = value;
            }
            else{
                testMin = value;
            }

            int lower = max(r->min, testMin);
            int upper = min(r->max, testMax);
            if(upper > lower){
                pass->max = upper;
                pass->min = lower;
            }
            else{

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

    map<string, Workflow> workflows;
    smatch m;
    smatch split;
    regex insns("\\{.*\\}");
    regex notComma("[^,]+");
    regex colon(":");
    vector<PartRange> valid;

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
        while(regex_search(noBraces,m,notComma)){
            string str = m.str();
            if(regex_search(str,split,colon)){
                string test = str.substr(0,split.position());
                string dest = str.substr(split.position() + split.length());
                tests.push_back(test);
                dests.push_back(dest);
            }
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

    file.close();

    //Test all possible x,m,a,s values
    vector<PartRange> ranges(1, PartRange("in",Range(),Range(),Range(),Range()));
    for(auto i : ranges){
        cout << i.label << endl;
    }
    while(!ranges.empty()){
        auto range = ranges.begin();
        while(range->label != "R" && range->label != "A"){
            if(auto it = workflows.find(range->label); it != workflows.end()){
                range->label = it->second.test(*range);
            }
        }
        if(range->label == "A"){
            total += (range->x.max - range->x.min) * (range->m.max - range->m.min) * 
            (range->a.max - range->a.min) * (range->s.max - range->a.min);
        }
        ranges.erase(range);
    }

    /*
    for(int i = 1; i <= 4000; i++){
        vector<string> cur(4,"in");
        vector<char> chars = {'x','m','a','s'};
        cout << i << endl;
        for(int j = 0; j < cur.size(); j++){
            while(cur[j] != "R" && cur[j] != "A"){
                if(auto it = workflows.find(cur[j]); it != workflows.end()){
                    cur[j] = it->second.test(chars[j],i);
                }
            }
            if(cur[j] == "A"){
                totals[j]++;
            }
        }
    }
    */

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}
