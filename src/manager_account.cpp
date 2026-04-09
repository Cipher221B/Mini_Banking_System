#include "manager_account.h"
#include "random_generation.h"
#include <stdexcept>

void Account::create_account()
{
    int choose;
    cout << "Type Account?\n";
    cout << "1. Current Account!\n";
    cout << "2. Saving Account\n";
    cout << "3. Credit Account?\n";
    cout << "4. Business Account?\n";
    cout << "Enter (1-4): ";
    cin >> choose;
    cin.ignore();
    
    if(set_type_account(choose) == false)
    {
        cout << "Error! Try Again!!!\n";
        create_account();
    }

    status = "ACTIVE";

}

bool Account::set_type_account(int choose)
{
    switch(choose)
    {
        case 1:
        {
            type = "CURRENT";
            break;

        }
        case 2:
        {
            type = "SAVING";
            break;

        }
        case 3:
        {
            type = "CREDIT";
            break;

        }
        case 4:
        {
            type = "BUSINESS";
            break;

        }
        default:
        {
            return false;
        }
    }

    return true;

}
