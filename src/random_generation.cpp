#include "random_generation.h"
#include <random>
#include <chrono>
#include "helper_support.h"
using namespace std;

string create_sequence_ms(Account& a, Account_Repository& ar)
{
    //dùng db để get sequence và tiến hành viết hàm tạo account no và transaction code
    int sequence_number = ar.get_sequence_account_no(a);
    return Support::standardization_number(sequence_number, 8, '0');
    
}

string get_unix_timestamp() // 13 digit
{
    auto now = chrono::system_clock::now();
    int ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    return to_string(ms);

}

string generate_number()
{
    string random_number; // 3 digit
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution <> dis(0, 9999999); //7

    return Support::standardization_number(dis(gen), 7, '0');
    
}

string create_check_digit(string& sn)
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
string create_account_no(Account& a, Account_Repository& ar) //self-allocation
{
    string temp_acc_no = "920";

    temp_acc_no += create_sequence_ms(a, ar); //8

    temp_acc_no += create_check_digit(a.account_no); //1

    return temp_acc_no;

}

//check -> continue từ đây
int last_digit(string& sn)
{
    int sum = 0;

    for(size_t i = 3; i < (sn.size() - 1) ; i++)
    {
        sum += sn[i] - '0'; 

    }

    sum = sum % 9;
    return sum;
}

string create_transaction_code()
{
    string temp_tran_code = "MB";

    temp_tran_code += get_unix_timestamp(); //13

    temp_tran_code += generate_number(); //7

    return temp_tran_code;
}