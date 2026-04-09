BEGIN TRY
	BEGIN TRANSACTION;

		DECLARE @sender_account_no VARCHAR(20);
		DECLARE @receiver_account_no VARCHAR(20);
		DECLARE @balance DECIMAL;
		DECLARE @amount DECIMAL;

		SET @sender_account_no = ?;
		SET @receiver_account_no = ?;
		SET @amount = ?;


		-- LOCK SENDER AND CHECK EXIST ACCOUNT
		SELECT @balance = Balance 
		FROM Account WITH (UPDLOCK, ROWLOCK)
		WHERE Account_NO = @sender_account_no;

		IF @@ROWCOUNT = 0
		BEGIN 
			ROLLBACK;
		    SELECT 'SENDER_NOT_EXIST' AS Result;
			RETURN;
		END

		-- LOCK RECEIVER AND CHECK EXIST ACCOUNT
		SELECT 1 
		FROM Account WITH (UPDLOCK, ROWLOCK)
		WHERE Account_NO = @receiver_account_no;

		IF @@ROWCOUNT = 0
		BEGIN 
			ROLLBACK;
			SELECT 'RECEIVER_NOT_EXIST' AS Result;
			RETURN;
			
		END

		--CHECK STATUS USER SENDER
		IF NOT EXISTS(
			SELECT 1 FROM Account a
			JOIN Users u ON a.UserID = u.UserID
			WHERE a.Account_NO = @sender_account_no
			AND u.Status_User = 'ACTIVE'
			AND a.Status_Account = 'ACTIVE'
		)
		BEGIN 
			ROLLBACK;
			SELECT 'SENDER_NOT_AVAILABLE' AS Result;
			RETURN;


		END

		--CHECK STATUS USER RECEIVER
		IF NOT EXISTS(
			SELECT 1 FROM Account a
			JOIN Users u ON a.UserID = u.UserID
			WHERE a.Account_NO = @receiver_account_no
			AND u.Status_User = 'ACTIVE'
			AND a.Status_Account = 'ACTIVE'
		)
		BEGIN 
			ROLLBACK;
			SELECT 'RECEIVER_NOT_AVAILABLE' AS Result;
			RETURN;

		END

		--CHECK BALANCE
		IF @balance < @amount
		BEGIN 
			ROLLBACK;
			SELECT 'INSUFFICIENT_FUND' AS Result
			RETURN;
		
		END

		--MINUS FOR SENDER
		UPDATE Account 
		SET Balance = Balance - @amount
		WHERE Account_NO = @sender_account_no;

		--PLUS FOR RECEIVER
		UPDATE Account
		SET Balance = Balance + @amount
		WHERE Account_NO = @receiver_acount_no;

	COMMIT;
		SELECT 'SUCCESS' AS Result;
		RETURN;

END TRY

BEGIN CATCH 
	ROLLBACK;
	SELECT 'SYSTEM_ERROR' AS Result;
	THROW; --Ensure Query return error to odbc

END CATCH
