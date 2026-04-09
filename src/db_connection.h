#pragma once
#include <windows.h>
#include <sqlext.h>
#include "sql_error.h"

class DataBase
{
    private:
        SQLHENV henv = SQL_NULL_HENV; //environment variable
        SQLHDBC hdbc = SQL_NULL_HDBC; //connection handle environment
        
        Error_Info last_err;

    public:
      
        DataBase();
        void handle_error(SQLSMALLINT type, SQLHANDLE handle);
        void set_off_auto_commit();
        SQLHDBC get_hdbc(){return hdbc;}
        ~DataBase();

};