Create Database Test_System

--1. Users
Create Table Users(
	UserID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	Full_Name VARCHAR(50) NOT NULL,
	Phone_Number VARCHAR(10) NOT NULL UNIQUE, -- (+84)...
	Created_At DATETIME NOT NULL DEFAULT GETDATE(),
	Status_User VARCHAR(15) NOT NULL CONSTRAINT CHECK_STATUS_USER CHECK(Status_User IN ('ACTIVE', 'CLOSED', 'LOCKED', 'SUSPENDED')), -- if blocked bu?c ph?i sync v?i permission ?? block transfer
	Session_Version INT NOT NULL DEFAULT 1
);

--2. Account
Create Table Account(
	AccountID INT NOT NULL IDENTITY(1,1) PRIMARY KEY, 
	UserID INT NOT NULL,
	Account_NO VARCHAR(20) NOT NULL Unique,
	Balance DECIMAL(19,4) NOT NULL DEFAULT 0 CHECK (Balance >= 0),
	Created_At DATETIME DEFAULT GETDATE(),
	Type_Account VARCHAR(25) NOT NULL CONSTRAINT CHECK_TYPE_ACCOUNT CHECK(Type_Account IN ('CURRENT', 'SAVING', 'CREDIT', 'BUSINESS', 'ADMIN')), --Current, Saving, Credit, Business
	Status_Account VARCHAR(25) NOT NULL CONSTRAINT CHECK_STATUS_ACCOUNT CHECK(Status_Account IN ('ACTIVE', 'CLOSED', 'LOCKED', 'SUSPENDED')), --Active, Closed, Suspended
	FOREIGN KEY (UserID) REFERENCES Users(UserID) 
);
--2. Roles
Create Table Roles(
	RoleID INT NOT NULL	IDENTITY(1,1) PRIMARY KEY,
	Role_Name VARCHAR(20) NOT NULL CONSTRAINT CHECK_ROLE_NAME CHECK(Role_Name IN ('ADMINISTRATOR', 'USER'))
);

--3. Permission
--Create Table Permission(
--	PermissionID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
--	Permissions_Name VARCHAR(20) NOT NULL CONSTRAINT CHECK_PERMISSION_NAME CHECK(Permissions_Name IN ('TRANSFER', 'DELETE_ACCOUNT', 'CREATE_ACCOUNT', 'MODIFY_USER', 'FULL_CONTROL'))
--);

--4. User_Role
Create Table User_Role(
	UserID INT NOT NULL,
	RoleID INT NOT NULL,
	PRIMARY KEY (UserID, RoleID),
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (RoleID) REFERENCES Roles(RoleID)
);

--5. Role_Permission (default permission for each role)
--Create Table Role_Permission(
--	RoleID INT NOT NULL,
--	PermissionID INT NOT NULL,
--	PRIMARY KEY (PermissionID, RoleID),
--	FOREIGN KEY (PermissionID) REFERENCES Permission(PermissionID),
--	FOREIGN KEY (RoleID) REFERENCES Roles(RoleID)
--);

--6. User_Permission
--Create Table User_Permission(
--	UserID INT NOT NULL,
--	PermissionID INT NOT NULL,
--	Available BIT NOT NULL,
--	PRIMARY KEY (PermissionID, UserID),
--	FOREIGN KEY (PermissionID) REFERENCES Permission(PermissionID),
--	FOREIGN KEY (UserID) REFERENCES Users(UserID)
--);

--7. Account


--8. Auth_Logs
Create Table Auth_Logs(
	LoginID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	Login_Time DateTime NOT NULL DEFAULT GETDATE(),
	Action_Log VARCHAR(20) NOT NULL CONSTRAINT CHECK_Action_Log CHECK (Action_Log IN ('REGISTER', 'LOGIN', 'LOGOUT')),
	Status_Log VARCHAR(10) NOT NULL CONSTRAINT CHECK_LOG CHECK (Status_Log IN ('SUCCESS', 'FAILED', 'DENIED')),
	Detail VARCHAR(20) NOT NULL CONSTRAINT CHECK_DETAIL CHECK (Detail IN ('USER_CREATED', 'ACCOUNT_CREATED', 'SYSTEM_ERROR', 'USER_NOT_FOUND', 'ACCOUNT_NOT_FOUND', 'WRONG_PASSWORD', 'LOGIN_SUCCESSFUL')),
	UserID INT NULL, 
	Target_Account_ID INT NULL,
	IP_Address VARCHAR(45) NULL,
	FOREIGN KEY (UserID) REFERENCES Users(UserID)
);

--Trigger to fill IP when 1 User login
CREATE TRIGGER Set_IP_Address
ON Auth_Logs AFTER INSERT AS 
BEGIN UPDATE a
SET IP_Address = ISNULL(c.client_net_address, 'UNKNOWN')
FROM Auth_Logs a
JOIN inserted i ON a.LoginID = i.LoginID -- Donn't use UserID because one user can log multiple times
CROSS JOIN sys.dm_exec_connections c
WHERE c.session_id = @@SPID;
END;

--sequence
CREATE SEQUENCE dbo.Account_NO 
	AS BIGINT 
	START WITH 0
	INCREMENT BY 1
	NO CYCLE;

--9. Transaction 
Create Table Transactions(
	TransactionID INT NOT NULL IDENTITY(1,1) Primary Key,
	Transaction_Code VARCHAR(12) NOT NULL UNIQUE,
	At_The_Time DATETIME NOT NULL DEFAULT GETDATE(),
	Type_Transaction VARCHAR(25) NOT NULL CONSTRAINT CHECK_TYPE_TRANSACTION CHECK (Type_Transaction IN ('TRANSFER', 'PAYMENT')),
	From_Account_ID INT NOT NULL,
	To_Account_ID INT NOT NULL,
	Transaction_Amount Decimal(19,4) NOT NULL,
	Status_Transaction VARCHAR(25) NOT NULL CONSTRAINT CHECK_STATUS_TRANSACTION CHECK (Status_Transaction IN ('PENDING', 'SUCCESS', 'FAILED')),
	FOREIGN KEY (From_Account_ID) REFERENCES Account(AccountID),
	FOREIGN KEY (To_Account_ID) REFERENCES Account(AccountID)
);

