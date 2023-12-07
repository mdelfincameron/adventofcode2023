#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <algorithm>

using namespace std;

enum handTypes{
    highCard,
    onePair,
    twoPair,
    threeOfAKind,
    fullHouse,
    fourOfAKind,
    fiveOfAKind
};

int cardToInt(char c){
    switch(c){
        case('T'):
            return 10;
        case('J'):
            return 11;
        case('Q'):
            return 12;
        case('K'):
            return 13; 
        case('A'):
            return 14;
        default:
            return c - '0';
    }
}

class Card{
    public:
        string str;
        int bid;
        int handType = -1;
        vector<int> cardCounts;
        Card(string str, int bid);
        void parse();
};

Card::Card(string str, int bid){

    this->str = str;
    this->bid = bid;
    vector<int> cards(14,0);
    cardCounts = cards;

    for(int i = 0; i < str.length(); i++){
        cardCounts[cardToInt(str[i]) - 1]++;
    }
}

void Card::parse(){
    bool twoExists = false;
    bool threeExists = false;
    int pairCount = 0;

    for(auto i : cardCounts){
        switch(i){
            case 5:
                handType = fiveOfAKind;
                break;
            case 4:
                handType = fourOfAKind;
                break;
            case 3:
                if(twoExists){
                    handType = fullHouse;
                }
                threeExists = true;
                break;
            case 2:
                if(threeExists){
                    handType = fullHouse;
                }
                pairCount++;
                twoExists = true;
                break;
        }
    }

    if(handType == -1){
        if(threeExists){
            handType = threeOfAKind;
        }
        else if(pairCount == 2){
            handType = twoPair;
        }
        else if(pairCount == 1){
            handType = onePair;
        }
        else{
            handType = highCard;
        }
    }
}

bool cardCompare(Card a, Card b){
    if(a.handType != b.handType){
        return a.handType < b.handType;
    }
    int length = a.str.length();
    for(int i = 0; i < length; i++){
        int ai = cardToInt(a.str[i]);
        int bi = cardToInt(b.str[i]);
        if(ai != bi){
            return ai < bi;
        }
    }
    return 0;
}

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
    long long total = 0;
    vector<Card> cards;

    //Get filename, open file
    cout << "Please input filename: ";
    cin >> filename;
    
    auto start = chrono::steady_clock::now();
    file.open(filename,ios::in);

    if(!file.is_open()){
        exit(1);
    }

    string line;

    while(getline(file,line)){

        //Separate card and bid
        vector<string> splitLine = split(line," ");
        Card c = Card(splitLine[0], stoi(splitLine[1]));
        c.parse();
        cards.push_back(c);

    }
    
    file.close();

    sort(cards.begin(), cards.end(), cardCompare);

    for(int i = 0; i < cards.size(); i++){
        //cout << cards[i].bid << "->" << cards[i].bid * (i + 1) << endl;
        total += cards[i].bid * (i + 1);
    }

    auto end = chrono::steady_clock::now();

    cout << total << endl;
    cout << "Runtime is " << chrono::duration <double, milli> (end - start).count() << " ms" << endl; 

    exit(0);
}