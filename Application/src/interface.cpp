#include "interface.h"
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include "interface.h"
#include "security/random_generation.h"
#include "security/hash.h"
#include "utils/input_validation.h"
#include "utils/string_utils.h"
#include "Shared/error/system_errors.h"
#include "error/layer_error.h"
using namespace std;

//logout
CLI::State CLI::handle_logout(Session& s, User& u, Account& a, bool enforced_logout)
{
    AuthService as;
    LOGOUT l = as.logout(u, a, enforced_logout);

    if(l.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
    {
        show_failed_interface("LOG OUT", "Connection Failed!!!");
    }
    else if(l.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
    {
        show_failed_interface("LOG OUT", "DataBase Error!!!");
    }
    u.clear_all_user_data();
    a.clear_all_account_data();
    s.clear_session();
    return CLI::State::MAIN_MENU;
}

//refresh
CLI::State_Admin_DashBoard CLI::admin_refresh(LOAD_DATA ld)
{
    if(ld.s != Service_Error::SYSTEM_ERROR::NONE || ld.b != Service_Error::BUSSINESS_ERROR::NONE || ld.v != Service_Error::VALIDATE_ERROR::NONE)
    {
        if(ld.s != Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(ld.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
            {
                show_failed_interface("REFRESH FAILED", "Connnection Error!!! Please Contact with Admin to give a support!");
                return State_Admin_DashBoard::ADMIN_MENU;
            }
            else if(ld.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
            {
                show_failed_interface("REFRESH FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");
                return State_Admin_DashBoard::ADMIN_MENU;
            }

        }
        else if(ld.s == Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v != Service_Error::VALIDATE_ERROR::NONE)
        {
            show_failed_interface("REFRESH FAILED", "Session has expired! Try Again!!!");
            return State_Admin_DashBoard::ENFORCED_LOG_OUT;
        }
        else 
        {
            show_failed_interface("REFRESH FAILED", "Unknown Error!!!");
        }
    }
    return State_Admin_DashBoard::ADMIN_MENU;
}

CLI::State_Custom_DashBoard CLI::custom_refresh(LOAD_DATA ld)
{
    if(ld.s != Service_Error::SYSTEM_ERROR::NONE || ld.b != Service_Error::BUSSINESS_ERROR::NONE || ld.v != Service_Error::VALIDATE_ERROR::NONE)
    {
        if(ld.s != Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(ld.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
            {
                show_failed_interface("REFRESH FAILED", "Connnection Error!!! Please Contact with Admin to give a support!");
            }
            else if(ld.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
            {
                show_failed_interface("REFRESH FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");
            }

        }
        else if(ld.s == Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v != Service_Error::VALIDATE_ERROR::NONE)
        {
            show_failed_interface("REFRESH FAILED", "Session has expired! Try Again!!!");
            return State_Custom_DashBoard::ENFORCED_LOG_OUT;
        }
        else 
        {
            show_failed_interface("REFRESH FAILED", "Unknown Error!!!");
        }
    }
    return State_Custom_DashBoard::CUSTOM_MENU;
}


//transfer
void CLI::show_transfer_success_interface(Transaction_Info& t, User& u, Account& a)
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("TRANSACTION SUCCESSFULLY", WIDTH);
    cout << string(WIDTH, '=') << endl;
    cout << "Transaction ID: " << t.transaction_code << endl;
    cout << "From: " << u.get_full_name() << endl;
    cout << "Sender Account NO: " << a.get_account_no() << endl;
    cout << "To: " << t.full_name << endl;
    cout << "Receiver Account NO: " << t.dest_account_no << endl;
    cout << "Time: " << t.datetime << endl;
    cout << "Amount: " << t.amount << " VND" << endl;

    cout << string(WIDTH, '-') << endl;
    cout << " Press any key to continue...\n";

    cin.ignore(); 
    cin.get();     
}

void CLI::show_authenticate_before_sensitive_action(const string& center_message, string& p)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center(center_message, WIDTH);
    cout << string(WIDTH, '=') << endl;;
    p = user_input("Password : ");
}

void CLI::show_transfer_interface(Account& a, string& dest_account, string& p, double& amount)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("TRANSFER MONEY", WIDTH);
    cout << string(WIDTH, '=') << endl;
    cout << "Current Balance: " << fixed << setprecision(2) << a.get_balance() << "VND"<< "\n\n";
    dest_account = user_input("To account/card: ");
    cout << "Enter Amount to Transfer: ";
    cin >> amount;
    cin.ignore();
    
    string password;
    show_authenticate_before_sensitive_action("TRANSFER MONEY", p);

    cout << string(WIDTH, '-') << endl;
    cout << "Processing...\n"; 
   
}


CLI::State_Custom_DashBoard CLI::handle_transfer(Session& s, User& u, Account& a)
{
    string dest_account_no, password;
    double amount;
    show_transfer_interface(a, dest_account_no, password, amount);
    if(amount <= 0)
    {
        show_failed_interface("TRANSACTION FAILED", "Invalid Amount");
    }
    else
    {
        Transaction_Info tran_in(dest_account_no, amount);
        UserService us;
        TRANSFER t = us.create_transaction(s, u, a, tran_in, password);
        if(t.s == Service_Error::SYSTEM_ERROR::NONE && t.b == Service_Error::BUSSINESS_ERROR::NONE && t.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            show_transfer_success_interface(tran_in, u, a);
        }
        else
        {  
            if(t.s != Service_Error::SYSTEM_ERROR::NONE && t.b == Service_Error::BUSSINESS_ERROR::NONE && t.v == Service_Error::VALIDATE_ERROR::NONE)
            {
                if(t.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
                {
                    show_failed_interface("TRANSACTION FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");

                }
                else if(t.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
                {
                    show_failed_interface("TRANSACTION FAILED", "System Error! Try Again!!!");
                }
                else if(t.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
                {
                    show_failed_interface("TRANSACTION FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");
                }
                else if(t.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
                {
                    show_failed_interface("TRANSACTION FAILED", "Connnection Error!!! Please Contact with Admin to give a support!");
                }
            }
            else if(t.s == Service_Error::SYSTEM_ERROR::NONE && t.b != Service_Error::BUSSINESS_ERROR::NONE && t.v == Service_Error::VALIDATE_ERROR::NONE)
            {
                if(t.b == Service_Error::BUSSINESS_ERROR::SAME_ACCOUNT_TRANSFER)
                {
                    show_failed_interface("TRANSACTION FAILED", "Sender and receiver accounts cannot be the same!");
                }
                else if(t.b == Service_Error::BUSSINESS_ERROR::RECEIVER_NOT_EXIST)
                {
                    show_failed_interface("TRANSACTION FAILED", "The Destination User Information does not exist. Try Again!!!");
                }
                else if(t.b ==  Service_Error::BUSSINESS_ERROR::INSUFFICIENT_BALANCE)
                {
                    show_failed_interface("TRANSACTION FAILED", "Insufficent Account Balance");
                }
                else if(t.b == Service_Error::BUSSINESS_ERROR::E_DUPLICATE_TRANSACTION_NO)
                {
                    show_failed_interface("TRANSACTION FAILED", "Cannot generate unique transaction code!!!");
                }
            }
            else if(t.s == Service_Error::SYSTEM_ERROR::NONE && t.b == Service_Error::BUSSINESS_ERROR::NONE && t.v != Service_Error::VALIDATE_ERROR::NONE)
            { 
                if(t.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
                {
                    show_failed_interface("TRANSACTION FAILED", "Wrong Password! Try Again!!!");

                }
                else if (t.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE || t.v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE)
                {
                    show_failed_interface("TRANSACTION FAILED", "Your current account is not available to perform this action.");     

                }
                else if(t.v == Service_Error::VALIDATE_ERROR::RECEIVER_NOT_AVAILABLE)
                {
                    show_failed_interface("TRANSACTION FAILED", "Actions are unavailable on this destination account!!!");
                }
                else if (t.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
                {
                    show_failed_interface("TRANSACTION FAILED", "Session has expired! Try Again!!!");
                    return State_Custom_DashBoard::ENFORCED_LOG_OUT;
                }
            }
            else
            {
                show_failed_interface("TRANSACTION FAILED", "Unknown Error!!!");   
            }
        }
    }
    u.clear_sensitive_data();
    a.clear_sensitive_data();
    AccountService as;
    return custom_refresh(as.refresh(s, a));
}

//custom interface
void CLI::show_custom_dashboard(User& u, Account& a) 
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("MINI BANKING DASHBOARD", WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center("Welcome back, " + u.get_full_name(), WIDTH);
    print_center("ACCOUNT INFORMATION", WIDTH);
    cout << string(WIDTH, '-') << endl;
    cout << "Account Number : " << a.get_account_no() << "\n";
    cout << "Current Balance: $" << fixed << setprecision(2) << a.get_balance() << "\n";
    cout << string(WIDTH, '-') << endl;
    cout << "What do you want to do?\n";
    cout << "1. Transfer Money\n";
    cout << "2. Change Password\n";
    cout << "3. Logout\n";
    cout << string(WIDTH, '-') << endl;
    cout << "Enter your choice: ";   
}

CLI::State_Custom_DashBoard CLI::custom_activity(User& u, Account& a)
{
    while(true)
    {
        show_custom_dashboard(u, a);

        //input
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }
        int choose = stoi(temp_choose);

        if (choose < 1 || choose > 3)
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }

        switch(choose)
        {
            case 1:
            {
                return State_Custom_DashBoard::TRANSFER;
            }
            case 2:
            {
                return State_Custom_DashBoard::CHANGE_PASSWORD;
            }
            case 3:
            {
                return State_Custom_DashBoard::LOG_OUT;
            }
        }

    }

}

CLI::State CLI::main_control_custom_dash_board(User& u, Account& a, Session& s)
{
    State_Custom_DashBoard sc = State_Custom_DashBoard::CUSTOM_MENU;

    while(sc != State_Custom_DashBoard::LOG_OUT && sc != State_Custom_DashBoard::ENFORCED_LOG_OUT)
    {
        switch(sc)
        {
            case (State_Custom_DashBoard::CUSTOM_MENU):
            {
                sc = custom_activity(u, a);
                break;
            }
            case (State_Custom_DashBoard::TRANSFER):
            {
                sc = handle_transfer(s, u, a);
                break;

            }
            case (State_Custom_DashBoard::CHANGE_PASSWORD):
            {
                sc = handle_change_password_user(s, u, a);
                break;
            }
            case State_Custom_DashBoard::LOG_OUT:
            case State_Custom_DashBoard::ENFORCED_LOG_OUT:
            {
                break;
            }
        }


    }
    if(sc == State_Custom_DashBoard::ENFORCED_LOG_OUT)
    {
        return handle_logout(s, u, a, 1);
    }
    return handle_logout(s, u, a, 0);
}

//change password
void CLI::show_change_password_successful_interface()
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE PASSWORD SUCCESSFULLY", WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center("Your Password updated successfully!!!", WIDTH);
    print_center("Please Login Again!", WIDTH);
    cout << string(WIDTH, '-') << endl;
    cout << " Press any key to continue...\n";
    cin.ignore();  
    cin.get();
}

void CLI::show_change_password_interface(string& new_password, string& confirm_new_password)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE PASSWORD", WIDTH);
    cout << string(WIDTH, '=') << endl;
    new_password = user_input("New Password: ");
    confirm_new_password = user_input("Re-enter your new password: ");
}

CLI::State_Custom_DashBoard CLI::handle_change_password_user(Session& s, User& u, Account& a)
{
    string new_password, confirm_new_password;
    show_change_password_interface(new_password, confirm_new_password);
    bool length = Input_Validation::check_password_length(new_password);
    bool confirm = Input_Validation::check_new_password(new_password, confirm_new_password);

    if(!length || !confirm)
    {
        if(!length)
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Password length at Least 8 character!!!");
        }
        else if(!confirm)
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "New Passwords do not match!!!");
        }
        else
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Unknown Error!!!");
        }
        int next_action = show_handle_next_action("Enter 1 to Retry or 0 to Exit this action: ");
        if(next_action == 1)
        {
            return State_Custom_DashBoard::CHANGE_PASSWORD;
        }
        AccountService as;
        return custom_refresh(as.refresh(s, a));

    }
    string current_password;
    show_authenticate_before_sensitive_action("CHANGE PASSWORD", current_password);
    if(!Input_Validation::check_password_length(current_password))
    {
        show_failed_interface("CHANGE PASSWORD FAILED", "Wrong Password! Try Again!!!");
        return State_Custom_DashBoard::CUSTOM_MENU;
    }
    AuthService aus;
    CHANGE_PASSWORD cp = aus.change_password(s, u, current_password, new_password);
    if(cp.s == Service_Error::SYSTEM_ERROR::NONE && cp.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        show_change_password_successful_interface();
        return State_Custom_DashBoard::ENFORCED_LOG_OUT;
    }
    else
    {  
        if(cp.s != Service_Error::SYSTEM_ERROR::NONE && cp.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(cp.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");

            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::CHANGE_PASSWORD_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "The change was unsuccessful!!! Please Contact with Admin to give a support!");
            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "System Error!!! Please Contact with Admin to give a support!");
            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");

            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Connnection Error!!! Please Contact with Admin to give a support!");
            } 
        }
        else if(cp.s == Service_Error::SYSTEM_ERROR::NONE && cp.v != Service_Error::VALIDATE_ERROR::NONE)
        { 
            if(cp.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Wrong Password! Try Again!!!");

            }
            else if (cp.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "User Not Available!!!");

            }
            else if (cp.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Session has expired! Try Again!!!");
                return State_Custom_DashBoard::ENFORCED_LOG_OUT;

            }
        }
        else
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Unknown Error!!!");   
        }
        
    }
    u.clear_sensitive_data();
    AccountService as;
    return custom_refresh(as.refresh(s, a));
}


