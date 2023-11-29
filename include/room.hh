#ifndef ROOM
#define ROOM
using namespace std;
#include <string>

class appointment;
class room
{
protected:
    int id;
    string HouseNumber; 
    int category;
    //category: 1:Outpatient Department; 2:Emergency Department; 3:Inpatient Department; 4:Ward
    int volume;
    int PersonNum;
    int last_hh;
    friend class appointment;

public:
    room();
    void fillMap();
    void saveMap();
    void addRoom();
    void printDetails();
    void printDetailsFromHistory(string extraDetails = "");
    void getDetails(int rec = 0);
    void getDetailsFromHistory();
    void removeRoom();
    bool IS_AVAILABLE();
};
#endif // !ROOM
