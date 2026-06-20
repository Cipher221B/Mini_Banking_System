#pragma once
#include <iostream>
#include "Shared/data/sql_error.h"
#include <vector>
#include <windows.h>
#include <sqlext.h>
#include "Shared/data/sql_error.h"
#include "entity/user.h"
#include "Shared/database/db_connection.h"
#include "entity/session.h"
#include "error/layer_error.h"

class User_Repository
{
    private: 
        SQLHDBC hdbc;

    public:
        User_Repository(SQLHDBC conn);
        
        //get data api
        Repository_Error::BUSSINESS_ERROR get_status_user_info(Status_User_Info& sui);
        Repository_Error::BUSSINESS_ERROR get_user_data_to_authentication(User& u, Session& s, std::string& phone_number);
        void get_authentication_data(User& u);
        int get_version_db(Session& s);

        //change status user api
        void change_status_user(Status_User_Info& sui);
        void update_session_version(int uid);

        //change password
        void change_credential(User& u, std::vector<unsigned char>& new_salt, std::vector<unsigned char>& new_credential);

        //api registry
        Repository_Error::BUSSINESS_ERROR add_new_user(User& u);

};