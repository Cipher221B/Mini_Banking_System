#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "interface.h"
#include "random_generation.h"
#include "hash.h"
#include "validation_helper.h"
#include "transaction_information.h"
#include "account_service.h"
#include "system_errors.h"

//custom interface
void CLI::show_custom_dashboard(User& u, Account& a) 
{
    clear();

    cout << "\n=====================================\n";
    cout << "           MINI BANKING DASHBOARD\n";
    cout << "=====================================\n\n";

    cout << "Welcome back, " << u.full_name << "!\n\n";

    cout << "--------- ACCOUNT INFORMATION ---------\n";
    cout << "Account Number : " << a.account_no << "\n";
    cout << "Current Balance: $" << fixed << setprecision(2) << a.balance << "\n";
    cout << "---------------------------------------\n\n";

    cout << "What do you want to do?\n";
    cout << "1. Transfer Money\n";
    cout << "2. View Transaction History\n";
    cout << "3. Create New Account\n";
    cout << "4. Logout\n";
    cout << "---------------------------------------\n";
    cout << "Enter your choice: ";
   
}

CLI::State_Custom_DashBoard CLI::custom_activity(User& u, Account& a)
{
    while(true)
    {
        //!check lại session, get lại data
        show_custom_dashboard(u, a);

        //input
        int choose;
        cin >> choose;
        cin.ignore();

        if (choose < 1 || choose > 4)
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
                return State_Custom_DashBoard::VIEW_LOG_TRANSACTION;
            }
            case 3:
            {
                return State_Custom_DashBoard::CREATE_ACCOUNT;
            }
            case 4:
            {
                return State_Custom_DashBoard::LOG_OUT;
            }
        }

    }

}

CLI::State CLI::main_control_custom_dash_board(User& u, Account& a, Session& s)
{
    State_Custom_DashBoard sc = State_Custom_DashBoard::CUSTOM_MENU;

    while(sc != State_Custom_DashBoard::LOG_OUT)
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
                //view_user
                break;

            }
            case (State_Custom_DashBoard::VIEW_LOG_TRANSACTION):
            {
                //block-unblock
                break;
            }
            case (State_Custom_DashBoard::CREATE_ACCOUNT):
            {
                //delete account
                break;
            }
        }

    }
    return State::MAIN_MENU;

}
//admin interface

void CLI::show_admin_dashboard()
{
    clear();

    cout << "\n=====================================\n";
    cout << "         ADMIN CONTROL PANEL\n";
    cout << "=====================================\n\n";

    cout << "         Welcome back, ADMIN!\n\n";

    cout << "--------- ADMIN OPERATIONS ---------\n";
    cout << "1. View All Users\n"; //có thể tạo thêm view transaction log
    cout << "3. Block / Unblock User\n"; 
    cout << "4. Delete Account\n";
    cout << "5. Logout\n";
    cout << "------------------------------------\n";
    cout << "Enter your choice: ";

}

