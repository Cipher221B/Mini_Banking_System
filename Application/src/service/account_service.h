#pragma once
#include "entity/account.h"
#include "entity/user.h"
#include "entity/session.h"
#include "Shared/database/db_connection.h"
#include "data/transaction_information.h"
#include "logging/audit_log.h"
#include "error/layer_error.h"
#include "Shared/error/system_errors.h"
#include "Shared/utils/db_helper.h"


struct LOAD_DATA
{
    Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
    Service_Error::BUSSINESS_ERROR b = Service_Error::BUSSINESS_ERROR::NONE;
    Service_Error::VALIDATE_ERROR v = Service_Error::VALIDATE_ERROR::NONE;
};


struct CHANGE_STATUS
{
    Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
    Service_Error::VALIDATE_ERROR v = Service_Error::VALIDATE_ERROR::NONE;
};

struct TRANSFER
{
    Service_Error::SYSTEM_ERROR s = Service_Error::SYSTEM_ERROR::NONE;
    Service_Error::BUSSINESS_ERROR b = Service_Error::BUSSINESS_ERROR::NONE;
    Service_Error::VALIDATE_ERROR v = Service_Error::VALIDATE_ERROR::NONE;
};



class AccountService
{
    public:
        LOAD_DATA refresh(Session& s, Account& a);
        LOAD_DATA get_account_status_info(Status_Account_Info& sai);
        LOAD_DATA get_user_status_info(Status_User_Info& sui);
        
};


class AdminService
{
    public:
        CHANGE_STATUS change_status_account(Session& s, User& u, Account& a, Status_Account_Info& sai, std::string& password);
        CHANGE_STATUS change_status_user(Session& s, User& u, Account& a, Status_User_Info& sui, std::string& password);
};

class UserService
{
    public:
        TRANSFER create_transaction(Session& s, User& u, Account& a, Transaction_Info& t, std::string& p);
};