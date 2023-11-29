using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "./../include/global.hh"
#include "./../include/room.hh"
#include "./../include/hospital.hh"

room::room(){
    id = -1;
    category = 1;
    PersonNum = 0;
    last_hh = 8;
}

void room::fillMap(){
    fstream f;
    f.open("./data/rooms.csv", ios::in);
    string temp;
    //skipping the first row containing column headers;
    getline(f >> ws, temp);
    //analyzing each entry afterwards;
    while (getline(f >> ws, temp)){
        room r;
        //creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s3, s4, s5, s6;
        getline(s, s1, ',');
        getline(s, r.HouseNumber, ',');
        getline(s, s3, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, s6, ',');
        r.id = strToNum(s1);
        r.category = strToNum(s3);
        r.volume = strToNum(s4);
        r.PersonNum = strToNum(s5);
        r.last_hh = strToNum(s6);
        hospital::roomsList[r.id] = r;
    }
    f.close();
    return;
}

void room::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "roomId,HouseNumber,category,volume,PersonNum,Is_available\n";
    for (auto i : hospital::roomsList)
        f << i.second.id << "," << i.second.HouseNumber << "," << i.second.category << "," << i.second.volume
          << "," << i.second.PersonNum << "," << i.second.last_hh << endl;
    f.close();
    remove("./data/rooms.csv");
    rename("./data/temp.csv", "./data/rooms.csv");
    return;
}

void room::addRoom(){
    if (hospital::roomsList.size() == hospital::roomsLimit){
        cout << "\n\nRooms limit reached, can't add more!\n\n";
        return;
    }
    //get basic details of the room from the user side;
    cout << "\nEnter HouseNumber:\n";
    getline(cin >> ws, HouseNumber);
    cout << "\nEnter category:\n";
    cin >> category;
    cout << "\nEnter volume:\n";
    cin >> volume;
    if (hospital::roomsList.rbegin() != hospital::roomsList.rend())
        id = ((hospital::roomsList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::roomsList[id] = *this;

    //creating a fstream object to read/write from/to files;
    fstream f;
    //creating a record in roomsHistory.csv;
    f.open("./data/roomsHistory.csv", ios::app);
    f << endl << HouseNumber << "," << category << "," << volume << ",N";
    f.close();

    cout << "\n"
         << HouseNumber << " registered successfully!\n";
    cout << "Its ID is: " << id << "\n";

    return;
}

void room::printDetails()
{
    if (id == -1)   return;
    cout << "\nDetails:\n";
    cout << "ID              : " << id << "\n";
    cout << "HouseNumber     : " << HouseNumber << "\n";
    cout << "Category        : " << category << "\n";
    cout << "Volume          : " << volume << "\n";
    cout << "PersonNum       : " << PersonNum << "\n";
    cout << "LastBooktime    : " << last_hh << "\n";
    return;
}

void room::printDetailsFromHistory(string extraDetails){
    if (id == -1)   return;
    cout << "\nHistory Details :\n";
    cout << "HouseNumber     : " << HouseNumber << "\n";
    cout << "Category        : " << category << "\n";
    cout << "Volume          : " << volume << "\n";
    stringstream k(extraDetails);
    string s4;
    getline(k, s4);
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/roomsHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column headers;
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp)){
            room r;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s2, s3;
            //reading from the string stream object 's';
            getline(s, r.HouseNumber, ',');
            getline(s, s2, ',');
            getline(s, s3, ',');
            if (r.HouseNumber == HouseNumber){
                getline(s, s4);
            }
        }
        f.close();
    }
    cout << "Removed?      : " << s4 << "\n";
    return;
}

void room::getDetails(int rec){
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by HouseNumber\n[3]: Filter by Category\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2 && opt != 3)
        cout << "option 1, 2 or 3?\n", cin >> opt;
    //1: Filter by ID;
    if (opt == 1){
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::roomsList.find(reqId) != hospital::roomsList.end())
            *this = hospital::roomsList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    //2: Filter by HouseNumber;
    else if (opt == 2){
        string reqHouseNumber;
        cout << "HouseNumeber:\n";
        getline(cin >> ws, reqHouseNumber);
        vector<room> matchingRecords;
        for (auto i : hospital::roomsList){
            i.second.printDetails();
            if (i.second.HouseNumber == reqHouseNumber)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        string tt = "N";
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required room: ";
                cin >> reqId;
                if (hospital::roomsList.find(reqId) != hospital::roomsList.end())
                    *this = hospital::roomsList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != "Y" || tt != "N")
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == "Y");
        }
    }
    //3: Filter by category;
    else if (opt == 3)
    {
        int reqCategory;
        cout << "Enter the category of room required:\n";
        cin >> reqCategory;
        vector<room> matchingRecords;
        for (auto i : hospital::roomsList)
        {
            if (i.second.category == reqCategory)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        string tt = "N";
        if (matchingRecords.size() > 0)
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required room: ";
                cin >> reqId;
                if (hospital::roomsList.find(reqId) != hospital::roomsList.end())
                    *this = hospital::roomsList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != "Y" || tt != "N")
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == "Y");
    }
    return;
}

void room::getDetailsFromHistory(){
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by HouseNumber\n[2]: Filter by Category\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;
        //1: Filter by name;
    if (opt == 1){
        string reqHouseNumber;
        cout << "HouseNumber:\n";
        getline(cin >> ws, reqHouseNumber);
        vector<room> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/roomsHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column headers;
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            room r;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s2, s3, s4;
            //reading from the string stream object 's';
            getline(s, r.HouseNumber, ',');

            if (r.HouseNumber == reqHouseNumber)
            {
                getline(s, s2, ',');
                getline(s, s3, ',');
                getline(s, s4);
                r.id = 0;
                r.category = strToNum(s2);
                r.volume = strToNum(s3);
                matchingRecords.push_back(r);
                extraDetails.push_back(s4);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    //2: Filter by category;
    else if (opt == 2){
        int reqCategory;
        cout << "Enter the category of room required:\n";
        cin >> reqCategory;
        vector<room> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/roomsHistory.csv", ios::in);
        string temp;
        //skipping the first row containing column headers;
        getline(f >> ws, temp);
        //analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            room r;
            //creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s2, s3, s4;
            //reading from the string stream object 's';
            getline(s, r.HouseNumber, ',');
            getline(s, s2, ',');
            getline(s, s3, ',');
            if (strToNum(s2) == reqCategory)
            {
                getline(s, s4);
                r.id = 0;
                r.category = strToNum(s2);
                r.volume = strToNum(s3);
                matchingRecords.push_back(r);
                extraDetails.push_back(s4);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    return;
}

void room::removeRoom(){
    cout << "\nSearch for the room you want to remove.\n";
    getDetails();
    if (id == -1)   return;
    if (PersonNum){
        cout << "\nSelected room has person now, can't be removed.\n\n";
        return;
    }
    hospital::roomsList.erase(id);
    string s, temp;
    stringstream str;
    fstream f, fout;
    str << HouseNumber << "," << category << "," << volume << ",N\n";
    getline(str, s);
    f.open("./data/roomsHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp)){
        if (temp == s){
            fout << "\n" << HouseNumber << "," << category << "," << volume << ",Y";
        }
        else
            fout << "\n" << temp;
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/roomsHistory.csv");
    rename("./data/temp.csv", "./data/roomsHistory.csv");
    cout << HouseNumber << " removed successfully!\n";
    return;
}

bool room::IS_AVAILABLE(){
    return (PersonNum + 2 <= volume) ? true : false;
}