CLI::State_Admin_DashBoard CLI::handle_change_password_admin(Session& s, User& u, Account& a)
{
    string new_password, confirm_new_password;
    show_change_password_interface(new_password, confirm_new_password);
    bool length = Input_Validation::check_password_length(new_password);
    bool confirm = Input_Validation::check_new_password(new_password, confirm_new_password);

    if(!length || !confirm)
    {
        if(!length)
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Password length at Least 8 character!!!");
        }
        else if(!confirm)
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "New Passwords do not match!!!");
        }
        else
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Unknown Error!!!");
        }
        int next_action = show_handle_next_action("Enter 1 to Retry or 0 to Exit this action: ");
        if(next_action == 1)
        {
            return State_Admin_DashBoard::CHANGE_PASSWORD;
        }
        AccountService as;
        return admin_refresh(as.refresh(s, a));

    }
    string current_password;
    show_authenticate_before_sensitive_action("CHANGE PASSWORD", current_password);
    if(!Input_Validation::check_password_length(current_password))
    {
        show_failed_interface("CHANGE PASSWORD FAILED", "Wrong Password! Try Again!!!");
        return State_Admin_DashBoard::ADMIN_MENU;
    }
    AuthService aus;
    CHANGE_PASSWORD cp = aus.change_password(s, u, current_password, new_password);
    if(cp.s == Service_Error::SYSTEM_ERROR::NONE && cp.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        show_change_password_successful_interface();
        return State_Admin_DashBoard::ENFORCED_LOG_OUT;
    }
    else
    {  
        if(cp.s != Service_Error::SYSTEM_ERROR::NONE && cp.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(cp.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "DataBase Error!");

            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::CHANGE_PASSWORD_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "The change was unsuccessful!");
            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "System Error!");
            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Fetch Data Failed!");

            }
            else if(cp.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Connnection Error!");
            } 
        }
        else if(cp.s == Service_Error::SYSTEM_ERROR::NONE && cp.v != Service_Error::VALIDATE_ERROR::NONE)
        { 
            if(cp.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Wrong Password! Try Again!!!");

            }
            else if (cp.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "User Not Available!!!");

            }
            else if (cp.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
            {
                show_failed_interface("CHANGE PASSWORD FAILED", "Session has expired! Try Again!!!");
                return State_Admin_DashBoard::ENFORCED_LOG_OUT;

            }
        }
        else
        {
            show_failed_interface("CHANGE PASSWORD FAILED", "Unknown Error!!!");   
        }
    }
    u.clear_sensitive_data();
    AccountService as;
    return admin_refresh(as.refresh(s, a));
}
//set permission
void CLI::show_change_status_object_success_interface(const string& center_message, const string& message)
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center(center_message, WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center(message, WIDTH);
    cout << string(WIDTH, '-') << endl;
    cout << " Press any key to continue...\n";
    cin.ignore(); 
    cin.get();    
}

