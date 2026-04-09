#include "db_connection.h"
#include <iostream>
#include "database_errors.h"
#include <stdexcept>
#include "odbc_helper_error.h"
using namespace std;

void DataBase::handle_error(SQLSMALLINT type, SQLHANDLE handle)
{
    last_err = Helper_Error::get_infor_error(type, handle);
    throw ConnectionErrors(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

//Open Connect
DataBase::DataBase()
{
    SQLRETURN res;
    // Create Environment
    res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    
    if(SQL_SUCCEEDED(res))
    {
        //Declared version for handle ODBC
        res = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        if(SQL_SUCCEEDED(res))
        {
            //Create Connection
            res = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

            if(SQL_SUCCEEDED(res))
            {
                //Connection
                SQLCHAR connloc[]= 
                "Driver={ODBC Driver 17 for SQL Server};"
                "Server=localhost;"
                "Database=Test_System;"
                "Trusted_Connection=yes;"; //Information connection

                SQLRETURN res = SQLDriverConnect(hdbc, NULL, connloc, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

                if(!SQL_SUCCEEDED(res))
                {
                    handle_error(SQL_HANDLE_DBC, hdbc);
                }

            }
            else
            {
                handle_error(SQL_HANDLE_ENV, henv);

            }

        }  
        else
        {
            //Error Create environment
            handle_error(SQL_HANDLE_ENV, henv);
        }

    }
    else
    {
        //Error AllocHandle of henv
        handle_error(SQL_HANDLE_ENV, henv);
    }
    
}

void DataBase::set_off_auto_commit()
{
    SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
}

// Close Connect
DataBase::~DataBase()
{
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
