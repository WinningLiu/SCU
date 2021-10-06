CREATE or REPLACE PROCEDURE addBookOrder (ordID in itemOrder.orderID%type, iID in itemOrder.itemID%type,
cusID in itemOrder.custID%type, oDate in itemOrder.orderDate%type, numOrdered in itemOrder.noOfItems%type, sDate 
in itemOrder.shippedDate%type)
IS
    l_custType Customer.custType%type;
	l_copies comicBooks.noOfCopies%type;
	l_shipFee itemOrder.shippingFee%type;
    noMoreBooks EXCEPTION;

BEGIN   
    SELECT noOfCopies INTO l_copies FROM comicBooks WHERE itemID = iID;
	SELECT custType INTO l_custType FROM Customer WHERE custID = cusID;

    IF (l_custType = 'regular') THEN    
        l_shipFee := 10.00;
    ELSIF (l_custType = 'gold') THEN
        l_shipFee := 0.00;
    END IF;

    IF (l_copies >= numOrdered) THEN
        INSERT INTO itemOrder VALUES (cusID, iID, ordID, oDate, NULL, numOrdered, l_shipFee);
        UPDATE comicBooks 
        SET noOfCopies = l_copies - numOrdered
        WHERE itemID = iID;
    ELSE
        DBMS_OUTPUT.PUT_LINE('Not enough books. Please try again.');
    END IF;

END;
/ 
Show Errors;