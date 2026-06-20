#pragma once
#include <iostream>
#include "entity/user.h"
#include "entity/account.h"
#include "service/auth_service.h"
#include "service/account_service.h"
#include "entity/session.h"
#include "data/transaction_information.h"

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
            CHANGE_STATUS_USER,
            CHANGE_STATUS_ACCOUNT,
            CHANGE_PASSWORD,
            ENFORCED_LOG_OUT,
            LOG_OUT
        };

        enum class State_Custom_DashBoard
        {
            CUSTOM_MENU,
            TRANSFER,
            CHANGE_PASSWORD,
            ENFORCED_LOG_OUT,
            LOG_OUT 
        };
    
        State pr_state;

    public:

        //helper interface api
        void show_failed_interface(const std::string& center_message, const std::string& message);
        void show_authenticate_before_sensitive_action(const std::string& center_message, std::string& p);
        int show_handle_next_action(const std::string& option_message);
        void print_center(const std::string& text, int width);

        //logout interface api
        State handle_logout(Session& s, User& u, Account& a, bool enforced_logout);

        //refresh api
        State_Admin_DashBoard admin_refresh(LOAD_DATA ld);
        State_Custom_DashBoard custom_refresh(LOAD_DATA ld);

        //main handle
        void main_control();

        //change status account
        void show_option_to_change_status_account(std::vector<std::string>& opt_st, Status_Account_Info& sai);
        std::string input_new_status_account(Status_Account_Info& sai);
        void show_change_status_account_interface(std::string& acc_no);
        State_Admin_DashBoard handle_change_status_account(Session& s, User& u, Account& a);

        //change status user
        void show_change_status_user_interface(std::string& p);
        void show_option_to_change_status_user(std::vector<std::string>& opt_st, Status_User_Info& sui);
        std::string input_new_status_user(Status_User_Info& sui);
        State_Admin_DashBoard handle_change_status_user(Session& s, User& u, Account& a);

        // set permission
        void show_change_status_object_success_interface(const std::string& center_message, const std::string& message);

        //maindashboard admin api
        void show_admin_dashboard();
        State_Admin_DashBoard admin_activity();
        State main_control_admin_dash_board(User& u, Account& a, Session& s);

        //maindashboard custom api
        void show_custom_dashboard(User& u, Account& a);
        void check_account_no(std::string& dest_account);
        void check_account_balance(Account& a, int& amount);
        bool check_password(User& u, std::string& password);
        State_Custom_DashBoard custom_activity(User& u, Account& a);
        State main_control_custom_dash_board(User& u, Account& a, Session& s);
        
        //start interface api  
        State activity();
        void show_start_interface();
        void show_end_interface();
        void clear();

        //transfer interface api
        State_Custom_DashBoard handle_transfer(Session& s, User& u, Account& a);
        void show_transfer_interface(Account& a, std::string& dest_account, std::string& p, double& amount);
        void show_transfer_success_interface(Transaction_Info& t, User& u, Account& a);

        // register interface api
        void show_register_success_interface(User& u);
        void show_register_interface();
        std::string user_input(const std::string& message);
        std::string input_type_account();
        void account_information(Account& a);
        State handle_register(User& u, Account& a);

        template<typename Func>
        void retry_until_valid(Func f)
        {
            try
            {
                show_register_interface();
                f();
            }
            catch(const Invalid_Input& e)
            {
                std::string error_message = std::string(e.what()) + " Try Again!!!";
                show_failed_interface("INVALID INFORMATION", error_message);
                retry_until_valid(f);
            }
        }

        //login interface api
        void show_login_interface(std::string& phone_number, std::string& password);
        State define_state_login(Session& s, User& u, Account& a, LOGIN l);
        State handle_login(Session& s, User& u, Account& a);
        
        // change password 
        void show_change_password_successful_interface();
        void show_change_password_interface(std::string& new_password, std::string& confirm_new_password);
        State_Custom_DashBoard handle_change_password_user(Session& s, User& u, Account& a);
        State_Admin_DashBoard handle_change_password_admin(Session& s, User& u, Account& a);
        
        
        
};