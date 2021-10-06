Create Table Customer(
    custID varchar(10) primary key,
    name varchar(20),
    email varchar(40) unique not null,
    address varchar(40),
    dateJoined date,
    custType varchar(8) check (custType = 'regular' OR custType = 'gold')
);

Create Table storeItems(
    itemID varchar(10) primary key,
    price decimal(8, 2)
);

Create Table tShirt(
    itemID varchar(10) references storeItems(itemID),
    shirtSize varchar(4),
    primary key (itemID)
);

Create Table comicBooks(
    itemID varchar(10) references storeItems(itemID),
    ISBN varchar(13) unique, 
    title varchar(40),
    publishedDate date,
    noOfCopies integer check (noOfCopies >= 0),
    primary key (itemID)
);

Create Table copies(
    itemID varchar(10) references comicBooks(itemID),
    copyno varchar(10) unique
);

Create Table itemOrder(
    custID varchar(10) references Customer(custID),
    itemID varchar(10) references storeItems(itemID),
    orderID varchar(10) primary key,
    orderDate date,
    shippedDate date,
    noOfItems integer,
    shippingFee decimal(8,2),
    constraint checkDate check (shippedDate >= orderDate)
);