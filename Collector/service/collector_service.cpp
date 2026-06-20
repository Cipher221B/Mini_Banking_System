#include "collector_service.h"
#include <iostream>
#include <vector>
#include "Shared/database/db_connection.h"
#include "repository/log_repository.h"
#include "data/category_log.h"
#include "Shared/error/database_errors.h"
#include "Shared/error/system_errors.h"
#include "Shared/logging/system_log.h"
#include "logging/audit_logger.h"
#include "data/category_batch.h"
using namespace std;

void CollectorService::run_service()
{
    try
    {
        DataBase conn;
        Log_Repository lr(conn.get_hdbc());

        int auth_check_point = lr.get_check_point_id(1);
        int activity_check_point = lr.get_check_point_id(2);
        int transaction_check_point = lr.get_check_point_id(3);

        //load batch
        AuthBatch auth_batch = lr.load_auth_batch_log(auth_check_point);
        ActivityBatch activity_batch = lr.load_activity_batch_log(activity_check_point);
        TransactionBatch transaction_batch = lr.load_transaction_batch_log(transaction_check_point);

        //export log && update check point 
        if(auth_batch.last_id_load > auth_check_point && !auth_batch.list_auth_log.empty())
        {
            TelemetryExport::export_log(auth_batch);
            lr.update_checkpoint(1, auth_batch.last_id_load);
        }

        if(activity_batch.last_id_load > activity_check_point && !activity_batch.list_activity_log.empty())
        {
            TelemetryExport::export_log(activity_batch);
            lr.update_checkpoint(2, activity_batch.last_id_load);
        }

        if(transaction_batch.last_id_load > transaction_check_point && !transaction_batch.list_transaction_log.empty())
        {
            TelemetryExport::export_log(transaction_batch);
            lr.update_checkpoint(3, transaction_batch.last_id_load);
        }

    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "collector.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }
    catch(const Data_Access_Error& e)
    {
        System_Log::error_log("Collector Access Log Data Failed", "collector.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }
    catch(const Query_Error& e)
    {
        System_Log::error_log("Update CheckPoint Failed", "collector.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }
    catch(const Open_File_Error& e)
    {
        System_Log::error_log("Export Log Failed", "collector.log", e.filename_err, e.line_err, e.what());
    }
}