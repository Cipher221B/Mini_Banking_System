#pragma once
#include <windows.h>
#include <sqlext.h>
#include "db_connection.h" 

class Helper_Error
{
    public:
        static Error_Info get_infor_error(SQLSMALLINT type, SQLHANDLE handle);
};

