CREATE or REPLACE FUNCTION computeTotal (oID in itemOrder.orderID%type)
RETURN NUMBER IS 
    l_total storeItems.price%type;
    l_subtotal storeItems.price%type;

    l_itemID itemOrder.itemID%type;
    l_itemPrice storeItems.price%type;
    l_quantity itemOrder.noOfItems%type;
    l_shipFee itemOrder.shippingFee%type;
    l_custType Customer.custType%type;
    l_custID Customer.custID%type;
BEGIN
    SELECT itemID INTO l_itemID FROM itemOrder WHERE orderID = oID;
    SELECT price INTO l_itemPrice FROM storeItems WHERE itemID = l_itemID;
    SELECT noOfItems INTO l_quantity FROM itemOrder WHERE orderID = oID;
    SELECT shippingFee INTO l_shipFee FROM itemOrder WHERE orderID = oID;
    SELECT custID INTO l_custID FROM itemOrder WHERE orderID = oID;
    SELECT custType INTO l_custType FROM Customer WHERE custID = l_custID;

    l_subtotal := (l_itemPrice*l_quantity);
    IF (l_custType = 'gold' AND l_subtotal >= 100) THEN
        l_subtotal := 0.9*l_subtotal;
    END IF;
    l_total := (1.05 * l_subtotal) + (l_shipFee);

    RETURN l_total;
END;
/
Show Errors;