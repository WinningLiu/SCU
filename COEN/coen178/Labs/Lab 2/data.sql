Create table Customer (
    custid varchar(5) PRIMARY KEY,
    firstname varchar(10),
    lastname varchar(15),
    city varchar(10)
);

Create table DeliveryService(
    serviceid varchar(10) PRIMARY KEY,
    item varchar(15),
    location varchar(15),
    servicefee decimal(5,2)
);

Create table Schedule(
    serviceid varchar(10),
    custid varchar(5),
    FOREIGN KEY (custid) REFERENCES Customer(custid),
    FOREIGN KEY (serviceid) REFERENCES DeliveryService(serviceid),
    day varchar(2) NOT NULL CHECK (day in ('m', 't', 'w', 'r', 'f'))
);