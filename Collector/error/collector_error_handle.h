#pragma once
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include "Shared/utils/odbc_helper_error.h"

namespace Collector_Handle
{
    //handle error api
    template<typename Exception_Type>
    static void handle_error(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free, const std::string& file_name, int line)
    {
        Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
        SQLFreeHandle(type_free, handle_free);
        throw Exception_Type(last_err.message_err, file_name, line, last_err.sql_state, last_err.native_err);
    }

    template<typename Exception_Type>
    static void handle_error(SQLSMALLINT type, SQLHANDLE handle, const std::string& file_name, int line)
    {
        Error_Info last_err = Helper_Error::get_infor_error(type, handle);
        if(last_err.native_err == 2627 || last_err.native_err == 2601) //DUPLICATE CASE
        {
            SQLFreeHandle(type, handle);
        }
        else
        {
            SQLFreeHandle(type, handle);
            throw Exception_Type(last_err.message_err, file_name, line, last_err.sql_state, last_err.native_err);
        }
    }
}