#include "db_connection.h"
#include <iostream>
#include <stdexcept>
#include "Shared/error/database_errors.h"
#include "Shared/utils/db_helper.h"
#include "Shared/utils/odbc_helper_error.h"
using namespace std;

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

                SQLRETURN res = SQLDriverConnectA(hdbc, NULL, connloc, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

                if(!SQL_SUCCEEDED(res))
                {
                    Error_Info last_err = Helper_Error::get_infor_error(SQL_HANDLE_DBC, hdbc);
                    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
                    throw ConnectionErrors(last_err.message_err, __FILE__, __LINE__, last_err.sql_state, last_err.native_err);
                }

            }
            else
            {
                Error_Info last_err = Helper_Error::get_infor_error(SQL_HANDLE_ENV, henv);
                SQLFreeHandle(SQL_HANDLE_ENV, henv);
                throw ConnectionErrors(last_err.message_err, __FILE__, __LINE__, last_err.sql_state, last_err.native_err);
            }

        }  
        else
        {
            Error_Info last_err = Helper_Error::get_infor_error(SQL_HANDLE_ENV, henv);
            SQLFreeHandle(SQL_HANDLE_ENV, henv);
            throw ConnectionErrors(last_err.message_err, __FILE__, __LINE__, last_err.sql_state, last_err.native_err);
        }

    }
    else
    {
        Error_Info last_err = Helper_Error::get_infor_error(SQL_HANDLE_ENV, henv);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw ConnectionErrors(last_err.message_err, __FILE__, __LINE__, last_err.sql_state, last_err.native_err);
    }
    
}

// Close Connect
DataBase::~DataBase()
{
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
