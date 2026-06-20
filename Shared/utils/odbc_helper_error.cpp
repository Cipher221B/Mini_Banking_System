#include "odbc_helper_error.h"
using namespace std;

Error_Info Helper_Error::get_infor_error(SQLSMALLINT type, SQLHANDLE handle) //pointer can getdata if parent process call and still true if don't
{
    SQLCHAR sql_state[7] = {0};
    SQLINTEGER native_err = 0;
    SQLCHAR message_err[256] = {0};
    SQLSMALLINT message_leng = 0;

    SQLRETURN res = SQLGetDiagRecA(type, handle, 1, sql_state, &native_err, message_err, sizeof(message_err), &message_leng);

    if(!SQL_SUCCEEDED(res))
    {
        return {"UNKNOWN", -1, "FAILED TO GET ERROR"};
    }
    sql_state[6] = '\0';
    message_err[message_leng] = '\0';
    return {string((char*) sql_state), native_err, string((char*) message_err)}; //SQLCHAR = Unsigned char but constructor of string only get char*

}