#pragma once
#include <iostream>
#include <optional>
using namespace std;

struct AuthLog
{
    public:
        string category;
        string timestamp;
        string action;
        string status;
        string detail;
        optional<int> userid;
        optional<int> target_accountid;
        string ip_address;
    
        AuthLog() : category("Authentication"), timestamp(""), action(""), status(""), detail(""), userid(nullopt), target_accountid(nullopt), ip_address(""){};
};

struct ActivityLog
{
    public:
        string category;
        string timestamp;
        string type;
        string status;
        string detail;
        int userid;
        optional<int> target_userid;
        optional<int> target_accountid;

        ActivityLog() : category("Activity"), timestamp(""), type(""), status(""), detail(""), userid(0), target_userid(nullopt), target_accountid(nullopt){};
};

struct TransactionLog
{
    public:
        string category;
        string transaction_code;
        string timestamp;
        string type;
        int from_accountid;
        int to_accountid;
        double amount;
        string status;

        TransactionLog() : category("Transaction"), transaction_code(""), timestamp(""), type(""), from_accountid(0), to_accountid(0), amount(0), status(""){};
};