#include "signal_manager.h"
#include <iostream>
#include <csignal>

atomic<bool> SignalManager::running(true);

void SignalManager::signal_handle(int signal)
{
    cout << "\nSignal received: " << signal << "\nStopping collector...\n";
    running = false;
}

void SignalManager::register_signal()
{
    signal(SIGINT, signal_handle); //Ctrl + C
    signal(SIGTERM, signal_handle); //docker stop

}

bool SignalManager::is_running()
{
    return running.load();
}