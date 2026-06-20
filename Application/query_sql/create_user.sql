SET NOCOUNT ON; --Remove nortification affected row

DECLARE @userid INT;

INSERT INTO Users(Full_Name, Phone_Number, Status_User) VALUES(?,?,?); 

SET @userid = SCOPE_IDENTITY();

INSERT INTO User_Security(UserID, Passwords, Salt) VALUES(@userid,?,?);

INSERT INTO User_Role(UserID, RoleID) VALUES(@userid,?);

SELECT @userid AS Result;