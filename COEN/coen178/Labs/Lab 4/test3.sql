alter table Emp_work drop constraint FK_AlphaCo;

alter table Emp_work
add constraint FK_AlphaCo
foreign key (name)
references AlphaCoEmp (name)
on delete cascade;