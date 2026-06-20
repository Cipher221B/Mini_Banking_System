#include "random_generation.h"
#include <random>
#include <chrono>
#include <unordered_map>
#include "utils/string_utils.h"
#include "utils/ip_manager.h"
using namespace std;

string Random_Generation::create_sequence_ms(Account_Repository& ar)
{
    //dùng db để get sequence và tiến hành viết hàm tạo account no và transaction code
    int sequence_number = ar.get_sequence_account_no();
    return String_Utils::standardization_number(sequence_number, 8, '0');
    
}

string Random_Generation::get_unix_timestamp() // 13 digit
{
    auto now = chrono::system_clock::now();
    int64_t ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    return to_string(ms);

}

int Random_Generation::generate_number(int start, int end)
{
    string random_number; 
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution <> dis(start, end); //7

    return dis(gen);
    
}

string Random_Generation::create_check_digit(const string& sn)
{
    int sum = 0;
    for(size_t i = 3; i < sn.size(); i++)
    {
        sum += sn[i] - '0'; // 1 char với index trong ASCII - '0' (tức 48) mới ra số thực của nó.
    }

    sum = sum % 9;
    return to_string(sum);

}

//20 digit
string Random_Generation::generate_account_no(Account_Repository& ar) //self-allocation
{
    string temp_acc_no = "920";

    temp_acc_no += create_sequence_ms(ar); //8

    temp_acc_no += create_check_digit(temp_acc_no); //1
    return temp_acc_no;

}

//check -> continue từ đây
int Random_Generation::last_digit(const string& sn)
{
    int sum = 0;

    for(size_t i = 3; i < (sn.size() - 1) ; i++)
    {
        sum += sn[i] - '0'; 

    }

    sum = sum % 9;
    return sum;
}

string Random_Generation::generate_transaction_code()
{
    string temp_tran_code = "MB";

    temp_tran_code += get_unix_timestamp(); //13

    temp_tran_code += String_Utils::standardization_number(generate_number(0, 9999999), 7, '0'); //7

    return temp_tran_code;
}

string Random_Generation::generate_ip()
{
    vector<string> pool_ip = {"203.0.113.", "198.51.100.", "192.0.2.", "172.20.10.", "100.64.0."}; //45.133.192. (attacker ip)
    unordered_map<string, string> list_ip_profile_exist = IP_Manager::get_list_ip_exist();
    string ip = "";
    do
    {
        ip = pool_ip[generate_number(0, 4)] + String_Utils::standardization_number(generate_number(2, 254), 3, '0');
    }while(list_ip_profile_exist.count(ip));
    return ip;
}