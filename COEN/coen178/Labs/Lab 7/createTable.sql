/*Create Table EMP7(
    empid varchar(5) primary key,
    empName varchar(20),
    deptId varchar(5),
    salary number (10, 2)
);

Create Table Project7(
    projectID varchar(5) primary key,
    title varchar(20),
    budget number(10,2),
    startDate date,
    endDate date,
    managerId varchar(5)
);

Create Table EMP_Project (
    projectID varchar(5),
    empid varchar(5),
    commission number(10, 2)
);*/

Create Table MyExpenses (
    Groceries number(10, 2),
    Entertainment number(10, 2),
    Gas number(10, 2)
);