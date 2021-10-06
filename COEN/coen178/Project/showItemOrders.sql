CREATE or REPLACE PROCEDURE showItemOrders(customerid in itemOrder.custID%type, inputDate in itemOrder.orderDate%type)
AS
    l_custID Customer.custID%type;
    l_name Customer.name%type;
    l_email Customer.email%type;
    l_address Customer.address%type;
    l_custType Customer.custType%type;

    l_orderID itemOrder.orderID%type;
    l_itemID storeItems.itemID%type;
    l_title comicBooks.title%type;
    l_size tShirt.shirtSize%type;
    l_price storeItems.price%type;
    l_orderDate itemOrder.orderDate%type;
    l_shippedDate itemOrder.shippedDate%type;
    l_quantity itemOrder.noOfItems%type;

    l_subtotal storeItems.price%type;
    l_total storeItems.price%type;
    l_tax storeItems.price%type;
    l_shippingFee itemOrder.shippingFee%type;
    l_discount storeItems.price%type;

    CURSOR c_comics IS 
    SELECT orderID, custID, itemID, title, price, orderDate, shippedDate, noOfItems, shippingFee FROM 
    (itemOrder JOIN comicBooks USING(itemID)) JOIN storeItems USING(itemID) WHERE custId = customerid AND orderDate >= inputDate ORDER BY custID;

    CURSOR c_tshirts IS 
    SELECT orderID, custID, itemID, shirtSize, price, orderDate, shippedDate, noOfItems, shippingFee FROM 
    (itemOrder JOIN tShirt USING(ItemId)) JOIN storeItems USING(itemID) WHERE custID = customerid AND orderDate >= inputDate ORDER BY custID;

BEGIN
    SELECT custID, name, email, address, custType INTO l_custID, l_name, l_email, l_address, l_custType FROM Customer WHERE custID = customerid;
    DBMS_OUTPUT.PUT_LINE('Customer Information: ');
    DBMS_OUTPUT.PUT_LINE('Customer ID: ' || l_custID || ', Name: ' || l_name || ', Email: ' || l_email || ', Address: ' || l_address);

    DBMS_OUTPUT.PUT_LINE('Comic Book Orders: ');
    OPEN c_comics;
    LOOP
        FETCH c_comics INTO l_orderID, l_custID, l_itemID, l_title, l_price, l_orderDate, l_shippedDate, l_quantity, l_shippingFee;
        EXIT WHEN c_comics%notfound;

        l_subtotal := l_price*l_quantity;
        
        IF l_custType = 'gold' AND l_subtotal >= 100 THEN
            l_discount := l_subtotal*0.1; 
            l_subtotal := l_subtotal*0.9;
            l_shippingFee := 0;
        ELSIF l_custType = 'regular' THEN
            l_discount := 0;
            l_shippingFee := 10;
        END IF;

        l_tax := l_subtotal*0.05;
        l_total := (l_subtotal*1.05) + l_shippingFee;

        DBMS_OUTPUT.PUT_LINE('Order ID: ' || l_orderID || ', Item ID: ' || l_itemID || ', Title: ' || l_title || ', Price: ' || l_price || ', Order Date: ' 
        || l_orderDate || ', Shipped Date: ' || l_shippedDate || ', Number Ordered: ' || l_quantity || ', Discount Applied: ' || l_discount || ', Shipping Fee: '
        || l_shippingFee || ', Tax: ' || l_tax || ', Order Total: ' || l_total);
    END LOOP;
    CLOSE c_comics;

    DBMS_OUTPUT.PUT_LINE('T-Shirt Orders: ');
    OPEN c_tshirts;
    LOOP
        FETCH c_tshirts INTO l_orderID, l_custID, l_itemID, l_size, l_price, l_orderDate, l_shippedDate, l_quantity, l_shippingFee;
        EXIT WHEN c_tshirts%notfound;

        l_subtotal := l_price*l_quantity;

        IF l_custType = 'gold' AND l_subtotal >= 100 THEN
            l_discount := l_subtotal*0.1; 
            l_subtotal := l_subtotal*0.9;
            l_shippingFee := 0;
        ELSIF l_custType = 'regular' THEN
            l_discount := 0;
            l_shippingFee := 10;
        END IF;

        l_tax := l_subtotal*0.05;
        l_total := (l_subtotal*1.05) + l_shippingFee;

        DBMS_OUTPUT.PUT_LINE('Order ID: ' || l_orderID || ', Item ID: ' || l_itemID || ', Size of T-Shirts: ' || l_size || ', Price: ' || l_price || ', Order Date: ' 
        || l_orderDate || ', Shipped Date: ' || l_shippedDate || ', Number Ordered: ' || l_quantity || ', Discount Applied: ' || l_discount || ', Shipping Fee: '
        || l_shippingFee || ', Tax: ' || l_tax || ', Order Total: ' || l_total);

    END LOOP;
    CLOSE c_tshirts;
END;
/
Show Errors;