#pragma once

#include "db_connection.h"
#include "user_repository.h"
#include "account_repository.h"
#include "manager_user.h"
#include "manager_account.h"

class AuthService
{
    public:
        enum class REGISTER
        {
            E_DUPLICATE_PHONE_NO,
            CONNECTION_FAILED,
            DATABASE_ERROR,
            FETCH_FAILED,
            HASH_FAILED,
            CREATE_USER_FAILED,
            CREATE_ACCOUNT_FAILED,
            SUCCESS
        };

        enum class LOGIN
        {   
            CONNECTION_FAILED,
            USER_DOES_NOT_EXIST,
            ACCOUNT_NOT_FOUND,
            DATABASE_ERROR,
            VERIFY_FAILED,
            USER_NOT_AVAILABLE,
            HASH_FAILED,
            SUCCESS,
            FAILED

        };

        enum class GET_USER
        {
            SUCCESS,
            PHONE_NUMBER_DOES_NOT_EXIST
        };

        enum class GET_ACCOUNT
        {
            SUCCESS,
            ACCOUNT_NOT_FOUND
        };
        
        enum class VERIFY
        {
            SUCCESS,
            WRONG_PASSWORD, 
            USER_NOT_AVAILABLE

        };



        //transfer api
        bool check_permission_dest_account_before_create_transaction(User& u);

        //login api
        LOGIN login(User& u, Account& a, Session& s, string& phone_number, string& password, bool data_user_in_memory);
        GET_USER get_user_information(User& u, Session& s, User_Repository& ur, string& phone_number);
        GET_ACCOUNT get_account_information(Account& a, Account_Repository& ar);
        VERIFY authentication(User& u, Account& a, string& password);

        //registry api
        REGISTER registry(User& u, Account& a);

        //void handle_error(Database& d);
        void handle_duplicate_account_no(DataBase& conn, Account& a, Account_Repository& ar, Account_Repository::Repository_Result repr);
        void check_error(DataBase& conn, Account& a, Account_Repository& ar, Account_Repository::Repository_Result repr);
};