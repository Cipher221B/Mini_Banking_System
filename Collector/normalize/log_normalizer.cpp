#include "log_normalizer.h"
using namespace std;

string Log_Normalize::normalize_timestamp(string event_time)
{
    event_time[10] = 'T';
    event_time += "+07:00";
    return event_time;
}