#pragma once
#include <vector>
#include "data/category_log.h"
#include "data/category_batch.h"
using namespace std;

namespace TelemetryExport
{
    void export_log(AuthBatch& auth_batch);
    void export_log(ActivityBatch& activity_batch);
    void export_log(TransactionBatch& transaction_batch);
}