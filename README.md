# 🏦 MiniBanking SIEM Lab
(Simulating banking operations, authentication flows, and security telemetry for SIEM correlation and analysis)   
   
## 📌 Project Overview   

Mini Banking is a CLI-based banking simulation system
built with C++ and connected to a local SQL Server
through ODBC.

The project focuses on:

- Authentication flow (registration, login, session control, and validation before sensitive actions).

- Authorization flow (simulated through user/account state management).

- Transaction simulation (including transfer operations and activity tracking).

- Security logging and telemetry generation (authentication logs, transaction logs, activity logs, and system error logging).

- Role-based operations (user/admin functionality separation).

- Simulated IP address generation for telemetry and SIEM-oriented analysis.   
   
## 🎯 Core Goal   

- Simulate authentication and transaction behavior in a CLI banking environment

- Generate security-related telemetry for logging and monitoring purposes.

- Model session validation and role-based authorization.

- Support attack simulation and SIEM-oriented security analysis.

- Explore security engineering concepts through a lightweight banking system.   
   
## 📍 Main Feature    
   
- [x] User registration.
- [x] Login and logout system.
- [x] Session-based authentication (validation/check) before sensitive actions.
- [x] The operations in the user interface include:   
    - Password change functionality.   
    - Banking transaction simulation (transfer operations).   
- [x] Administrative controls (change user/account status).   
- [x] Authentication, Activity, Transaction and System Error logging.
   
## 🏗️ Architecture Overview   

The project follows a modular architecture with separation between:

- 🖥️ `Interface Layer` Handles CLI menus, user interaction and screen rendering.

- ⚙️ `Service Layer` Handles business logic such as authentication, transactions, authorization checks, and session validation.

- 🗄️ `Repository Layer` Handles database communication and SQL operations through ODBC.

Additionally, the project includes supporting modules for:

- 🔒 Security utilities (password hashing, salt generation, random data generation. For example: account number, transaction code, IP address generation/simulation to support consistency in simulating scenarios.).

- 🛢️ Database connection management.

- 📝 Logging and file handling.

- ⌨️ Input Verification.   
   
## 📐 Project Structure   
   
```text
Mini_Banking
|   ip_profile.txt
|   Makefile
|   README.md
|   system.log
|
+---docs
|   |   README.md
|   |
|   +---erd
|   |       erd.png
|   |
|   +---flowcharts
|   |       change_password_flow.png
|   |       change_status_user_flow.png
|   |       login_flow.png
|   |       transfer_flow.png
|   |
|   \---screenshots
|           admin_menu.png
|           change_password_success_interface.png
|           change_status_user_success_interface.png
|           custom_menu.png
|           telemetry_logs.png
|           transfer_success_interface.png
|
+---query_sql
|       create_user.sql
|       query_transaction.sql
|
+---sql
|       Mini_Banking.sql
|
\---src
    |   interface.cpp
    |   interface.h
    |   main.cpp
    |
    +---database
    |       db_connection.cpp
    |       db_connection.h
    |
    +---error
    |       database_errors.h
    |       layer_error.h
    |       system_errors.h
    |
    +---helper
    |       db_helper.cpp
    |       db_helper.h
    |       file_handle.cpp
    |       file_handle.h
    |       input_validation.cpp
    |       input_validation.h
    |       log_service.cpp
    |       log_service.h
    |       odbc_helper_error.cpp
    |       odbc_helper_error.h
    |       string_utils.cpp
    |       string_utils.h
    |
    +---identity
    |       account.cpp
    |       account.h
    |       session.cpp
    |       session.h
    |       user.cpp
    |       user.h
    |
    +---log
    |       audit_log.cpp
    |       audit_log.h
    |       system_log.cpp
    |       system_log.h
    |
    +---repository
    |       account_repository.cpp
    |       account_repository.h
    |       user_repository.cpp
    |       user_repository.h
    |
    +---security
    |       hash.cpp
    |       hash.h
    |       random_generation.cpp
    |       random_generation.h
    |
    +---service
    |       account_service.cpp
    |       account_service.h
    |       auth_service.cpp
    |       auth_service.h
    |
    \---storage
            sql_error.h
            transaction_information.h
```   
   
## 🗃️ Database Design   
   
| Table | Responsibility |
| ------------ | ------------------------------------------------ |
| `Users` | Identity management |
| `Accounts` | Banking account data |
| `Roles` | User classification |  
| `User_Role` | Intermediate table mapping from users to roles |
| `Auth_Logs` | Store authentication-related information | 
| `Transactions` | Transfer Activity | 
| `Activity_Logs` | Record user, account activities | 
| `User_Security` | Sensitive authentication information |    
   
## 🔒 Security Concepts   
   
The project includes several security-oriented mechanisms to simulate authentication, authorization, and telemetry generation within a banking environment.
### Implemented Concepts   

- Password hashing with PBKDF2 and salt generation.

- Session-based validation/check before sensitive operations.

- Role/state-based authorization through user/account status management.

- Authentication telemetry logging (successful, failed login and multiple state activity).

- Transaction and activity logging.

- Simulated IP address generation for SIEM-oriented analysis.

- System error logging for operational monitoring and debugging.   
   
## 🛠️ Build & Run   
### Requirements

- Windows environment
- MSYS2 / MinGW64
- SQL Server
- ODBC Driver
- OpenSSL

### Build

```bash
mingw32-make
```

### Run

```bash
mingw32-make run
```

### Database Setup

Run the SQL scripts inside:

```text
/sql
```   
   
## ⚠️ Limitations   

❌ Simple CLI interface.   

❌ Single-threaded architecture.  

❌ No Web/UI Layer for better user interaction.   

❌ 1 user <-> 1 account model.   

❌ IP generation simulations have many limitations when simulating attack scenarios and lack flexibility.   

❌ Local SQL Server deployment.   

❌ The code structure may not be clear, but the design is standard, optimized, and as clean as possible.   

❌ It doesn't have many features yet, a more realistic bank simulation would provide more exploitable options.   

   
## 🚀 Future Improvements   

🔧 Web/UI interface.   
   
🔧 Multiple thread.   
   
🔧 1 user <-> multiple account.   

🔧 Real Network telemetry.   
   
🔧 Adding more features can create more exploitation opportunities.   
   