void CLI::show_option_to_change_status_account(vector<string>& opt_st, Status_Account_Info& sai)
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE STATUS ACCOUNT", WIDTH);
    cout << string(WIDTH, '=') << endl;
    cout << "Account ID: " << sai.accountid << endl;
    cout << "Current Status: " << sai.current_status << endl;
    cout << "Which state do you want to switch to?\n";
    for(size_t i = 0; i < opt_st.size(); i ++)
    {
        cout << i + 1 << ". " << opt_st[i] << endl;    
    }
    cout << string(WIDTH, '-') << endl;
    cout << "Enter your choice: ";
}

string CLI::input_new_status_account(Status_Account_Info& sai)
{
    int choose = 0;
    vector<string> opt_st= {"ACTIVE", "SUSPENDED", "LOCKED", "CLOSED"};
    opt_st.erase(remove(opt_st.begin(), opt_st.end(), sai.current_status), opt_st.end());
    while(true)
    {
        show_option_to_change_status_account(opt_st, sai);
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("CHANGE STATUS ACCOUNT", "Invalid Input!!! Try Again!");
            continue;
        }
        int choose = stoi(temp_choose);

        if (choose < 1 || choose > 3)
        {
            show_failed_interface("CHANGE STATUS ACCOUNT", "Invalid Input!!! Try Again!");
            continue;

        }
        break;
    }
    return opt_st[choose - 1];
}

