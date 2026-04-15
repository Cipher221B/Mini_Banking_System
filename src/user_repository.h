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
        
        //api refresh
        Repository_Result get_user_data(User& u, Session& s);

        //api login
        Repository_Result get_user_data_to_authentication(User& u, Session& s, string& phone_number);
        void get_user_role(User& u);
        int get_version_db(Session& s);
        void get_salt();

        //api registry
        Repository_Result add_new_user(User& u);
        // void create_password(User& u);
        void set_role(const User& u);

};