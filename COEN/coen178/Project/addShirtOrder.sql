CREATE or REPLACE PROCEDURE addShirtOrder (ordID in itemOrder.orderID%type, iID in itemOrder.itemID%type,
cusID in itemOrder.custID%type, oDate in itemOrder.orderDate%type, numOrdered in itemOrder.noOfItems%type, sDate 
in itemOrder.shippedDate%type)
IS
    l_custType Customer.custType%type;
	l_shipFee itemOrder.shippingFee%type;
    noMoreBooks EXCEPTION;

BEGIN   
	SELECT custType INTO l_custType FROM Customer WHERE custID = cusID;

    IF (l_custType = 'regular') THEN    
        l_shipFee := 10.00;
    ELSIF (l_custType = 'gold') THEN
        l_shipFee := 0.00;
    END IF;

    INSERT INTO itemOrder VALUES (cusID, iID, ordID, oDate, NULL, numOrdered, l_shipFee);

END;
/ 
Show Errors;