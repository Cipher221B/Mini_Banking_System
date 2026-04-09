BEGIN TRY
    BEGIN TRANSACTION; 
    
        DECLARE @transactionid INT;
		DECLARE @sender_account_no VARCHAR(20);
		DECLARE @receiver_account_no VARCHAR(20);

        DECLARE @temp_table TABLE (
                transactionid INT;
                sender_account_no VARCHAR(20);
                receiver_account_no VARCHAR(20);
        );

        INSERT INTO Transactions(Type_Transaction, From_Account_ID, To_Account_ID, Transaction_Amount, Status_Transaction) 
        OUPUT inserted.TransactionID, inserted.From_Account_ID, inserted.To_Account_ID INTO @tenp_table VALUES (?,?,?,?,?); 

        SELECT @transactionid = transactionid,
               @sender_account_no = sender_account_no,
               @receiver_account_no = receiver_account_no 
        FROM @temp_table;

        INSERT INTO Account_Transaction(TransactionID, AccountID) VALUES (@transactionid, @sender_account_no); 
        INSERT INTO Account_Transaction(TransactionID, AccountID) VALUES (@transactionid, @receiver_account_no); 
        
        
    COMMIT; 

END TRY 

BEGIN CATCH 
    ROLLBACK; 
	THROW; --Ensure Query return error to odbc

END CATCH;