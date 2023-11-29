#ifndef APPOINTMENT
#define APPOINTMENT
using namespace std;

#include "./patient.hh"
#include "./doctor.hh"
#include "./room.hh"

class appointment
{
private:
    int id;
    doctor D;
    patient P;
    int start_hh; //hh -> start hour in 24 hour format;
    int end_hh; //hh -> end hour in 24 hour format;
    room r;
public:
    appointment();
    ~appointment();
    void fillMap();
    void saveMap();
    void printDetails();
    void book();
    void getDetails();
};
#endif // !APPOINTMENT