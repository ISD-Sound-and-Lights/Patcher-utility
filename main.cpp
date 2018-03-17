#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include"rlutil.h"
#include <iomanip>
using namespace rlutil;
using namespace std;

const int universeSize = 512;

const string banner = "Eyepatch";
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

vector<DMXDevice> devices;
void saveDevices(){
    ofstream outfile;

    outfile.open("devices.icsv", ios::out);

    for(int i =0 ; i < devices.size(); i++){
        cout << "Saving ";
        devices[i].printInfo();
        cout <<endl;
        devices[i].serialise(&outfile);
    }

    outfile.close();

    cout << "Devices saved, press any key to continue...";
    anykey();
}

void loadDevices(){
    ifstream infile;
    infile.open("devices.icsv", ios::in);

    string input;
    vector<string> splitinput;

    infile>>input;
    splitinput = split(input, "\n");
    if(splitinput.size()==0){
        cout << splitinput.size();
        return;
    }
    for(int i = 0; i < splitinput.size(); i++){
        devices.push_back(DMXDevice(NULL));
        devices[i].deserialise(splitinput[i]);
    }
}

void printAllDevices(){
    for(int i = 0; i < devices.size(); i++){
        cout << i<< ": ";
        devices[i].printInfo();
    }
    cout<<"Press any key to continue..."<<endl;
    getch();
}

void newDevice(){
    string name;
    int quantity;
    int footprint;

    cout << "Device addition mode:"<<endl;
    cout << "Name: ";
    cin >> name;
    cout <<"Quantity: ";
    cin >> quantity;
    cout << "DMX Footprint: ";
    cin >> footprint;

    devices.push_back( DMXDevice(footprint,name,quantity));
}

void editDevice(){
    int id;

    cout << "Device edit mode:"<<endl;
    printAllDevices();
    cout << "Enter device id: ";

    cin >> id;
    anykey();
    cls();
    cout << "Device edit mode:"<<endl;

    cout << "Editing ";
    devices[id].printInfo();

    cout << "q. Edit quantity\nn. Edit name\nf. Edit footprint\n";
    char in = getch();
    cls();
    cout << "Device edit mode:"<<endl;
    if(in == 'q'){
        cout << "Enter new quantity: ";
        int n;
        cin >> n;

        devices[id].quantity=n;
    }else if (in == 'n'){
        cout << "Enter new name: ";
        string n;
        cin>> n;

        devices[id].name=n;
    }else if (in == 'f'){
        cout << "Enter new DMX footprint: ";
        int n;
        cin >> n;

        devices[id].footprint=n;
    }
}

void generate(){
    cout << "Generating standard\n";
    bool blocked[512];
    string output = "";
    for(int i = 0; i < 512; i++){
        blocked[i] = false;
    }

    for(int i = 0; i < devices.size(); i++){
        int requiredSize = devices[i].quantity * devices[i].footprint;
        int foundAdress;
        for(int n = 1; n < 513; n++){//Iterate through entire patch space
            bool bigEnough = true;
            for(int z = n-1; z < requiredSize; z++){
                if (blocked[z]){
                    bigEnough = false;
                    break;
                }
            }
            if(bigEnough){
                foundAdress = n;
                break;
            }
        }
        for(int z = foundAdress; z < requiredSize; z++){
            blocked[z] = true;
        }
        for(int n = 0; n < devices[i].quantity;n++){
            output+=devices[i].name+"(";
            output+=to_string(n+1)+"),";
            output+=to_string(devices[i].footprint)+",";
            output+=to_string((n*devices[i].footprint)+foundAdress);
            output+="\n";
        }
    }

    ofstream outfile;
    outfile.open("PatchingStandard.csv");
    outfile<<output;
    outfile.close();
    cout << "Standard generation complete, press any key to continue\n";
    anykey();
}

int main(){
    cout << banner;
    loadDevices();
    cout <<endl<<endl;
    while (true){
        cout << "d. devices\nb. blocks\ng. generate standard\n";
        char in = getch();
        cls();
        if(in == 'd'){
            while (true){
                cout <<"n. new device\nl. list device\ns. save devices\ne. edit device\nq. back\n";

                char in = getch();
                cls();
                if(in == 'n'){
                    newDevice();
                }else if (in == 'l'){
                    printAllDevices();
                }else if (in == 's'){
                    saveDevices();
                }else if (in == 'e'){
                    editDevice();
                }else if (in=='q'){
                    break;
                }
                cls();
            }
        }else if (in == 'b'){
            
        }else if (in == 'g'){
            generate();
        }


        cls();
    }
    return 0;
}
