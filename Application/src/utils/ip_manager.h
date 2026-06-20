#pragma once
#include <iostream>
#include <unordered_map>
#include "Shared/error/system_errors.h"

namespace IP_Manager
{
    void ip_registration(const std::string& phone_number, const std::string& ip_address);
    std::string get_ip_from_file(const std::string& phone_number);
    std::unordered_map<std::string, std::string> get_list_ip_exist();
};