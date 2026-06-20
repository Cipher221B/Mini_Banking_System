#pragma once

namespace Service_Error
{
    enum class SYSTEM_ERROR
    {
        NONE,
        CONNECTION_FAILED,
        DATABASE_ERROR,
        CREATE_USER_FAILED,
        CREATE_ACCOUNT_FAILED,
        CHANGE_STATUS_USER_FAILED,
        CHANGE_STATUS_ACCOUNT_FAILED,
        CHANGE_PASSWORD_FAILED,
        FETCH_FAILED,
        HASH_FAILED,
    };
    
    enum class BUSSINESS_ERROR
    {
        NONE,
        E_DUPLICATE_ACCOUNT_NO,
        E_DUPLICATE_TRANSACTION_NO,
        E_DUPLICATE_PHONE_NO,
        USER_NOT_EXIST,
        ACCOUNT_NOT_FOUND,
        RECEIVER_NOT_EXIST,
        SAME_ACCOUNT_TRANSFER,
        INSUFFICIENT_BALANCE
    };

    enum class VALIDATE_ERROR
    {
        NONE,
        VERIFY_FAILED,
        USER_NOT_AVAILABLE,
        ACCOUNT_NOT_AVAILABLE,
        RECEIVER_NOT_AVAILABLE,
        SESSION_VERSION_EXPIRED,
    };

}

namespace Repository_Error
{
    enum class BUSSINESS_ERROR
    {
        NONE,
        DUPLICATE_ACCOUNT_NO, 
        ACCOUNT_NOT_FOUND, 
        DUPLICATE_PHONE_NO,
        DUPLICATE_TRANSACTION_NO, 
        PHONE_NUMBER_DOES_NOT_EXIST
    };
}