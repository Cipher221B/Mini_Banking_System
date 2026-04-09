#include "system_log.h"
#include <sstream>
#include <fstream>
#include <chrono>
#include <iomanip>

string System_Log::get_real_time()
{
    auto now = chrono::system_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch())%1000;
    

    time_t time = chrono::system_clock::to_time_t(now);

    tm* ptm = localtime(&time);

    stringstream rt;
    rt << setfill('0') << ((ptm->tm_year) + 1900) << "-" << setw(2) << ((ptm->tm_mon) + 1) << "-" << setw(2) << ptm->tm_mday << " " << setw(2) 
    << ptm->tm_hour << ":" << setw(2) << ptm->tm_min << ":" << setw(2) << ptm->tm_sec << "." << setw(3) << ms.count(); // setfill to padding.

    return rt.str();

}

void System_Log::error_log(const string& type_err, const string& namefile_err, const string& state, int native, const string& message)
{
    ofstream write("system.log", ios::app);

    if(write.is_open())
    {
        write << get_real_time() << " [" << namefile_err << "] " << type_err << " state=" << state << " native=" << native << " message='" << message << endl;
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'system.log'. Log Failed!!!\n"; 
    }
    write.close();

}

void System_Log::error_log(const string& type_err, const string& namefile_err, const string& message)
{
    ofstream write("system.log", ios::app);

    if(write.is_open())
    {
        write << get_real_time() << " [" << namefile_err << "] " << type_err << " message='" << message << "'." << endl;
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'system.log'. Log Failed!!!\n"; 
    }
    write.close();

}