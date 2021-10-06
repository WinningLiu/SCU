/*create table Emp_work(
    name varchar (25) primary key,
    Project varchar (20) default NULL,
    constraint FK_AlphaCo
    foreign key (name) references AlphaCoEmp (name)
); */

/*inserts names starting with a from AlphaCoEmp to Emp_work*/

 /*insert into Emp_work (name) select Name from AlphaCoEmp where
 regexp_like (name, '^[args]', 'i'); */

 select name from AlphaCoEmp where regexp_like(name,'^[args]', 'i');