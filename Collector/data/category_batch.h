#pragma once
#include <vector>
#include "category_log.h"
using namespace std;

struct AuthBatch
{
    int last_id_load;
    vector<AuthLog> list_auth_log;
};

struct ActivityBatch
{
    int last_id_load;
    vector<ActivityLog> list_activity_log;
};

struct TransactionBatch
{
    int last_id_load;
    vector<TransactionLog> list_transaction_log;
};