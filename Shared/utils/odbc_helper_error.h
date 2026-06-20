#pragma once
#include <windows.h>
#include <sqlext.h>
#include "Shared/data/sql_error.h"

namespace Helper_Error
{
    Error_Info get_infor_error(SQLSMALLINT type, SQLHANDLE handle);
}

