CREATE or REPLACE PROCEDURE setShippingDate (ordID in itemOrder.orderID%type, sDate in itemOrder.shippedDate%type)
IS
    l_orderDate itemOrder.orderDate%type;
BEGIN
    SELECT orderDate INTO l_orderDate FROM itemOrder WHERE orderID = ordID;
    IF (l_orderDate <= sDate) THEN
        UPDATE itemOrder
        SET shippedDate = sDate
        WHERE orderID = ordID;
    ELSE
        DBMS_OUTPUT.PUT_LINE('Invalid Date: Shipped date is before order date. Please try again.');
    END IF;

END;
/
Show Errors;