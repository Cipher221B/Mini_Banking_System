#pragma once

#include <iostream>
using namespace std;

struct Error_Info
{
    string sql_state;
    int native_err;
    string message_err;
        
};