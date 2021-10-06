SELECT custid, firstname || lastname AS fullname FROM Customer;

SELECT * FROM Customer
ORDER BY lastname ASC;

SELECT * FROM Schedule
ORDER BY serviceid DESC, 
custid DESC;

SELECT serviceid FROM DeliveryService MINUS SELECT serviceid FROM Schedule;

SELECT firstname from Customer, Schedule where day = 'm';

SELECT lastname FROM Customer
WHERE custid IN (SELECT custid FROM Schedule);

SELECT max(servicefee) AS highest_Servicefee FROM DeliveryService;

SELECT count(day), day FROM Schedule
GROUP BY day;

/*fix this for duplicates*/
SELECT A.custid, B.custid, A.city FROM Customer A, Customer B
WHERE A.city = B.city
AND A.custid > B.custid;

/*fix this for duplicates*/
SELECT custid, firstname || lastname AS fullname FROM Customer 
NATURAL JOIN DeliveryService
NATURAL JOIN Schedule
WHERE Customer.city = DeliveryService.location;

/*k*/
SELECT MAX(salary) AS maximum_salary, min(salary) AS minimum_salary FROM STAFF;