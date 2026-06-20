Create Database Test_System

--1. Users
Create Table Users(
	UserID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	Full_Name VARCHAR(50) NOT NULL,
	Phone_Number VARCHAR(10) NOT NULL UNIQUE, -- (+84)...
	Created_At DATETIME2 NOT NULL DEFAULT GETDATE(),
	Status_User VARCHAR(15) NOT NULL CONSTRAINT CHECK_STATUS_USER CHECK(Status_User IN ('ACTIVE', 'CLOSED', 'LOCKED', 'SUSPENDED')), -- if blocked bu?c ph?i sync v?i permission ?? block transfer
	Session_Version INT NOT NULL DEFAULT 1
);

--2. Account
Create Table Accounts(
	AccountID INT NOT NULL IDENTITY(1,1) PRIMARY KEY, 
	UserID INT NOT NULL,
	Account_Number VARCHAR(20) NOT NULL UNIQUE,
	Balance DECIMAL(19,4) NOT NULL DEFAULT 0 CHECK (Balance >= 0),
	Created_At DATETIME2 DEFAULT GETDATE(),
	Type_Account VARCHAR(25) NOT NULL CONSTRAINT CHECK_TYPE_ACCOUNT CHECK(Type_Account IN ('CURRENT','BUSINESS', 'ADMIN')), --Current, Saving, Credit, Business
	Status_Account VARCHAR(15) NOT NULL CONSTRAINT CHECK_STATUS_ACCOUNT CHECK(Status_Account IN ('ACTIVE', 'CLOSED', 'LOCKED', 'SUSPENDED')), --Active, Closed, Suspended
	FOREIGN KEY (UserID) REFERENCES Users(UserID) 
);
--3. Roles
Create Table Roles(
	RoleID INT NOT NULL	IDENTITY(1,1) PRIMARY KEY,
	Role_Name VARCHAR(20) NOT NULL UNIQUE CONSTRAINT CHECK_ROLE_NAME CHECK(Role_Name IN ('ADMINISTRATOR', 'USER'))
);


--4. User_Role
Create Table User_Role(
	UserID INT NOT NULL,
	RoleID INT NOT NULL,
	PRIMARY KEY (UserID, RoleID),
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (RoleID) REFERENCES Roles(RoleID)
);


--5. Auth_Logs
Create Table Auth_Logs(
	LoginID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	At_The_Time DATETIME2 NOT NULL DEFAULT GETDATE(),
	Action_Log VARCHAR(20) NOT NULL CONSTRAINT CHECK_Action_Log CHECK (Action_Log IN ('REGISTER', 'LOGIN', 'LOGOUT')),
	Status_Log VARCHAR(10) NOT NULL CONSTRAINT CHECK_LOG CHECK (Status_Log IN ('SUCCESS', 'FAILED', 'DENIED')),
	Detail VARCHAR(25) NOT NULL CONSTRAINT CHECK_DETAIL CHECK (Detail IN ('USER_CREATED', 'ACCOUNT_CREATED', 'USER_CLOSED', 'USER_LOCKED', 'USER_SUSPENDED', 'USER_NOT_FOUND', 'ACCOUNT_NOT_FOUND', 'WRONG_PASSWORD', 'LOGIN_SUCCESSFUL', 'SESSION_VERSION_EXPIRED', 'USER_LOGGED_OUT')),
	UserID INT NULL, 
	Target_Account_ID INT NULL,
	IP_Address VARCHAR(45) NOT NULL,
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (Target_Account_ID) REFERENCES Accounts(AccountID)
);

--sequence
CREATE SEQUENCE dbo.Account_NO 
	AS BIGINT 
	START WITH 0
	INCREMENT BY 1
	NO CYCLE;

--6. Transaction 
Create Table Transactions(
	TransactionID INT NOT NULL IDENTITY(1,1) Primary Key,
	Transaction_Code VARCHAR(25) NOT NULL UNIQUE,
	At_The_Time DATETIME2 NOT NULL DEFAULT GETDATE(),
	Type_Transaction VARCHAR(25) NOT NULL CONSTRAINT CHECK_TYPE_TRANSACTION CHECK (Type_Transaction IN ('TRANSFER')),
	From_Account_ID INT NOT NULL,
	To_Account_ID INT NOT NULL,
	Transaction_Amount DECIMAL(19,4) NOT NULL,
	Status_Transaction VARCHAR(10) NOT NULL CONSTRAINT CHECK_STATUS_TRANSACTION CHECK (Status_Transaction IN ('PENDING', 'SUCCESS', 'FAILED')),
	FOREIGN KEY (From_Account_ID) REFERENCES Accounts(AccountID),
	FOREIGN KEY (To_Account_ID) REFERENCES Accounts(AccountID)
);