CLI::State_Admin_DashBoard CLI::admin_activity()
{
    while(true)
    {
        //!check lại session, get lại data
        show_admin_dashboard();

        //input
        int choose;
        cin >> choose;
        cin.ignore();

        if (choose < 1 || choose > 5)
        {
            show_failed_interface("MINI BANKING", "Invalid Input!!! Try Again!");
            continue;
        }

        switch(choose)
        {
            case 1:
            {
                return State_Admin_DashBoard::VIEW_USER;
            }
            case 2:
            {
                return State_Admin_DashBoard::SET_PERMISSION_USER;
            }
            case 3:
            {
                return State_Admin_DashBoard::DELETE_ACCOUNT_CUSTOM;
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
    if(refresh_data(u, a) == AccountService::LOAD_DATA::SUCCESSS)
    {
        CLI::State_Admin_DashBoard sa = CLI::State_Admin_DashBoard::ADMIN_MENU;

        while(sa != CLI::State_Admin_DashBoard::LOG_OUT)
        {
            switch(sa)
            {
                case (CLI::State_Admin_DashBoard::ADMIN_MENU):
                {
                    sa = admin_activity();
                    break;
                }
                case (CLI::State_Admin_DashBoard::VIEW_USER):
                {
                    //view_user
                    break;

                }
                case (CLI::State_Admin_DashBoard::SET_PERMISSION_USER):
                {
                    //block-unblock
                    break;
                }
                case (CLI::State_Admin_DashBoard::DELETE_ACCOUNT_CUSTOM):
                {
                    //delete account
                    break;
                }
            }

        }
    }

    return CLI::State::MAIN_MENU;

}
//login flow

CLI::State CLI::define_state_login(User& u, Account& a, AuthService::LOGIN l)
{
    if(l == AuthService::LOGIN::SUCCESS)
    {
        string cs = Validate::check_role(u, a);
        if(cs == "ADMIN")
        {
            cout << "-------------------------------------\n";
            cout << " Press any key to continue...\n";

            cin.ignore();  // clear buffer
            cin.get();     // wait for key press

            return State::ADMIN_MENU;

        }
        else if(cs == "CUSTOM")
        {
            cout << "-------------------------------------\n";
            cout << " Press any key to continue...\n";

            cin.ignore();  // clear buffer
            cin.get();     // wait for key press

            return State::CUSTOM_MENU;
        }

    }    
    else if(l == AuthService::LOGIN::DATABASE_ERROR)
    {
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";

        cin.ignore();  // clear buffer
        cin.get();     // wait for key press
        show_failed_interface("LOGIN FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");

    }
    else if(l == AuthService::LOGIN::USER_DOES_NOT_EXIST)
    {
        show_failed_interface("LOGIN FAILED", "User does not exist!!! Try Againt!!!");
        //có thể cho đệ quy để gọi lại hàm kết hợp if else chỉ cho phép log in vài lần

    }
    else if(l == AuthService::LOGIN::ACCOUNT_NOT_FOUND)
    {
        show_failed_interface("LOGIN FAILED", "Can't found account on this User!!!");
        
    }
    else if(l == AuthService::LOGIN::VERIFY_FAILED)
    {
        // sau khi mô phỏng attack tiến hành cho nhập giới hạn lần password 
        show_failed_interface("LOGIN FAILED", "Wrong Password! Try Again!!!");

    }
    else if (l == AuthService::LOGIN::USER_NOT_AVAILABLE || l == AuthService::LOGIN::FAILED)
    {
        show_failed_interface("LOGIN FAILED", "User Not Available!!!");

    }
    return State::MAIN_MENU;
}

void CLI::show_login_interface(string& phone_number, string& password)
{
    clear();

    cout << "\n=====================================\n";
    cout << "             MINI BANKING \n";
    cout << "=====================================\n";
    cout << "                LOGIN\n";
    cout << "-------------------------------------\n";
    
    cout << " Phone Number : ";
    getline(cin, phone_number);

    cout << " Password : ";
    getline(cin, password);

    cout << "\n-------------------------------------\n";
    cout << " Authenticating...\n";
    
}

CLI::State CLI::handle_login(Session& s, User& u, Account& a)
{
    string phone_number;
    string password;

    show_login_interface(phone_number, password);
    AuthService::LOGIN l;

    if(pr_state == State::REGISTER && (phone_number == u.phone_number))
    {
        AuthService as;
        l = as.login(u, a, s, phone_number, password, 1);
    }
    else
    {
        AuthService as;
        l = as.login(u, a, s, phone_number, password, 0);
    }

    return define_state_login(u, a, l);
    
}
//register flow

void CLI::show_register_interface()
{
    clear();

    cout << "=====================================\n";
    cout << "            MINI BANKING  \n";
    cout << "              REGISTER\n";
    cout << "=====================================\n\n";   

}

string CLI::input_phone()
{
    string pn;
    cout << "Phone Number('Include 10 digit and start with 0'): ";
    getline(cin, pn);
    return pn;
}

string CLI::input_password()
{
    string p;
    cout << "Password('At Least 8 character'): ";
    getline(cin, p);
    return p;

}

string CLI::input_full_name()
{
    string f;
    cout << "Full Name: ";
    getline(cin, f);
    return f;
    
}

void CLI::create_password(User& u)
{
    try
    {
        u.set_credential(input_password());
    }
    catch(const Invalid_Input& e)
    {
        cout << e.what() << " Try Againt!!!" << endl;
        create_password(u);
    }

}

void CLI::create_phone(User& u)
{
    try
    {
        u.set_phone_number(input_phone());
    }
    catch(const Invalid_Input& e)
    {
        cout << e.what() << " Try Againt!!!" << endl;
        create_phone(u);
    }
}

void CLI::create_full_name(User& u)
{
    try
    {
        u.set_full_name(input_full_name());
    }
    catch(const Invalid_Input& e)
    {
        cout << e.what() << " Try Againt!!!" << endl;
        create_full_name(u);
    }

}

int CLI::input_type_account()
{
    clear();
    cout << "Type Account?\n";
    cout << "1. Current Account!\n";
    cout << "2. Saving Account\n";
    cout << "3. Credit Account?\n";
    cout << "4. Business Account?\n";
    cout << "Enter (1-4): ";

    int choose;
    cin >> choose;
    cin.ignore();
    return choose;
}

void CLI::account_information(Account& a)
{
    try
    {
        a.make_type(input_type_account());
        a.set_status("ACTIVE");
    }
    catch(const Invalid_Input& e)
    {
        cout << e.what() << " Try Again!!!\n";
        account_information(a);
    }

}

void CLI::show_register_success_interface(User& u)
{
    clear();

    cout << "\n=====================================\n";
    cout << "    ACCOUNT CREATED SUCCESSFULLY\n";
    cout << "=====================================\n\n";

    cout << " Username : " << u.full_name << endl;
    cout << " Status   : ACTIVE\n\n";

    cout << " Your account has been successfully registered.\n";
    cout << " You can now login to access the system.\n\n";

    cout << "-------------------------------------\n";
    cout << " Press any key to continue...\n";

    cin.ignore();  // clear buffer
    cin.get();     // wait for key press

}


void CLI::show_failed_interface(const string& center_message, const string& message)
{
    clear();

    cout << "=====================================\n";
    cout << "           "  << center_message << "   \n";
    cout << "=====================================\n\n";
    cout << message << endl;

    cout << "-------------------------------------\n";
    cout << " Press any key to continue...\n";
    cin.ignore(); 
    cin.get();     

}

CLI::State CLI::handle_register(User& u, Account& a)
{
    //input user
    show_register_interface();
    create_full_name(u);
    create_phone(u);
    create_password(u);
    u.set_status_user("ACTIVE");
    u.set_role_id(2);

    //input account
    account_information(a);

    AuthService as;
    AuthService::REGISTER r;
    while(true)
    {
        r = as.registry(u, a);
        if(r == AuthService::REGISTER::E_DUPLICATE_PHONE_NO)
        {
            cout << "This phone number is already being used by another account. Please use a different phone number!\n";
            create_phone(u);
            continue;
        }
        break;

    }

    if(r == AuthService::REGISTER::CREATE_USER_FAILED || r == AuthService::REGISTER::CREATE_ACCOUNT_FAILED || r == AuthService::REGISTER::HASH_FAILED )
    {
        cout << "-------------------------------------\n";
        cout << " Press any key to continue...\n";
        cin.ignore(); 
        cin.get();     
        show_failed_interface("REGISTER FAILED", "DataBase Error!!! Please Contact with Admin to give a support!");
        return State::MAIN_MENU;
    }
    else
    {
        switch(r)
        {
            case(AuthService::REGISTER::SUCCESS):
            {
                cout << "-------------------------------------\n";
                cout << " Press any key to continue...\n";
                cin.ignore(); 
                show_register_success_interface(u);
                return State::LOGIN;
            }
            case(AuthService::REGISTER::CONNECTION_FAILED):
            {
                cout << "-------------------------------------\n";
                cout << " Press any key to continue...\n";
                cin.ignore(); 
                show_failed_interface("REGISTER FAILED", "Connection Failed!!! Please Contact with Admin to give a support!");
                return State::MAIN_MENU;
            }
            case(AuthService::REGISTER::FETCH_FAILED):
            {
                cout << "-------------------------------------\n";
                cout << " Press any key to continue...\n";
                cin.ignore(); 
                show_failed_interface("REGISTER FAILED", "Fetch Data Failed!!! Please Contact with Admin to give a support!");
                return State::MAIN_MENU;
            }

        }
    }
    cout << "-------------------------------------\n";
    cout << " Press any key to continue...\n";
    cin.ignore(); 
    show_failed_interface("REGISTER FAILED", "Register Failed!!! Check Your Information and Try Again!");
    return State::MAIN_MENU;

}

//start flow
void CLI::show_start_interface()
{
    clear();

    cout << "=====================================\n";
    cout << "            MINI BANKING  \n";
    cout << "    Hello, Welcome to Mini Bank!\n";
    cout << "=====================================\n\n";
    cout << "What do you want to do?\n";
    cout << "1. Register account\n";
    cout << "2. I already have an account before! Login?\n";
    cout << "3. Exit Program!\n";
    cout << "-------------------------------------\n";
    cout << "Enter (1) or (2) to start: ";

}

void CLI::show_end_interface()
{
    clear();
    cout << "\n=====================================\n";
    cout << "     Thank you for using Mini Bank!\n";
    cout << "         See you next time.\n";
    cout << "=====================================\n";
}

CLI::State CLI::activity()
{
    while(true)
    {
        show_start_interface();

        //input
        int choose;
        cin >> choose;
        cin.ignore();

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
                s = handle_login(session, u, a); //!!!vấn đề ở phần register done thì truyền data vào 
                pr_state = State::LOGIN;
                break;
            }
            case (State::ADMIN_MENU):
            {
                s = main_control_admin_dash_board(u, a, session);
                //ghi log log out
                pr_state = State::ADMIN_MENU;
                break;
            }
            case (State::CUSTOM_MENU):
            {
                s = main_control_custom_dash_board(u, a, session); //!!!vấn đề ở phần register done thì truyền data vào 
                pr_state = State::CUSTOM_MENU;
                break;

            }
        }

    }
    show_end_interface(); //end point
}

// don't care



// void CLI::check_account_no(string& dest_account)
// {
//     while(Validate::check_account_no(dest_account) == false)
//     {
//         cout << "Invalid Destination Account Number. Please Try Again!!!\n";
//         cout << "To account/card: ";
//         getline(cin, dest_account);
//     }
// }


// void CLI::check_account_balance(Account& a, int& amount)
// {
//     while(Validate::check_balance(a, amount) == false)
//     {
//         cout << "Insufficent Account Balance, Please Try Again!!!\n";
//         cout << "Enter Amount to Transfer: ";
//         cin >> amount;
//     }
// }

// bool CLI::check_password(User& u, string& password)
// {
//     while(Validate::verify(u, password) == false)
//     {
//         cout << "Wrong Password, Please Try Again!!!\n";
//         cout << " Password : ";

//         getline(cin, password);
//         cout << "\n---------------------------------\n";
//         cout << "Processing...\n"; 
//     }

// }

// void CLI::show_authenticate_before_trading(string& password)
// {
//     clear();
//     cout << "\n=====================================\n";
//     cout << "           TRANSFER MONEY\n";
//     cout << "=====================================\n\n";
//     cout << "-------------------------------------\n";

//     cout << " Password : ";
//     getline(cin, password);
// }

// bool CLI::show_transfer_interface(User& u, Account& a, string& dest_account, int& amount)
// {

//     cout << "\n=====================================\n";
//     cout << "           TRANSFER MONEY\n";
//     cout << "=====================================\n\n";
//     cout << "-------------------------------------\n";

//     cout << "Current Balance: VND" << fixed << setprecision(2) << a.balance << "\n\n";

//     cout << "To account/card: ";
//     getline(cin, dest_account);

//     cout << "Enter Amount to Transfer: ";
//     cin >> amount;
//     cin.ignore();

//     string password;
//     show_authenticate_before_trading(password);

//     cout << "\n---------------------------------\n";
//     cout << "Processing...\n"; //-> tạo 1 bước confirm (nhập lại password)

//     if(check_password(u, password) == true) //!!! nên check password bằng cách hash trực tiếp trên os rồi compare với SQL (k nên get vể)
//     {
//         return true;
//     }
//     return false;

// }

// void CLI::show_sender_not_exist_interface()
// {
//     clear();
//     cout << "=====================================\n";
//     cout << "            MINI BANKING  \n";
//     cout << "=====================================\n\n";
//     cout << "Transaction failed. Please check your information.\n";

//     cout << "-------------------------------------\n";
//     cout << " Press any key to continue...\n";
//     cin.ignore(); 
//     cin.get();     
//     clear();   
// }

// void CLI::show_receiver_not_exist_interface()
// {
//     clear();
//     cout << "=====================================\n";
//     cout << "            MINI BANKING  \n";
//     cout << "=====================================\n\n";
//     cout << "Tha Destination User Information does not exist. Try Again!!!\n";

//     cout << "-------------------------------------\n";
//     cout << " Press any key to continue...\n";
//     cin.ignore(); 
//     cin.get();     
//     clear();   
// }

// void CLI::show_receiver_not_available_interface()
// {
//     clear();
//     cout << "=====================================\n";
//     cout << "            MINI BANKING  \n";
//     cout << "=====================================\n\n";
//     cout << "Tha Destination User Information is Unavailable.. Try Again!!!\n";

//     cout << "-------------------------------------\n";
//     cout << " Press any key to continue...\n";
//     cin.ignore(); 
//     cin.get();     
//     clear();   
// }

// void CLI::handle_transfer(User& u, Account& a)
// {
//     string dest_account;
//     double amount;

//     Transaction_Info tran_in(dest_account, amount);

//     if(show_transfer_interface(u, a, dest_account, amount) == true)
//     {
//         AccountService as;
//         AccountService::TRANSFER t = as.create_transaction(u, a, dest_account, amount);

//         if(t == AccountService::TRANSFER::SENDER_NOT_EXIST)
//         {
//             show_sender_not_exist_interface();
//             show_main_custom_dashboard();

//         }
//         else if(t == AccountService::TRANSFER::RECEIVER_NOT_EXIST || t == AccountService::TRANSFER::RECEIVER_NOT_AVAILABLE)
//         {
//             //cout bussiness err and back về nhập lại destination user
//             if(t == AccountService::TRANSFER::RECEIVER_NOT_EXIST)
//             {
//                 show_receiver_not_exist_interface();

//             }


//         }
//         else if(t == AccountService::TRANSFER::SENDER_NOT_AVAILABLE)
//         {
//             //cout bussiness err and tùy mức độ cho back vể đúng nơi có thể bị văng khỏi app
//         }
//         else if(t == AccountService::TRANSFER::SUCCESS)
//         {
//             //in bill
//         }
//         else if(t ==  AccountService::TRANSFER::INSUFFICIENT_BALANCE)
//         {
//             //cout bussiness err and back về menu

//         }
//     }

//     cout << "Create Transaction Failed! Try Again!!!\n";

// }




void CLI::clear()
{
    system("cls");
}