--10. Account_Transaction
Create Table Account_Transaction(
	TransactionID INT NOT NULL,
	AccountID INT NOT NULL,
	PRIMARY KEY (TransactionID, AccountID),
	FOREIGN KEY (TransactionID) REFERENCES Transactions(TransactionID),
	FOREIGN KEY (AccountID) REFERENCES Account(AccountID)
);

--11. Security_Logs
Create Table Security_Logs(
	LogID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	Levels VARCHAR(10) NOT NULL CONSTRAINT CHECK_LEVELS_TYPE CHECK(Levels IN('INFO', 'WARNING', 'ALERT', 'CRITICAL')),
	Event_Time DATETIME NOT NULL DEFAULT GETDATE(),
	Detail_Event NVARCHAR(MAX) NOT NULL,
	Event_Type VARCHAR(25) NOT NULL CONSTRAINT CHECK_EVENT_TYPE CHECK(Event_Type IN('LOGIN_SUCCESS', 'LOGIN_FAILED', 'BRUTE_FORCE', 'SQL_INJECTION', 'PASSWORD_CHANGE')),
	UserID INT NOT NULL, 
	Account_ID INT NOT NULL,
	Result VARCHAR(10) NOT NULL CONSTRAINT CHECK_RESULT CHECK (Result IN ('PENDING', 'SUCCESS', 'FAILED', 'ERROR')),
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (Account_ID) REFERENCES Account(AccountID)

);

--12. User_Activity
Create Table User_Activity(
	ActivityID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	At_The_Time  DateTime NOT NULL DEFAULT GETDATE(),
	Activity_Type VARCHAR(25) NOT NULL CONSTRAINT CHECK_ACTIVITY_TYPE CHECK(Activity_Type IN('CREATE', 'DELETE', 'REGISTER')),
	Status_Activity VARCHAR(10) NOT NULL CONSTRAINT CHECK_ACTIVITY_STATUS CHECK (Status_Activity IN ('SUCCESS', 'FAILED')),
	Detail_Event VARCHAR(20) NOT NULL,
	UserID INT NOT NULL, 
	Target_Account_ID INT NULL,
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (Target_Account_ID) REFERENCES Account(AccountID)

);

--13. User_Security
Create Table User_Security(
	UserID INT NOT NULL PRIMARY KEY,
	Passwords VARBINARY(32) NOT NULL,
	Salt VARBINARY(16) NOT NULL, 
	FOREIGN KEY (UserID) REFERENCES Users(UserID)
);

--Create Admin 
INSERT INTO Users(Full_Name, Phone_Number, Status_User) VALUES ('ADMIN', '0999999999', 'ACTIVE');
INSERT INTO User_Security(UserID, Passwords, Salt) VALUES (1, 0x9EF1418D090C0B60A8CD06D81CC2553C8A805D767FEEBDCDD42B136B4A114F3C, 0x1E0E58D3F7523F650CB696CFAB159C66);
INSERT INTO Account(UserID, Account_NO, Balance, Type_Account, Status_Account) VALUES (1, '111111111111', 0, 'ADMIN', 'ACTIVE');
UPDATE Account SET Balance = 100000 WHERE Account_NO = 111111111111;
INSERT INTO Roles(Role_Name) VALUES ('ADMINISTRATOR');
INSERT INTO Roles(Role_Name) VALUES ('USER');
INSERT INTO User_Role(RoleID, UserID) VALUES (1, 1);

--INSERT INTO Permission(Permissions_Name) VALUES ('FULL_CONTROL');
--INSERT INTO Permission(Permissions_Name) VALUES ('TRANSFER');
--INSERT INTO Permission(Permissions_Name) VALUES ('DELETE_ACCOUNT');
--INSERT INTO Permission(Permissions_Name) VALUES ('CREATE_ACCOUNT');
--INSERT INTO Permission(Permissions_Name) VALUES ('MODIFY_USER');



SELECT * FROM Users;
SELECT * FROM Account;
SELECT * FROM Roles;
--SELECT * FROM Permission;
SELECT * FROM User_Role;
--SELECT * FROM Role_Permission;
--SELECT * FROM User_Permission;
SELECT * FROM Auth_Logs;
SELECT * FROM User_Security;
SELECT * FROM Security_Logs;
SELECT * FROM User_Activity;
SELECT * FROM Transactions;
SELECT * FROM Account_Transaction;

DELETE FROM Users;
DBCC CHECKIDENT ('Users', RESEED, 0)

DELETE FROM Account;
DBCC CHECKIDENT ('Account', RESEED, 0)

DELETE FROM User_Activity;
DBCC CHECKIDENT ('User_Activity', RESEED, 0)

DELETE FROM User_Security;
DBCC CHECKIDENT ('User_Security', RESEED, 0)

DELETE FROM Auth_Logs;
DBCC CHECKIDENT ('Auth_Logs', RESEED, 0)

ALTER TABLE Auth_Logs
ADD Detail NVARCHAR(MAX) NOT NULL;

BEGIN TRANSACTION; BEGIN TRY INSERT INTO Transactions(Type_Transaction, From_Account_ID, To_Account_ID, Transaction_Amount, Status_Transaction) OUPUT inserted.TransactionID INTO @u1 VALUES (?,?,?,?,?)