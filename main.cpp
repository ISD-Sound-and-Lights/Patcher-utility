#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include"rlutil.h"
#include <iomanip>
using namespace rlutil;
using namespace std;

const string seperator=",,,,";

vector<string> split(string str, string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current != NULL){
        arr.push_back(current);
        current=strtok(NULL, sep.c_str());
    }
    return arr;
}

class DMXDevice{
public:
    int footprint;
    string name;
    int quantity;

    DMXDevice(int n){

    }

    DMXDevice(int f, string n, int q){
        footprint=f;
        name=n;
        quantity=q;
    }

    void serialise(ofstream* out){
        *out<<footprint<<seperator<<name<<seperator<<quantity<<"\n";
    }

    void deserialise(string in){
        vector<string> spl = split(in, seperator);

        footprint=stoi(spl[0]);
        name=spl[1];
        cout << spl[3]<<endl;
        quantity=stoi(spl[2]);
    }
    void printInfo(){
        cout << name<<endl;
        cout << "\tFootprint: "<<setw(5)<<footprint<<endl;
        cout << "\tQuantity: " << setw(5)<<quantity <<endl;
    }
};

int main(){

    //tests
    /*
    DMXDevice testDevice(NULL);
    ifstream infile;
    infile.open("devices.icsv", ios::in);
    string input;
    infile>>input;
    vector<string> splitinput;
    splitinput = split(input, "\n");
    testDevice.deserialise(splitinput[0]);
    testDevice.printInfo();
    infile.close();*/
    /*ofstream outfile;
    outfile.open("devices.icsv", ios::out|ios::app);
    testDevice.serialise(&outfile);
    outfile.close();*/
    return 0;
}