//change status account
void CLI::show_change_status_account_interface(string& acc_no)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE STATUS ACCOUNT", WIDTH);
    cout << string(WIDTH, '=') << endl;
    acc_no = user_input("Account NO of User: ");
}

CLI::State_Admin_DashBoard CLI::handle_change_status_account(Session& s, User& u, Account& a)
{
    string account_no;
    show_change_status_account_interface(account_no);
    Status_Account_Info sai(account_no);
    AccountService as;
    LOAD_DATA ld = as.get_account_status_info(sai);
    if(ld.s == Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        sai.new_status = input_new_status_account(sai); 
        string password;
        show_authenticate_before_sensitive_action("CHANGE STATUS ACCOUNT", password);
        AdminService ads;
        CHANGE_STATUS csa = ads.change_status_account(s, u, a, sai, password);
        if(csa.s == Service_Error::SYSTEM_ERROR::NONE && csa.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            show_change_status_object_success_interface("CHANGE STATUS ACCOUNT SUCCESSFULLY", "The target account status has been changed");
        }
        else
        {
            if(csa.s != Service_Error::SYSTEM_ERROR::NONE && csa.v == Service_Error::VALIDATE_ERROR::NONE)
            {
                if(csa.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Connnection Error!");
                }
                else if(csa.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Fetch Data Failed!");
                }
                else if(csa.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "System Error!");
                    
                }
                else if(csa.s == Service_Error::SYSTEM_ERROR::CHANGE_STATUS_ACCOUNT_FAILED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "The change was unsuccessful!");
                }
                else if(csa.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "DataBase Error!");
                }
            }
            else if(csa.v != Service_Error::VALIDATE_ERROR::NONE && csa.s == Service_Error::SYSTEM_ERROR::NONE)
            {
                if(csa.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Login session has expired!");
                    return State_Admin_DashBoard::ENFORCED_LOG_OUT; 
                }
                else if(csa.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "User Not Available!!!");
                }
                else if(csa.v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Account Not Available!!!");
                }
                else if(csa.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Wrong Password!");
                }
                else
                {
                    show_failed_interface("CHANGE STATUS ACCOUNT FAILED", "Unknown Error!!!");   
                }
            }
            
        }
    }
    else if(ld.s != Service_Error::SYSTEM_ERROR::NONE)
    {
        if(ld.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
        {
            show_failed_interface("GET ACCOUNT STATUS INFORMATION FAILED", "Connnection Error!");
        }
        else if(ld.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
        {
            show_failed_interface("GET ACCOUNT STATUS INFORMATION FAILED", "Fetch Data Failed!");
        }

    }
    else if(ld.b != Service_Error::BUSSINESS_ERROR::NONE)
    {
        if(ld.b == Service_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND)
        {
            show_failed_interface("GET ACCOUNT STATUS INFORMATION FAILED", "Account Not Found!");
        }
    }
    else
    {
        show_failed_interface("GET ACCOUNT STATUS INFORMATION FAILED", "Unknown Error!!!");   
    }
        
    u.clear_sensitive_data();
    a.clear_sensitive_data();
    return admin_refresh(as.refresh(s, a));
}

//change status user
void CLI::show_option_to_change_status_user(vector<string>& opt_st, Status_User_Info& sui)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE STATUS USER", WIDTH);
    cout << string(WIDTH, '=') << endl;
    cout << "Full Name: " << sui.full_name << endl;
    cout << "Current Status: " << sui.current_status << endl;
    cout << "Which state do you want to switch to?\n";
    for(size_t i = 0; i < opt_st.size(); i ++)
    {
        cout << i + 1 << ". " << opt_st[i] << endl;
        
    }
    cout << string(WIDTH, '-') << endl;
    cout << "Enter your choice: ";
}

