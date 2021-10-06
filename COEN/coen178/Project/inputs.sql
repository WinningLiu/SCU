set serveroutput on

insert into customer values ('g000000000', 'Daren Liu', 'dliu3@scu.edu', '111 Welker St.', '10-Mar-21', 'gold');
insert into customer values ('g000000001', 'Stephen Curry', 'wardellcurry@warriors.com', '300 Atherton Av.', '06-Jun-09', 'gold');
insert into customer values ('g000000002', 'Homie Dawg', 'yaboi@gmail.com', 'Lmao EggsDee St.', '15-Apr-20', 'gold');
insert into customer values ('r000000000', 'John Doe', 'regularjoe@gmail.com', '980 Fake Blvd.', null, 'regular');
insert into customer values ('r000000001', 'Peter Griffin', 'hansolo@hotmail.com', '255 Spooner St.', null, 'regular');
insert into customer values ('r00000002', 'Jake Paul', 'JakePaul@gmail.com', 'Everyday Bro St.', null, 'regular');

select * from customer;

insert into storeItems values ('s000000000', 10.99);
insert into storeItems values ('s000000001', 15.25);
insert into storeItems values ('s000000002', 25.95);
insert into storeItems values ('b000000000', 29.99);
insert into storeItems values ('b000000001', 15.99);
insert into storeItems values ('b000000002', 9.50);

select * from storeItems;

insert into comicBooks values ('b000000000', '9781479580033', 'Adventures of Superman', '25-Aug-2016', 5);
insert into comicBooks values ('b000000001', '9780676978995', 'The Life of Pi', '11-Sep-2001', 7);
insert into comicBooks values ('b000000002', '9856162516860', 'Batman and Robin', '23-Feb-2017', 10);

select * from comicBooks;

insert into tShirt values ('s000000000', 'm');
insert into tShirt values ('s000000001', 'l');
insert into tShirt values ('s000000002', 'xl');

select * from tShirt;

start /home/dliu1/Documents/COEN/coen178/Project/addBookOrder.sql

exec addBookOrder ('PX100ME836', 'b000000000', 'r000000001', '1-Mar-21', 2, null);
exec addBookOrder ('ASDFN253DS', 'b000000001', 'g000000001', '21-Feb-21', 5, null);

select * from itemOrder;

--Won't work since there are more ordered than available
exec addBookOrder ('JNBKKR5651', 'b000000001', 'g000000000', '21-Dec-20', 30, null);

select * from itemOrder;

start /home/dliu1/Documents/COEN/coen178/Project/addShirtOrder

exec addShirtOrder ('DJHFNG5315', 's000000002', 'r000000001', '2-Mar-21', 10, null);
exec addShirtOrder ('EWKLGQH512', 's000000001', 'g000000002', '10-Mar-21', 20, null);

select * from itemOrder;

start /home/dliu1/Documents/COEN/coen178/Project/setShippingDate.sql

exec setShippingDate ('PX100ME836', '25-Mar-21');
--Won't work since shipping date is before order date
exec setShippingDate ('ASDFN253DS', '10-Feb-21');

exec setShippingDate ('ASDFN253DS', '10-Mar-21');
exec setShippingDate ('DJHFNG5315', '24-Mar-21');
exec setShippingDate ('EWKLGQH512', '05-Jun-21');

select * from itemOrder;

start /home/dliu1/Documents/COEN/coen178/Project/computeTotal.sql

--Book orders
select computeTotal('PX100ME836') from dual;
select computeTotal('ASDFN253DS') from dual;

--Shirt orders
select computeTotal('DJHFNG5315') from dual;
select computeTotal('EWKLGQH512') from dual;

start /home/dliu1/Documents/COEN/coen178/Project/checkCustTypeChange.sql

select * from customer;
select * from itemOrder;

UPDATE Customer
SET dateJoined = SYSDATE, 
custType = 'gold'
WHERE custID = 'r000000001';

select * from Customer;
select * from itemOrder;

start /home/dliu1/Documents/COEN/coen178/Project/showItemOrders

exec showItemOrders('r000000001', '9-Jan-01');

--Won't show anything since date is after the order date
exec showItemOrders('g000000001', '22-Feb-21');

exec showItemOrders('g000000002', '20-Feb-21');