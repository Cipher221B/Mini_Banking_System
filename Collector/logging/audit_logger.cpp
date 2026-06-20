#include "logging/audit_logger.h"
#include "Shared/error/system_errors.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void TelemetryExport::export_log(TransactionBatch& transaction_batch)
{
    ofstream write("../Banking_logs/transaction_log.json", ios::app);
    // cout << "on export log\n";
    if(write.is_open())
    {
        for(size_t i = 0; i < transaction_batch.list_transaction_log.size(); i++)
        {
            json j;
            j["category"] = transaction_batch.list_transaction_log[i].category;
            j["event_time"] = transaction_batch.list_transaction_log[i].timestamp;
            j["transaction_code"] = transaction_batch.list_transaction_log[i].transaction_code;
            j["type_transaction"] = transaction_batch.list_transaction_log[i].type;
            j["status"] = transaction_batch.list_transaction_log[i].status;
            j["amount"] = transaction_batch.list_transaction_log[i].amount;
            j["from_accountid"] = transaction_batch.list_transaction_log[i].from_accountid;
            j["to_accountid"] = transaction_batch.list_transaction_log[i].to_accountid;

            write << j.dump() << endl;
            
        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'Banking_logs/authentication_log.json'. Log Failed!!!\n"; 
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore(); 
        cin.get();    
        write.close();
        throw Open_File_Error("Can't export log to this file: 'Banking_logs/authentication_log.json'.", __FILE__, __LINE__);
    }
    write.close();
}

void TelemetryExport::export_log(ActivityBatch& activity_batch)
{
    ofstream write("../Banking_logs/activity_log.json", ios::app);
    // cout << "on export log\n";
    if(write.is_open())
    {
        for(size_t i = 0; i < activity_batch.list_activity_log.size(); i++)
        {
            json j;
            j["category"] = activity_batch.list_activity_log[i].category;
            j["event_time"] = activity_batch.list_activity_log[i].timestamp;
            j["type_activity"] = activity_batch.list_activity_log[i].type;
            j["status"] = activity_batch.list_activity_log[i].status;
            j["detail"] = activity_batch.list_activity_log[i].detail;
            j["userid"] =  activity_batch.list_activity_log[i].userid;
            j["target_userid"] = activity_batch.list_activity_log[i].target_userid;
            j["target_accountid"] = activity_batch.list_activity_log[i].target_accountid;

            write << j.dump() << endl;

        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'Banking_logs/authentication_log.json'. Log Failed!!!\n"; 
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore(); 
        cin.get();    
        write.close();
        throw Open_File_Error("Can't export log to this file: 'Banking_logs/authentication_log.json'.", __FILE__, __LINE__);
    }
    write.close();
}

void TelemetryExport::export_log(AuthBatch& auth_batch)
{
    ofstream write("../Banking_logs/authentication_log.json", ios::app);
    if(write.is_open())
    {
        for(size_t i = 0; i < auth_batch.list_auth_log.size(); i++)
        {
            json j;
            j["category"] = auth_batch.list_auth_log[i].category;
            j["event_time"] = auth_batch.list_auth_log[i].timestamp;
            j["action"] = auth_batch.list_auth_log[i].action;
            j["status"] = auth_batch.list_auth_log[i].status;
            j["detail"] = auth_batch.list_auth_log[i].detail;
            j["userid"] =  auth_batch.list_auth_log[i].userid;
            j["target_accountid"] = auth_batch.list_auth_log[i].target_accountid;
            j["ip_address"] = auth_batch.list_auth_log[i].ip_address;
            write << j.dump() << endl;
        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "'Banking_logs/authentication_log.json'. Log Failed!!!\n"; 
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore(); 
        cin.get();    
        write.close();
        throw Open_File_Error("Can't export log to this file: 'Banking_logs/authentication_log.json'.", __FILE__, __LINE__);
    }
    write.close();
}