string CLI::input_new_status_user(Status_User_Info& sui)
{
    int choose;
    vector<string> opt_st= {"ACTIVE", "SUSPENDED", "LOCKED", "CLOSED"};
    opt_st.erase(remove(opt_st.begin(), opt_st.end(), sui.current_status), opt_st.end());
    while(true)
    {
        
        show_option_to_change_status_user(opt_st, sui);
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("CHANGE STATUS USER", "Invalid Input!!! Try Again!");
            continue;
        }
        choose = stoi(temp_choose);

        if (choose < 1 || choose > 3)
        {
            show_failed_interface("CHANGE STATUS USER", "Invalid Input!!! Try Again!");
            continue;

        }
        break;
    }
    return opt_st[choose - 1];

    
}

void CLI::show_change_status_user_interface(string& p)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("CHANGE STATUS USER", WIDTH);
    cout << string(WIDTH, '=') << endl;
    p = user_input("Phone Number of User: ");  
}

CLI::State_Admin_DashBoard CLI::handle_change_status_user(Session& s, User& u, Account& a)
{
    string phone_number;
    show_change_status_user_interface(phone_number);
    Status_User_Info sui(phone_number);
    AccountService as;
    LOAD_DATA ld = as.get_user_status_info(sui);
    if(ld.s == Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        sui.new_status = input_new_status_user(sui);
        string password;
        show_authenticate_before_sensitive_action("CHANGE STATUS USER", password);
        AdminService ads;
        CHANGE_STATUS csu = ads.change_status_user(s, u, a, sui, password);
       
        if(csu.s == Service_Error::SYSTEM_ERROR::NONE && csu.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            show_change_status_object_success_interface("CHANGE STATUS USER SUCCESSFULLY", "The target user status has been changed");
        }
        else
        {
            if(csu.s != Service_Error::SYSTEM_ERROR::NONE && csu.v == Service_Error::VALIDATE_ERROR::NONE)
            {
                if(csu.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "Connnection Error!");
                   
                }
                else if(csu.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "Fetch Data Failed!");
                }
                else if(csu.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "System Error!");
                    
                }
                else if(csu.s == Service_Error::SYSTEM_ERROR::CHANGE_STATUS_USER_FAILED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "The change was unsuccessful!");
                }
                else if(csu.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "DataBase Error!");
                }
            }
            else if(csu.v != Service_Error::VALIDATE_ERROR::NONE && csu.s == Service_Error::SYSTEM_ERROR::NONE)
            {
                if(csu.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "Login session has expired!");
                    return State_Admin_DashBoard::ENFORCED_LOG_OUT; 
                }
                else if(csu.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE )
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "User Not Available!!!");
                }
                else if(csu.v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "Account Not Available!!!");
                }
                else if(csu.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
                {
                    show_failed_interface("CHANGE STATUS USER FAILED", "Wrong Password!");
                }
            }
            else
            {
                show_failed_interface("CHANGE STATUS USER FAILED", "Unknown Error!!!");   
            }
        }
    }
    else if(ld.s != Service_Error::SYSTEM_ERROR::NONE && ld.b == Service_Error::BUSSINESS_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        if(ld.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
        {
            show_failed_interface("GET USER STATUS INFORMATION FAILED", "Connnection Error!");
        }
        else if(ld.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
        {
            show_failed_interface("GET USER STATUS INFORMATION FAILED", "Fetch Data Failed!");
        }

    }
    else if(ld.b != Service_Error::BUSSINESS_ERROR::NONE && ld.s == Service_Error::SYSTEM_ERROR::NONE && ld.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        if(ld.b == Service_Error::BUSSINESS_ERROR::USER_NOT_EXIST)
        {
            show_failed_interface("GET USER STATUS INFORMATION FAILED", "User does not exist!");
        }
    }
    else
    {    
        show_failed_interface("GET USER STATUS INFORMATION FAILED", "Unknown Error!!!");   
    }
    u.clear_sensitive_data();
    a.clear_sensitive_data();
    return admin_refresh(as.refresh(s, a));
}


//admin interface
void CLI::show_admin_dashboard()
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("ADMIN CONTROL PANEL", WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center("Welcome back, ADMIN!", WIDTH);
    cout << string(WIDTH, '-') << endl;
    cout << "1. Change Status User\n"; 
    cout << "2. Change Status Account\n"; 
    cout << "3. Change Admin Password\n"; 
    cout << "4. Logout\n";
    cout << string(WIDTH, '-') << endl;
    cout << "Enter your choice: ";

}

CLI::State_Admin_DashBoard CLI::admin_activity()
{
    while(true)
    {
        show_admin_dashboard();
        //input
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }
        int choose = stoi(temp_choose);
        if (choose < 1 || choose > 4)
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }
        switch(choose)
        {
            case 1:
            {
                return State_Admin_DashBoard::CHANGE_STATUS_USER;
            }
            case 2:
            {
                return State_Admin_DashBoard::CHANGE_STATUS_ACCOUNT;
            }
            case 3:
            {
                return State_Admin_DashBoard::CHANGE_PASSWORD;
            }
            case 4:
            {
                return State_Admin_DashBoard::LOG_OUT;
            }

        }

    }
    
}


