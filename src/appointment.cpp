using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "./../include/global.hh"
#include "./../include/appointment.hh"
#include "./../include/hospital.hh"

appointment::appointment()
{
    id = -1;
    D.id = -1;
    P.id = -1;
    r.id = -1;
}
appointment::~appointment()
{
    id = -1;
    D.id = -1;
    P.id = -1;
    r.id = -1;
    return;
}
void appointment::fillMap()
{
    fstream f;
    f.open("./data/appointments.csv", ios::in);
    string temp;
    //skipping the first row containing column headers;
    getline(f >> ws, temp);
    //analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        appointment a;
        //creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s2, s3, s4, s5, s6, s7;
        //reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, s2, ','); // date is of no use here;
        getline(s, s3, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, s6, ',');
        getline(s, s7);
        a.id = strToNum(s1);
        a.D = hospital::doctorsList[strToNum(s3)];
        a.P = hospital::patientsList[strToNum(s4)];
        a.start_hh = strToNum(s5);
        a.end_hh = strToNum(s6);
        a.r = hospital::roomsList[strToNum(s7)];
        hospital::appointmentsList[a.id] = a;
    }
    f.close();
    return;
}
void appointment::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "appointmentId,date(YYYYMMDD),doctorId,patientId,startTime(in 24-hr format),endTime(in 24-hr format),room\n";
    for (auto i : hospital::appointmentsList)
        f << i.second.id << "," << yyyymmdd << "," << i.second.D.id << "," << i.second.P.id
          << "," << i.second.start_hh << "," << i.second.end_hh << "," << i.second.r.id << endl;
    f.close();
    remove("./data/appointments.csv");
    rename("./data/temp.csv", "./data/appointments.csv");
    return;
}
void appointment::printDetails()
{
    if (id == -1)
        return;
    cout << "\n\n\nAppointment Details:\nID                 : " << id << "\n"
         << "Patient's Name     : " + P.firstName + " " + P.lastName + "(ID = " << P.id << ")\n"
         << "Doctor's Name      : " + D.firstName + " " + D.lastName + "(ID = " << D.id << ")\n"
         << "Start_Time (24 Hr format): " << start_hh << ":00 Hrs\n"
         << "End_Time (24 Hr format): " << end_hh << ":00 Hrs\n"
         << "Room               : " + r.HouseNumber + "(ID = " << r.id << ")\n\n";
    return;
}
void appointment::book()
{
    int st, et;
    if (hospital::appointmentsList.size() >= 8 * hospital::doctorsList.size())
    {
        cout << "\n\nSorry, no doctor is available for appointment today!\n\n";
        return;
    }

    bool F = false;
    for (auto i: hospital::roomsList)
        if (i.second.IS_AVAILABLE()) F = true;
    if (!F){
        cout << "\n\nSorry, no room is available for appointment today!\n\n";
        return;
    }

    cout << "\n\nIs the patient already registered (Y : Yes || N : No)?\n";
    string ans;
    cin >> ans;
    while (ans != "Y" && ans != "N"){
        cout << "Y or N?\n";
        cin >> ans;
    }
    if (ans == "N")
    {
        cout << "Register the patient:\n";
        P.addPerson();
    }
    else
    {
        cout << "Search for the required patient:\n\n";
        ans = "Y";
        while (ans == "Y"){
            P.getDetails();
            ans = "K";
            if (P.id == -1){
                cout << "Try again (Y : Yes || N : No)?\n";
                cin >> ans;
                while (ans != "Y" && ans != "N"){
                    cout << "Y or N?\n";
                    cin >> ans;
                }
            }
            else if (!P.alive){
                cout << "Selected patient has gone. We hope his soul rests in peace, amen!\n";
                cout << "Try again (Y : Yes || N : No)?\n";
                cin >> ans;
                while (ans != "Y" && ans != "N"){
                    cout << "Y or N?\n";
                    cin >> ans;
                }
                P.id = -1;
            }
        }
        if (ans == "Y"){
            return;
        }
    }
    cout << "\n\nNow, search for the required doctor:\n";
    ans = "Y";
    while (ans == "Y"){
        D.getDetails();
        ans = "K";
        if (D.id == -1){
            cout << "Try again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N")
            {
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else if (D.appointmentsBooked >= 8){
            cout << "Sorry, selected doctor has no free slot left for the day!\n";
            cout << "Search again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N"){
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else if (D.last_hh >= 22){
            cout << "Sorry, selected doctor has no free slot left for the day!\n";
            cout << "Search again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N"){
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else{
            cout << "Please enter start time of the appointment: ";
            cin >> st;
            cout << "Please enter end time of the appointment: ";
            cin >> et;
            while (st >= et || st < 8 || et > 22){
                cout << "Invalid! Please enter a valid start time of the appointment (Start time should no less than 8): ";
                cin >> st;
                cout << "Please a valid end time of the appointment(End time should no greater than 22): ";
                cin >> et;
            }
            if (st < D.last_hh){
                cout << "Sorry, selected doctor has no free slot left for that time range!\n";
                cout << "Search again (Y : Yes || N : No)?\n";
                cin >> ans;
                while (ans != "Y" && ans != "N"){
                    cout << "Y or N?\n";
                    cin >> ans;
                }
            }
        }
    }
    if (ans == "N"){
        return;
    }

    cout << "\n\nNow, search for the required room:\n";
    ans = "Y";
    while (ans == "Y")
    {
        r.getDetails();
        ans = "K";
        if (r.id == -1)
        {
            cout << "Try again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N")
            {
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else if (!r.IS_AVAILABLE())
        {
            cout << "Sorry, selected room has no free slot left for today!\n";
            cout << "Search again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N")
            {
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else if (r.last_hh > st){
            cout << "Sorry, selected room has no free slot left for that time range!\n";
            cout << "Search again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != "Y" && ans != "N"){
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
    }

    if (ans == "N"){
        return;
    }


    if (hospital::appointmentsList.rbegin() != hospital::appointmentsList.rend())
        id = ((hospital::appointmentsList.rbegin())->first) + 1;
    else
        id = 1;
    start_hh = st;
    end_hh = et;
    D.last_hh = et;
    r.last_hh = et;
    hospital::appointmentsList[id] = *this;

    hospital::doctorsList[D.id].appointmentsBooked++;
    hospital::roomsList[r.id].PersonNum += 2;

    cout << "\nAppointment of patient " + P.firstName + " " + P.lastName + " with doctor "
         << D.firstName << " " << D.lastName << " with room" << r.HouseNumber 
         << "from " << start_hh << ":00 hrs to" << end_hh << ":00 hrs"
         <<" booked successfully!\n";
    printDetails();
    return;
}
void appointment::getDetails()
{
    cout << "\nEnter appointment ID:\n";
    cin >> id;
    if (hospital::appointmentsList.find(id) == hospital::appointmentsList.end())
    {
        cout << "\nInvalid appointment ID!\n";
        id = -1;
        return;
    }
    *this = hospital::appointmentsList[id];
    return;
}