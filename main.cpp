#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include"rlutil.h"
#include <iomanip>
#include <sstream>
using namespace rlutil;
using namespace std;
const int universeSize = 512;

const string banner = "Eyepatch";

const string seperator=",,,,";

static class Settings{
    vector<string> settingName;
    vector<bool> settingState;
public:

    void registerSetting(string name, bool defaultState=true){
        settingName.push_back(name);
        settingState.push_back(defaultState);
    }

    bool getSetting(string name){
        int index;
        for(int i = 0; i < settingName.size(); i++){
            if(settingName[i] == name){
                index = i;
                break;
            }
        }
        return getSetting(index);
    }

    bool getSetting(int index){
        return settingState[index];
    }

    string getSettingName(int index){
        return settingName[index];
    }

    int getSettingsSize(){
        return settingName.size();
    }
    void toggleSetting(int index){
        settingState[index] = !settingState[index];
    }
}settings;

void setColorIf(int code){
    if(settings.getSetting("theme_colours")){
        setColor(code);
    }
}

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
class Block{
public:
    int start;
    int end;
    string name;

    void printInfo(){
        cout << name<<endl;
        cout << "\t"<<start<<"-"<<end<<endl;
    }
    Block(int n){

    }
    Block(int s, int e, string n){
        start = s;
        end = e;
        name = n;
    }
    void serialise(ofstream* out){
        *out<<start<<seperator<<name<<seperator<<end<<"\n";
    }

    void deserialise(string in){
        vector<string> spl = split(in, seperator);

        start=stoi(spl[0]);
        name=spl[1];
        end=stoi(spl[2]);
    }

};

vector<Block> blocks;
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

void saveBlocks(){
    ofstream outfile;

    outfile.open("blocks.icsv", ios::out);

    for(int i =0 ; i < blocks.size(); i++){
        cout << "Saving ";
        blocks[i].printInfo();
        cout <<endl;
        blocks[i].serialise(&outfile);
    }

    outfile.close();

    cout << "Blocks saved, press any key to continue...";
    anykey();
}

void loadBlocks(){
    ifstream infile;
    infile.open("blocks.icsv", ios::in);

    string input;
    while(getline(infile, input)){
        blocks.push_back(Block(NULL));
        blocks.back().deserialise(input);
    }
}