CLI::State CLI::main_control_admin_dash_board(User& u, Account& a, Session& s)
{
    CLI::State_Admin_DashBoard sa = CLI::State_Admin_DashBoard::ADMIN_MENU;

    while(sa != CLI::State_Admin_DashBoard::LOG_OUT && sa != State_Admin_DashBoard::ENFORCED_LOG_OUT)
    {
        switch(sa)
        {
            case (CLI::State_Admin_DashBoard::ADMIN_MENU):
            {
                sa = admin_activity();
                break;
                
            }
            case (CLI::State_Admin_DashBoard::CHANGE_STATUS_USER):
            {
                sa = handle_change_status_user(s, u, a);
                break;
            }
            case (CLI::State_Admin_DashBoard::CHANGE_STATUS_ACCOUNT):
            {
                sa = handle_change_status_account(s, u, a);
                break;
            }
            case (CLI::State_Admin_DashBoard::CHANGE_PASSWORD):
            {
                sa = handle_change_password_admin(s, u, a);
                break;
            }
            case State_Admin_DashBoard::LOG_OUT:
            case State_Admin_DashBoard::ENFORCED_LOG_OUT:
            {
                break;
            }
            
        }

    }
    if(sa == State_Admin_DashBoard::ENFORCED_LOG_OUT)
    {
        return handle_logout(s, u, a, 1);
    }
    return handle_logout(s, u, a, 0);
}
//login flow

