#pragma once
#include <windows.h>
#include <sqlext.h>
#include <vector>
#include "data/category_log.h"
#include "data/category_batch.h"
using namespace std;

class Log_Repository
{
    private:
        SQLHDBC hdbc;
    
    public:
        Log_Repository(SQLHDBC conn);
        int get_check_point_id(int category_id);
        void update_checkpoint(int category_id, int new_checkpoint);
        AuthBatch load_auth_batch_log(int check_point);
        ActivityBatch load_activity_batch_log(int check_point);
        TransactionBatch load_transaction_batch_log(int check_point); 
};