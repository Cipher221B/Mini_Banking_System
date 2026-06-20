#pragma once
#include <iostream>

struct Error_Info
{
    std::string sql_state;
    int native_err;
    std::string message_err;
        
};