CLI::State CLI::define_state_login(Session& s, User& u, Account& a, LOGIN l)
{
    if(l.s == Service_Error::SYSTEM_ERROR::NONE && l.b == Service_Error::BUSSINESS_ERROR::NONE && l.v == Service_Error::VALIDATE_ERROR::NONE)
    {
        switch(u.get_role_id())
        {
            case(1):
            {
                u.clear_sensitive_data();
                return State::ADMIN_MENU;
            }
            case(2):
            {
                u.clear_sensitive_data();
                return State::CUSTOM_MENU;
            }
        }
    }
    else
    {  
        if(l.s != Service_Error::SYSTEM_ERROR::NONE && l.b == Service_Error::BUSSINESS_ERROR::NONE && l.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(l.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR)
            {
                show_failed_interface("LOGIN FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");

            }
            else if(l.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
            {
                show_failed_interface("LOGIN FAILED", "System Error!!! Please Contact with Admin to give a support!");
            }
            else if(l.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
            {
                show_failed_interface("LOGIN FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");

            }
            else if(l.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
            {
                show_failed_interface("LOGIN FAILED", "Connnection Error!!! Please Contact with Admin to give a support!");
            } 
        }
        else if(l.s == Service_Error::SYSTEM_ERROR::NONE && l.b != Service_Error::BUSSINESS_ERROR::NONE && l.v == Service_Error::VALIDATE_ERROR::NONE)
        {
            if(l.b == Service_Error::BUSSINESS_ERROR::USER_NOT_EXIST)
            {
                show_failed_interface("LOGIN FAILED", "User does not exist!!! Try Againt!!!");
            }
        }
        else if(l.s == Service_Error::SYSTEM_ERROR::NONE && l.b == Service_Error::BUSSINESS_ERROR::NONE && l.v != Service_Error::VALIDATE_ERROR::NONE)
        { 
            if(l.v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
            {
                // sau khi mô phỏng attack tiến hành cho nhập giới hạn lần password 
                show_failed_interface("LOGIN FAILED", "Wrong Password! Try Again!!!");

            }
            else if (l.v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
            {
                show_failed_interface("LOGIN FAILED", "User Not Available!!!");

            }
            else if (l.v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED)
            {
                show_failed_interface("LOGIN FAILED", "Login session has expired! Try Again!!!");
            }
        }
        else
        {
            show_failed_interface("GET USER STATUS INFORMATION FAILED", "Unknown Error!!!");   
        }
        
    }
    u.clear_all_user_data();
    a.clear_all_account_data();
    s.clear_session();
    int next_action = show_handle_next_action("Enter 1 to Retry or 0 to Exit this action: ");
    if(next_action == 1)
    {
        return State::LOGIN;
    }
    return State::MAIN_MENU;
    
}

void CLI::show_login_interface(string& phone_number, string& password)
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("MINI BANKING", WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center("LOGIN", WIDTH);
    cout << string(WIDTH, '=') << endl;
    
    phone_number = user_input("Phone Number: ");
    password = user_input("Password: ");

    cout << string(WIDTH, '-') << endl;
    cout << "Authenticating...\n";
    
}

CLI::State CLI::handle_login(Session& s, User& u, Account& a)
{
    string phone_number;
    string password;

    show_login_interface(phone_number, password);
    LOGIN l;

    if(pr_state == State::REGISTER && (phone_number == u.get_phone_number()))
    {
        AuthService as;
        l = as.login(u, a, s, phone_number, password, 1);
    }
    else
    {
        AuthService as;
        l = as.login(u, a, s, phone_number, password, 0);
    }
    return define_state_login(s, u, a, l);
    
}
//register flow

void CLI::show_register_interface()
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("MINI BANKING", WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center("REGISTER", WIDTH);
    cout << string(WIDTH, '=') << endl;

}

string CLI::user_input(const string& message)
{
    string u_in;
    cout << message;
    getline(cin, u_in);
    return u_in;
}


string CLI::input_type_account()
{
    show_register_interface();
    cout << "Type Account?\n";
    cout << "1. Current Account?\n";
    cout << "2. Business Account?\n";
    cout << "Enter (1-2): ";

    string t;
    getline(cin, t);
    return t;
}

void CLI::show_register_success_interface(User& u)
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("ACCOUNT CREATED SUCCESSFULLY", WIDTH);
    cout << string(WIDTH, '=') << endl;

    cout << " Username : " << u.get_full_name() << endl;
    cout << " Status   : ACTIVE\n\n";

    cout << " Your account has been successfully registered.\n";
    cout << " You can now login to access the system.\n\n";

    cout << string(WIDTH, '-') << endl;
}

int CLI::show_handle_next_action(const string& option_message)
{
    int choose;
    while(true)
    {
        cout << option_message;
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }
        choose = stoi(temp_choose); 
        if(choose == 1 || choose == 0)
        {
            break;
        }
        show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
        clear();
    }
    return choose;
}

void CLI::show_failed_interface(const string& center_message, const string& message)
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center(center_message, WIDTH);
    cout << string(WIDTH, '=') << endl;
    print_center(message, WIDTH);

    cout << string(WIDTH, '-') << endl;
    cout << "Press any key to continue...\n";
    cin.ignore(); 
    cin.get();     
}

CLI::State CLI::handle_register(User& u, Account& a)
{
    //user input
    retry_until_valid([&](){u.set_full_name(user_input("Full Name: "));});
    retry_until_valid([&](){u.set_phone_number(user_input("Phone Number('Include 10 digit and start with 0'): "));});
    retry_until_valid([&](){u.set_credential(user_input("Password('At Least 8 character'): "));});
    u.set_status_user("ACTIVE");
    u.set_role_id(2);
    retry_until_valid([&](){a.make_type(input_type_account());});
    a.set_status("ACTIVE");
    a.set_balance(0);

    AuthService as;
    REGISTER r;
    while(true)
    {
        r = as.registry(u, a);
        if(r.b == Service_Error::BUSSINESS_ERROR::E_DUPLICATE_PHONE_NO)
        {
            show_failed_interface("INVALID INFORMATION", "This phone number is already being used by another account. Please use a different phone number!");
            retry_until_valid([&](){u.set_phone_number(user_input("Phone Number('Include 10 digit and start with 0'): "));});
            continue;
        }
        else
        {
            if(r.s == Service_Error::SYSTEM_ERROR::NONE && r.b == Service_Error::BUSSINESS_ERROR::NONE)
            {
                show_register_success_interface(u);
                int next_action = show_handle_next_action("Enter 1 to Login Now or 0 to return the Main Menu: ");
                if(next_action == 1)
                {
                    return State::LOGIN;
                }
            }
            else
            {
                if(r.s != Service_Error::SYSTEM_ERROR::NONE && r.b == Service_Error::BUSSINESS_ERROR::NONE)
                {
                    if(r.s == Service_Error::SYSTEM_ERROR::CREATE_USER_FAILED || r.s == Service_Error::SYSTEM_ERROR::CREATE_ACCOUNT_FAILED || r.s == Service_Error::SYSTEM_ERROR::DATABASE_ERROR || r.b == Service_Error::BUSSINESS_ERROR::E_DUPLICATE_ACCOUNT_NO)
                    {
                        show_failed_interface("REGISTER FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");
                    }
                    else if(r.s == Service_Error::SYSTEM_ERROR::CONNECTION_FAILED)
                    {
                        show_failed_interface("REGISTER FAILED", "Connection Failed!!! Please Contact with Admin to give a support!");
                    }
                    else if(r.s == Service_Error::SYSTEM_ERROR::HASH_FAILED)
                    {
                        show_failed_interface("REGISTER FAILED", "System Error!!! Please Contact with Admin to give a support!");
                    }
                    else if(r.s == Service_Error::SYSTEM_ERROR::FETCH_FAILED)
                    {
                        show_failed_interface("REGISTER FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");
                    }
                }
                else if(r.s == Service_Error::SYSTEM_ERROR::NONE && r.b != Service_Error::BUSSINESS_ERROR::NONE)
                {
                    show_failed_interface("REGISTER FAILED", "Cannot generate unique account number!!!");
                }
                else
                {
                    show_failed_interface("REGISTER FAILED", "Unknown Error!!!");
                }
            }
        }
        break;
    }
    u.clear_all_user_data();
    a.clear_all_account_data();
    return State::MAIN_MENU;
}

//start flow
void CLI::show_start_interface()
{
    clear();

    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("MINI BANKING", WIDTH);
    print_center("Hello, Welcome to Mini Bank!", WIDTH);
    cout << string(WIDTH, '=') << endl;
    cout << "What do you want to do?\n";
    cout << "1. Register account\n";
    cout << "2. I already have an account before! Login?\n";
    cout << "3. Exit Program!\n";
    cout << string(WIDTH, '-') << endl;
    cout << "Enter (1) or (2) to start: ";
}

void CLI::show_end_interface()
{
    clear();
    const int WIDTH = 50;
    cout << string(WIDTH, '=') << endl;
    print_center("Thank you for using Mini Bank!", WIDTH);
    print_center("See you next time.", WIDTH);
    cout << string(WIDTH, '=') << endl;
}

CLI::State CLI::activity()
{
    while(true)
    {
        show_start_interface();
        //input
        string temp_choose;
        getline(cin, temp_choose);
        if(!Input_Validation::check_user_input(temp_choose))
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }
        int choose = stoi(temp_choose);
        if (choose < 1 || choose > 3)
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;

        }
        switch(choose)
        {
            case 1:
            {
                return State::REGISTER;
            }
            case 2:
            {
                return State::LOGIN;
            }
            case 3:
            {
                return State::EXIT;
            }

        }
    }
}

void CLI::main_control()
{
    State s = State::MAIN_MENU;
    Account a;
    User u;
    Session session;

    while(s != State::EXIT)
    {
        switch(s)
        {
            case (State::MAIN_MENU):
            {
                s = activity();
                pr_state = State::MAIN_MENU;
                break;
            }
            case (State::REGISTER):
            {
                s = handle_register(u, a);
                pr_state = State::REGISTER;
                break;
            }
            case (State::LOGIN):
            {
                s = handle_login(session, u, a); 
                pr_state = State::LOGIN;
                break;
            }
            case (State::ADMIN_MENU):
            {
                s = main_control_admin_dash_board(u, a, session);
                pr_state = State::ADMIN_MENU;
                break;
            }
            case (State::CUSTOM_MENU):
            {
                s = main_control_custom_dash_board(u, a, session);  
                pr_state = State::CUSTOM_MENU;
                break;

            }
            case (State::EXIT):
            {
                break;
            }
        }

    }
    show_end_interface(); //end point
}

void CLI::clear()
{
    system("cls");
}

void CLI::print_center(const string& text, int width)
{
    int padding = (width - text.size()) / 2;

    if(padding < 0)
    {
        padding = 0;
    }

    cout << string(padding, ' ') << text << endl;
}