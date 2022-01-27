#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

class Event{
    public:
        time_t begin, end; // [start,stop)
        string name, author, location;
        //bool isNotified;
};

class Recur{
    //Periodical events
    public:
        // 'd' = daily, 'w' = weekly, 'm' = monthly, 'y' = yearly
        time_t begin, end;
        char type;
        string name, author, location;
};


vector<Event> events;
vector<Recur> recurs;


vector<Event> importEvent(){ //⮹
//import Events from .txt file into a vector
    vector<Event> events;
    ifstream source("events.txt");
    if(!source.fail()){
        string line;
        char c[3][100];
        Event temp;
        while(getline(source,line)){
            sscanf("%d %d %[^:]%[^:]%[^\n]", line.c_str(), &temp.begin, &temp.end, c[0], c[1], c[2]);
            temp.name = c[0];
            temp.author = c[1];
            temp.location = c[2];
            events.push_back(temp);
        }
    }
    return events;
}

vector<Recur> importRecur(){
//import Recurs from .txt file into a vector
    vector<Recur> recurs;
    ifstream source("recurs.txt");
    if(!source.fail()){
        string line;
        char c[3][100];
        Recur temp;
        while(getline(source,line)){
            sscanf("%d %d %c %[^:]%[^:]%[^\n]", line.c_str(), &temp.begin, &temp.end, &temp.type, c[0], c[1], c[2]);
            temp.name = c[0];
            temp.author = c[1];
            temp.location = c[2];
            recurs.push_back(temp);
        }
    }
    return recurs;
}

void updateEvent(){
    ofstream source("events.txt");
    for(unsigned int i = 0; i < events.size(); i++){
        source << events[i].begin << ' ';
        source << events[i].end << ' ';
        source << events[i].name << ':';
        source << events[i].author << ':';
        source << events[i].location << '\n';
    }
    source.close();
}

void updateRecur(){
    ofstream source("recurs.txt");
    for(unsigned int i = 0; i < recurs.size(); i++){
        source << recurs[i].begin << ' ';
        source << recurs[i].end << ' ';
        source << recurs[i].type << ' ';
        source << recurs[i].name << ':';
        source << recurs[i].author << ':';
        source << recurs[i].location << '\n';
    }
    source.close();
}

void sortEvent(vector<Event> &events){
    time_t temp;
	int ptr;
	for(unsigned int i = 1; i < events.size(); i++){
		temp = events[i].begin;
		for(ptr = i-1; (ptr >= 0 && events[ptr].begin < temp); ptr--) 
			events[ptr+1].begin = events[ptr].begin;
		events[ptr+1].begin = temp;
	}
}

void printEvent(vector<Event> events = ::events, int i = -1){
    tm temp[2];
    temp[0] = *localtime(&events[i].begin);
    temp[1] = *localtime(&events[i].end);
    if(i != -1) {cout << "[" << i+1 << "] : ";}
    if(temp[0].tm_mday == temp[1].tm_mday && temp[0].tm_mon == temp[1].tm_mon && temp[0].tm_year == temp[1].tm_year){
        cout << "From    " << temp[0].tm_hour << '.' << temp[0].tm_min;
        cout << "    to    " << temp[1].tm_hour << '.' << temp[1].tm_min;
        cout << "   on   " << temp[0].tm_mday << '/' << temp[0].tm_mon << '/' << temp[0].tm_year+1900;
        cout << "    by" << left << setw(30) << events[i].name << "at" << setw(30) << events[i].location << '\n';
    }else{
        cout << "From    " << temp[0].tm_hour << '.' << temp[0].tm_min;
        cout << "   on   " << temp[0].tm_mday << '/' << temp[0].tm_mon << '/' << temp[0].tm_year+1900;
        cout << "    to    " << temp[1].tm_hour << '.' << temp[1].tm_min;
        cout << "   on   " << temp[1].tm_mday << '/' << temp[1].tm_mon << '/' << temp[1].tm_year+1900;
        cout << "    by" << left << setw(30) << events[i].name << "at" << setw(30) << events[i].location << '\n';
    }
}

void addEvent(){
    return;
}

void passEvent(){
    time_t currTime time(0);
    for(unsigned int i = 0; i < events.size(); i++){
        if(events[i].end < currTime){
            events.erase(events.begin()+i);
            i--;
        }else{
            break;
        }
    }
}

