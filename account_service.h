#pragma once

class AccountService
{
    public:
        enum class TRANSFER
        {
            SUCCESS,
            NOT_PERMISSION,
            SENDER_NOT_AVAILABLE,
            RECEIVER_NOT_AVAILABLE,
            SENDER_NOT_EXIST,
            RECEIVER_NOT_EXIST,
            INSUFFICIENT_BALANCE,
            FAILED
        };
        TRANSFER create_transaction(User& u, Account& a, string& dest_account, int amount);

};