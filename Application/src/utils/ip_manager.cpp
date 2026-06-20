#include "ip_manager.h"
#include <fstream>
#include <sstream>
using namespace std;

void IP_Manager::ip_registration(const std::string& phone_number, const std::string& ip_address)
{
    ofstream write("ip_profile.txt", ios::app);
    if(write.is_open())
    {
        write << phone_number << ":" << ip_address << endl;
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'ip_profile.txt'. Log Failed!!!\n"; 
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore(); 
        cin.get();    
        write.close();
        throw Open_File_Error("Can't registration ip to this file: 'ip_profile.txt'.", __FILE__, __LINE__);
    }
    write.close();
}

string IP_Manager::get_ip_from_file(const string& phone_number)
{
    string temp_line;
    string ip = "";
    ifstream read("ip_profile.txt");

    if(read.is_open())
    {
        while(getline(read, temp_line))
        {
            stringstream ss(temp_line);
            string f_phone_number;
            getline(ss, f_phone_number, ':');
            if(f_phone_number == phone_number)
            {
                getline(ss, ip);
                break;
            }
        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: 'ip_profile.txt'. Read File Failed!!!\n";
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore();  
        cin.get();   
        read.close();
        throw Open_File_Error("Can't get ip from file: 'ip_profile.txt'.", __FILE__, __LINE__);
        
    }
    read.close();
    return ip;
}

unordered_map<string, string> IP_Manager::get_list_ip_exist()
{
    unordered_map<string, string> list_ip_profile;
    string temp_line, phone_no;
    ifstream read("ip_profile.txt");

    if(read.is_open())
    {
        while(getline(read, temp_line))
        {
            stringstream ss(temp_line);
            getline(ss, phone_no, ':');
            string ip;
            getline(ss, ip);
            list_ip_profile[phone_no] = ip;
        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: 'ip_profile.txt'. Read File Failed!!!\n";
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore();  
        cin.get();     
        read.close();
        throw Open_File_Error("Can't get list ip from file: 'ip_profile.txt'.", __FILE__, __LINE__);
    }
    read.close();
    return list_ip_profile;

}