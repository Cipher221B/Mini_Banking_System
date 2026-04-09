#pragma once

#include <vector>
#include <windows.h>
#include <sqlext.h>
#include <string>
#include "sql_error.h"
#include "manager_user.h"
#include "db_connection.h"
#include "session.h"

class User_Repository
{
    private: 
        SQLHDBC hdbc;

    public:
        enum class Repository_Result
        {
            NONE,
            SUCCESS,
            FAILED,
            DUPLICATE_PHONE_NO, 
            PHONE_NUMBER_DOES_NOT_EXIST
        };
        
        User_Repository(SQLHDBC conn);
        
        //api login
        Repository_Result get_user_information(User& u, Session& s, string& phone_number);
        void get_user_role(User& u);
        void get_session_version(User& u, Session& s);
        void get_salt();

        //api registry
        Repository_Result add_new_user(User& u);
        // void create_password(User& u);
        void set_role(User& u);

};