void interactEvent(int index){
    string cmd;
    cout << "\n\n-------------------------------------------\n\n";
    cout << "To enter previous page, enter return\n";
    cout << "To delete event, enter del\n";
    cout << "To edit event, enter edit\n";
    cout << "Your input : ";
    getline(cin,cmd);    
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    if(cmd == "RETURN"){
        return;
    }else if(cmd == "DEL" || cmd == "DELETE"){
        events.erase(events.begin()+index);
        cout << "Event " << index+1 << " deleted\n";
    }else if(cmd == "EDIT"){
        while(true){
            string ele;
            cout << "To return back into previous page, enter return\n";
            cout << "Which element do you want to edit?\n";
            cout << "For starting time, enter start\n";
            cout << "For ending time, enter end\n";
            cout << "For date, enter date\n";
            cout << "For month, enter mon\n";
            cout << "For year, enter year\n";
            cout << "For event name, enter name\n";
            cout << "For event author, enter auth\n";
            cout << "For event location, enter loc\n";
            cout << "Your input : ";
            getline(cin,ele);
            transform(ele.begin(), ele.end(), ele.begin(), ::toupper);
            if(ele == "RETURN"){
                break;
            }else if(ele == "START"){
                int hr,min;
                tm temp = *localtime(&events[index].begin);
                cout << "Enter hour : ";
                cin >> hr;
                cout << "Enter minute : ";
                cin >> min;
                cin.ignore();
                temp.tm_hour = hr;
                temp.tm_min = min;
                events[index].begin = mktime(&temp);
            }else if(ele == "END"){
                int hr,min;
                tm temp = *localtime(&events[index].end);
                cout << "Enter hour : ";
                cin >> hr;
                cout << "Enter minute : ";
                cin >> min;
                cin.ignore();
                temp.tm_hour = hr;
                temp.tm_min = min;
                events[index].end = mktime(&temp);
            }else if(ele == "DAT" || ele == "DATE"){
                int date;
                tm temp = *localtime(&events[index].begin);
                cout << "Enter date : ";
                cin >> date;
                cin.ignore();
                temp.tm_mday = date;
                events[index].begin = mktime(&temp);
            }else if(ele == "MON" || ele == "MONTH"){
                int mon;
                tm temp = *localtime(&events[index].begin);
                cout << "Enter month : ";
                cin >> mon;
                cin.ignore();
                temp.tm_mon = mon;
                events[index].begin = mktime(&temp);
            }else if(ele == "YEAR"){
                int year;
                tm temp = *localtime(&events[index].begin);
                cout << "Enter year : ";
                cin >> year;
                cin.ignore();
                temp.tm_year = year-1900;
                events[index].begin = mktime(&temp);
            }else{
            cout << "\nUnknown command, please enter again : ";
            }
        printEvent(events, index);
        }
        cout << "Event " << index+1 << "edited\n";
    }else{
        cout << "\nUnknown command, please enter again : ";
    }
}

void eventsPage(){
    string cmd;
    tm temp[2];
    system("CLS");
    for(unsigned int i = 0; i < events.size(); i++){
        temp[0] = *localtime(&events[i].begin);
        temp[1] = *localtime(&events[i].end);
        cout << "[" << i+1 << "] : ";
        if(temp[0].tm_mday == temp[1].tm_mday && temp[0].tm_mon == temp[1].tm_mon && temp[0].tm_year == temp[1].tm_year){
            cout << "From    " << temp[0].tm_hour << '.' << temp[0].tm_min;
            cout << "    to    " << temp[1].tm_hour << '.' << temp[1].tm_min;
            cout << "   on   " << temp[0].tm_mday << '/' << temp[0].tm_mon << '/' << temp[0].tm_year+1900;
            cout << "    by" << left << setw(30) << events[i].name << "at" << setw(30) << events[i].location << '\n';
        }else{
            cout << "From    " << temp[0].tm_hour << '.' << temp[0].tm_min;
            cout << "   on   " << temp[0].tm_mday << '/' << temp[0].tm_mon << '/' << temp[0].tm_year+1900;
            cout << "    to    " << temp[1].tm_hour << '.' << temp[1].tm_min;
            cout << "   on   " << temp[1].tm_mday << '/' << temp[1].tm_mon << '/' << temp[1].tm_year+1900;
            cout << "    by" << left << setw(30) << events[i].name << "at" << setw(30) << events[i].location << '\n';
        }
    }
    cout << "\n\n-------------------------------------------\n\n";
    cout << "To enter previous page, enter return\n";
    cout << "To add a new event, enter new\n";
    cout << "To add a delete all passed events, enter pass\n";
    cout << "To select an event, enter its index number\n";
    cout << "Your input : ";

    while(true){
        getline(cin,cmd);
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        if(cmd == "RETURN"){
            return;
        }else if(cmd == "NEW"){
            addEvent();
            system("CLS");
        }else if(cmd == "Passed"){
            //passEvent();
            system("CLS");
        }else if(atoi(cmd.c_str()) > 0 && atoi(cmd.c_str()) <= (int)events.size()){
            interactEvent(atoi(cmd.c_str()) - 1);
            system("CLS");
        }else{
            cout << "\nUnknown command, please enter again : ";
        }
    }

}

void recursPage(){

}

void mainMenu(){
    string cmd;
    cout << "To view events, enter 1\n";
    cout << "To view recursive events, event 2\n";
    cout << "To exit, enter exit\n";
    cout << "Your input : ";

    while(true){
        getline(cin, cmd);
        transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
        if(cmd == "1"){
            eventsPage();
            system("CLS");
        }else if(cmd == "2"){
            recursPage();
            system("CLS");            
        }else if(cmd == "EXIT"){
            return;
        }else{
            cout << "\nUnknown command, please enter again : ";
        }
    }
}

void voteEvent(unsigned int index){

}


int main(){
    /*
    vector<Recur> Abe;
    Recur temp;
    temp.type = 'w';
    temp.begin = time(0);
    temp.end = time(0);
    temp.name = "Study Physics";
    temp.author = "Thanakorn";
    temp.location = "-";

    Abe.push_back(temp);
    cout << (Abe[0]).type << '\n';

    cout << sizeof(tm);
    */

    events = importEvent();
    sortEvent(events);
    recurs = importRecur();
    mainMenu();

    return 0;
}
