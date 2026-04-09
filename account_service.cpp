#include "account_service.h"
#include "db_connection.h"
#include "user_repository.h"
#include "account_repository.h"
#include "database_errors.h"
#include "audit_log.h"
using namespace std;

AccountService::TRANSFER AccountService::create_transaction(User& u, Account& a, string& dest_account, int amount)
{
    DataBase conn; //Data Base Connection Error
    User_Repository ur(conn.get_hdbc());
    Account_Repository ar(conn.get_hdbc());

    //how to transfer 1 transaction
    try
    {   
        Account_Repository::Repository_Result repr = ar.transaction(a, dest_account, amount);

        if(repr == Account_Repository::Repository_Result::SENDER_NOT_EXIST)
        {
            return TRANSFER::USER_NOT_EXIST;
        }
        else if(repr == Account_Repository::Repository_Result::RECEIVER_NOT_EXIST)
        {
            return TRANSFER::DESTINATION_USER_NOT_EXIST;
        }
        else if(repr == Account_Repository::Repository_Result::SENDER_NOT_AVAILABLE)
        {
            return TRANSFER::USER_NOT_AVAILABLE;
        }
        else if(repr == Account_Repository::Repository_Result::RECEIVER_NOT_AVAILABLE)
        {
            return TRANSFER::DESTINATION_USER_NOT_AVAILABLE;
        }
        else if(repr == Account_Repository::Repository_Result::INSUFFICIENT_BALANCE)
        {
            return TRANSFER::INSUFFICIENT_BALANCE;
        }

        Audit_Log::transaction(conn.get_hdbc(), a, dest_account, "SUCCESS", amount);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    }
    catch(const Transaction_Error& e)
    {
        System_Log::error_log("Transaction Failed", e.filename_err, e.state, e.native_err, e.what());
        return TRANSFER::FAILED;
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", e.filename_err, e.state, e.native_err, e.what());
        return TRANSFER::FAILED; // !!!return sai
    }

    return TRANSFER::SUCCESS;
    
}