--7. User_Activity
Create Table Activity_Logs(
	ActivityID INT NOT NULL IDENTITY(1,1) PRIMARY KEY,
	At_The_Time DATETIME2 NOT NULL DEFAULT GETDATE(),
	Activity_Type VARCHAR(25) NOT NULL CONSTRAINT CHECK_ACTIVITY_TYPE CHECK(Activity_Type IN('CHANGE_PASSWORD', 'CHANGE_STATUS_USER', 'CHANGE_STATUS_ACCOUNT')),
	Status_Activity VARCHAR(10) NOT NULL CONSTRAINT CHECK_ACTIVITY_STATUS CHECK (Status_Activity IN ('SUCCESS', 'FAILED')),
	Detail_Event VARCHAR(255) NOT NULL,
	UserID INT NOT NULL, 
	Target_User_ID INT NULL,
	Target_Account_ID INT NULL,
	FOREIGN KEY (UserID) REFERENCES Users(UserID),
	FOREIGN KEY (Target_User_ID) REFERENCES Users(UserID),
	FOREIGN KEY (Target_Account_ID) REFERENCES Accounts(AccountID)

);

--8. User_Security
Create Table User_Security(
	UserID INT NOT NULL PRIMARY KEY,
	Passwords VARBINARY(32) NOT NULL,
	Salt VARBINARY(16) NOT NULL, 
	FOREIGN KEY (UserID) REFERENCES Users(UserID)
);

--9. Collector_CheckPoint
Create Table Collector_CheckPoint(
	CategoryID INT NOT NULL PRIMARY KEY,
	Category_Name VARCHAR(20) NOT NULL,
	Last_ID_From_Last_Read INT NOT NULL, 
);

--Create Admin 
INSERT INTO Users(Full_Name, Phone_Number, Status_User) VALUES ('ADMIN', '0999999999', 'ACTIVE');
INSERT INTO User_Security(UserID, Passwords, Salt) VALUES (1, 0x9EF1418D090C0B60A8CD06D81CC2553C8A805D767FEEBDCDD42B136B4A114F3C, 0x1E0E58D3F7523F650CB696CFAB159C66);
INSERT INTO Accounts(UserID, Account_Number, Balance, Type_Account, Status_Account) VALUES (1, '111111111111', 0, 'ADMIN', 'ACTIVE');
UPDATE Accounts SET Balance = 100000 WHERE Account_Number = 111111111111;
INSERT INTO Roles(Role_Name) VALUES ('ADMINISTRATOR');
INSERT INTO Roles(Role_Name) VALUES ('USER');
INSERT INTO User_Role(RoleID, UserID) VALUES (1, 1);
INSERT INTO Collector_CheckPoint(CategoryID, Category_Name, Last_ID_From_Last_Read) VALUES (1, 'Authentication',0);
INSERT INTO Collector_CheckPoint(CategoryID, Category_Name, Last_ID_From_Last_Read) VALUES (2, 'Activity',0);
INSERT INTO Collector_CheckPoint(CategoryID, Category_Name, Last_ID_From_Last_Read) VALUES (3, 'Transaction',0);

--Display Table
SELECT * FROM Users;
SELECT * FROM Accounts;
SELECT * FROM Roles;
SELECT * FROM User_Role;
SELECT * FROM User_Security;
SELECT * FROM Auth_Logs;
SELECT * FROM Transactions;
SELECT * FROM Activity_Logs;
SELECT * FROM Collector_CheckPoint;
 
--test case
UPDATE Accounts SET Balance = 230000 WHERE Account_Number = 920000000000;
UPDATE Users SET Status_User = 'ACTIVE' WHERE UserID = 2;
UPDATE Account SET Status_Account = 'ACTIVE' WHERE UserID = 1;
UPDATE Collector_CheckPoint SET Last_ID_From_Last_Read = '0' WHERE CategoryID = 1;
UPDATE Collector_CheckPoint SET Last_ID_From_Last_Read = '0' WHERE CategoryID = 2;
UPDATE Collector_CheckPoint SET Last_ID_From_Last_Read = '0' WHERE CategoryID = 3;

DROP TABLE Collector_CheckPoint;