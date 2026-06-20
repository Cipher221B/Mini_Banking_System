#pragma once
#include "Shared/database/db_connection.h"
#include "repository/user_repository.h"
#include "repository/account_repository.h"
#include "entity/user.h"
#include "entity/account.h"
#include "error/layer_error.h"

struct LOGOUT
{
    Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
};

struct CHANGE_PASSWORD
{
     public:
        Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
        Service_Error::VALIDATE_ERROR v = Service_Error::VALIDATE_ERROR::NONE;
};

struct LOGIN
{
    public:
        Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
        Service_Error::BUSSINESS_ERROR b = Service_Error::BUSSINESS_ERROR::NONE;
        Service_Error::VALIDATE_ERROR v = Service_Error::VALIDATE_ERROR::NONE;
};

struct REGISTER
{
    public:
        Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
        Service_Error::BUSSINESS_ERROR b = Service_Error::BUSSINESS_ERROR::NONE;
};



class AuthService
{
    private:
        //auth logic check
        bool check_status_user(User& u);
        bool check_status_account(Account& a);
        bool verify(User& u, std::string& p);
        
        //handle get data
        Service_Error::BUSSINESS_ERROR load_authenticated_user(User& u, Session& s, User_Repository& ur, std::string& phone_number);

    public:

        //authentication api
        Service_Error::VALIDATE_ERROR action_verification_on_user(Session& s, User_Repository& ur, User& u, std::string& password);
        Service_Error::VALIDATE_ERROR action_verification_on_account(Session& s, Account_Repository& ar, User_Repository& ur, Account& a, User& u, std::string& password);
        Service_Error::VALIDATE_ERROR login_verification(User& u, std::string& password);

        bool check_session_version(Session& s, int current_session_on_db);

        //logout
        LOGOUT logout(User& u, Account& a, bool enforced_logout);

        //change password
        CHANGE_PASSWORD change_password(Session& s, User& u, std::string& current_p, std::string& new_p);
        
        //login api
        LOGIN login(User& u, Account& a, Session& s, std::string& phone_number, std::string& password, bool data_user_in_memory);
        
        //registry api
        REGISTER registry(User& u, Account& a);

};

