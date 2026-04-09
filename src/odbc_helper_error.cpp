#include "odbc_helper_error.h"

Error_Info Helper_Error::get_infor_error(SQLSMALLINT type, SQLHANDLE handle) //pointer can getdata if parent process call and still true if don't
{
    SQLCHAR sql_state[6];
    SQLINTEGER native_err;
    SQLCHAR message_err[256];
    SQLSMALLINT message_leng;

    SQLGetDiagRec(type, handle, 1, sql_state, &native_err, message_err, sizeof(message_err), &message_leng);

    return {(char*) sql_state, native_err, (char*) message_err}; //SQLCHAR = Unsigned char but constructor of string only get char*

}