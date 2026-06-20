#pragma once
#include <atomic>
using namespace std;

class SignalManager
{
    private:
        static atomic<bool> running;
    
    public:
        static void signal_handle(int signal);
        static void register_signal();
        static bool is_running();

};