void loadDevices(){
    ifstream infile;
    infile.open("devices.icsv", ios::in);

    string input;
    while(getline(infile, input)){
        devices.push_back(DMXDevice(NULL));
        devices.back().deserialise(input);
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

void newBlock(){
    string name;
    int start;
    int end;

    cout << "Block addition mode:"<<endl;
    cout << "Name: ";
    cin >> name;
    cout <<"Start: ";
    cin >> start;
    cout << "End: ";
    cin >> end;

    blocks.push_back(Block(start,end,name));
}

void printAllBlocks(){
    for(int i = 0; i < blocks.size(); i++){
        cout << i<< ": ";
        blocks[i].printInfo();
    }
    cout<<"Press any key to continue..."<<endl;
    getch();
}

void editBlock(){
    int id;

    cout << "Block edit mode:"<<endl;
    printAllBlocks();
    cout << "Enter Block id: ";

    cin >> id;
    anykey();
    cls();
    cout << "Block edit mode:"<<endl;

    cout << "Editing ";
    devices[id].printInfo();

    cout << "s. Edit start\nn. Edit name\ne. Edit end\nd. Delete Block";
    char in = getch();
    cls();
    cout << "Device edit mode:"<<endl;
    if(in == 's'){
        cout << "Enter new start: ";
        int n;
        cin >> n;

        blocks[id].start=n;
    }else if (in == 'n'){
        cout << "Enter new name: ";
        string n;
        cin>> n;

        blocks[id].name=n;
    }else if (in == 'e'){
        cout << "Enter new end: ";
        int n;
        cin >> n;

        blocks[id].end=n;
    }else if (in == 'd'){
        cout << "Are you sure to delete this block?\n";
        blocks[id].printInfo();
        cout << "y/n\n";
        char yn = getch();
        if(yn == 'y'){
            cout << "deleting block...\n";
            blocks.erase(blocks.begin() + id);
        }
    }
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

    cout << "q. Edit quantity\nn. Edit name\nf. Edit footprint\nd. Delete footprint\n";
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
    }else if (in == 'd'){
        cout << "Are you sure to delete this device?\n";
        devices[id].printInfo();
        cout << "y/n\n";
        char yn = getch();
        if(yn == 'y'){
            cout << "deleting device...\n";
            devices.erase(devices.begin()+ id);
        }
    }
}

void generate(){
    cout << "Generating standard\n";
    bool blocked[512];
    string output = "name,footprint,address\n";
    for(int i = 0; i < 512; i++){
            blocked[i] = false;
    }

    for(int i = 0; i < blocks.size(); i++){
        for(int z = blocks[i].start-1; z< blocks[i].end;z++){
            cout << "Blocking " <<z<<endl;
            blocked[z] = true;
        }
    }

    for(int i = 0; i < devices.size(); i++){
        int requiredSize = devices[i].quantity * devices[i].footprint;
        int foundaddress;
        for(int n = 1; n < 513; n++){//Iterate through entire patch space
            bool bigEnough = true;
            cout << "Searching address " << n<<endl;
            for(int z = n-1; z < requiredSize+n-1; z++){
                cout << "Checking allocation of " << z<<endl;
                if (blocked[z]){
                    cout << "Allocation invalid"<<endl;
                    bigEnough = false;
                    break;
                }
                cout << "Allocation avalible"<<endl;
            }
            if(bigEnough){
                foundaddress = n;
                break;
            }
        }
        for(int z = foundaddress; z < requiredSize+foundaddress; z++){
            cout << "Blocking " << z<<endl;
            blocked[z] = true;
        }
        for(int n = 0; n < devices[i].quantity;n++){
            output+=devices[i].name+"(";
            output+=to_string(n+1)+"),";
            output+=to_string(devices[i].footprint)+",";
            output+=to_string((n*devices[i].footprint)+foundaddress);
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
    hidecursor();
    //Loading process
    loadDevices();
    loadBlocks();

    //Register settings
    settings.registerSetting("csv_output_blocks");
    settings.registerSetting("dmx_adress_connected");
    settings.registerSetting("theme_colours");
    cout <<endl;

    //mainloop
    while (true){
        //header
        cout << "\033[22;31md. devices\n\033[22;32mb. blocks\n\033[22;34mg. generate standard\n\033[01;35ms. settings";
        resetColor();

        //input
        char in = getch();
        cls();
        if(in == 's'){
            int point = 0;
            while(true){
                cls();
                setColorIf(MAGENTA);


                cout << "w and s to navigate, space to toggle\n\n";

                for(int i = 0; i < settings.getSettingsSize(); i++){
                    cout <<setw(30)<<settings.getSettingName(i);
                    if(settings.getSetting(i)){
                        setColor(GREEN);
                        cout <<setw(5)<<"ON";
                    }else{
                        setColor (RED);
                        cout<<setw(5) << "OFF";
                    }
                    resetColor();
                    setColorIf(MAGENTA);

                    if(i == point){
                        cout <<"<";
                    }
                    cout<<endl;
                }
                char in = getch();
                cout << in <<point<<endl;
                if(in == 'w'){
                    point -=1;
                    if(point == -1){
                        point=settings.getSettingsSize()-1;
                    }
                }else if (in == 's'){
                    point += 1;
                    if(point == settings.getSettingsSize()){
                        point = 0;
                    }
                }else if (in == ' '){
                    settings.toggleSetting(point);
                }

            }
        }else if(in == 'd'){
            while (true){
                setColorIf(RED);
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
            while(true){
                setColorIf(GREEN);
                cout <<"n. new block\nl. list blocks\ne. edit blocks\ns. save blocks\nq. back\n";

                char in = getch();
                cls();
                if(in=='n'){
                    newBlock();
                }else if (in == 'l'){
                    printAllBlocks();
                }else if(in == 'e'){
                    editBlock();
                }else if ( in == 'q'){
                    break;
                }else if (in=='s'){
                    saveBlocks();
                }
                cls();
            }
        }else if (in == 'g'){
            generate();
        }


        cls();
    }
    return 0;
}
