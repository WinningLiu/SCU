CREATE or REPLACE TRIGGER checkCustTypeChange
AFTER UPDATE ON Customer
FOR EACH ROW
BEGIN
    --Couldn't add after shipped date functionality
    IF :new.custType = 'gold' THEN
        UPDATE itemOrder
        SET shippingFee = 0.00
        WHERE :old.custID = :new.custID;

    ELSE 
        UPDATE itemOrder
        SET shippingFee = 10.00
        WHERE :old.custID = :new.custID;
    END IF;
END checkCustTypeChange;
/ 
Show Errors;