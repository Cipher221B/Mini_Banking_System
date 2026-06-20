BEGIN TRY

	DECLARE @sender_account_id INT;
	DECLARE @receiver_account_id INT;
	DECLARE @balance DECIMAL(19, 4);
	DECLARE @amount DECIMAL(19, 4);

	SET @sender_account_id = ?;
	SET @receiver_account_id = ?;
	SET @amount = ?;

	-- LOCK SENDER
	SELECT @balance = Balance 
	FROM Accounts WITH (UPDLOCK, ROWLOCK)
	WHERE AccountID = @sender_account_id;

	-- LOCK RECEIVER
	IF EXISTS (
    	SELECT 1 
   	 	FROM Accounts WITH (UPDLOCK, ROWLOCK)
    	WHERE AccountID = @receiver_account_id
	)
	
	--CHECK STATUS USER RECEIVER
	IF NOT EXISTS(
		SELECT 1 FROM Accounts a
		JOIN Users u ON a.UserID = u.UserID
		WHERE a.AccountID = @receiver_account_id
		AND u.Status_User = 'ACTIVE'
		AND a.Status_Account = 'ACTIVE'
	)
	BEGIN
		THROW 50001, 'RECEIVER_NOT_AVAILABLE', 1
	END

	-- CHECK BALANCE
	IF @balance < @amount
	BEGIN 
		THROW 50002, 'INSUFFICIENT_FUND', 1;
	END

	SELECT @sender_account_id AS s_account_id, @receiver_account_id AS r_account_id, @balance AS balance, @amount AS amount;


	-- UPDATE
	UPDATE Accounts 
	SET Balance = Balance - @amount
	WHERE AccountID = @sender_account_id;

	UPDATE Accounts
	SET Balance = Balance + @amount
	WHERE AccountID = @receiver_account_id;

END TRY

BEGIN CATCH
	THROW; -- đẩy lỗi ra ngoài C++
END CATCH