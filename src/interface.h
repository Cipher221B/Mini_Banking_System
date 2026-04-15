#pragma once

#include <iostream>
using namespace std;
#include "manager_user.h"
#include "manager_account.h"
#include "auth_service.h"
#include "session.h"


class CLI
{
    private:
        enum class State
        {
            MAIN_MENU,
            REGISTER,
            LOGIN,
            ADMIN_MENU,
            CUSTOM_MENU,
            EXIT
        };

        enum class State_Admin_DashBoard
        {
            ADMIN_MENU,
            VIEW_USER,
            SET_PERMISSION_USER,
            DELETE_ACCOUNT_CUSTOM,
            LOG_OUT
        };

        enum class State_Custom_DashBoard
        {
            CUSTOM_MENU,
            TRANSFER,
            VIEW_LOG_TRANSACTION,
            CREATE_ACCOUNT,
            LOG_OUT

        };
    
        State pr_state;

    public:

        //main handle
        void main_control();

        //maindashboard admin api
        void show_admin_dashboard();
        State_Admin_DashBoard admin_activity();
        State main_control_admin_dash_board(User& u, Account& a, Session& s);

        //maindashboard custom api
        void show_custom_dashboard(User& u, Account& a);
        void check_account_no(string& dest_account);
        void check_account_balance(Account& a, int& amount);
        bool check_password(User& u, string& password);

        State_Custom_DashBoard custom_activity(User& u, Account& a);
        State main_control_custom_dash_board(User& u, Account& a, Session& s);
        

        //start interface api  
        State activity();
        void show_start_interface();
        void show_end_interface();
        void clear();

        //helper interface api
        void show_failed_interface(const string& center_message, const string& message);

        //transfer interface api
        void handle_transfer(User& u, Account& a);
        bool show_transfer_interface(Account& a, string& dest_account, int& amount);
        void show_authenticate_before_trading(string& password);
        void show_sender_not_exist_interface();
        void show_receiver_not_exist_interface();
        void show_receiver_not_available_interface();

        // register interface api
        void show_register_success_interface(User& u);
        void show_register_interface();
        string input_phone();
        string input_password();
        string input_full_name();
        void create_full_name(User& u);
        void create_phone(User& u);
        void create_password(User& u);
        int input_type_account();
        void account_information(Account& a);
        State handle_register(User& u, Account& a);

        //login interface api
        void show_login_interface(string& phone_number, string& password);
        State define_state_login(User& u, Account& a, AuthService::LOGIN l);
        State handle_login(Session& s, User& u, Account& a);
        
};