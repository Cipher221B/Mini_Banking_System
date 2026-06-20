#include <iostream>
#include <thread>
#include <windows.h>
#include "signal/signal_manager.h"
#include "service/collector_service.h"
using namespace std;

int main()
{
    SignalManager::register_signal();
    CollectorService cs;
    cout << "Collector is running..." << endl;
    while(SignalManager::is_running())
    {
        cs.run_service();
        this_thread::sleep_for(chrono::seconds(10));
    }
    cout << "Collector is stopped!" << endl;
    return 0;
}


