CREATE or REPLACE procedure saveCountByTitle
AS
l_advisor_cnt integer := 0;
l_director_cnt integer := 0; 
l_assistant_cnt integer := 0;
l_manager_cnt integer := 0;
l_supervisor_cnt integer := 0;
l_associate_cnt integer := 0;
BEGIN
l_advisor_cnt := countByTitle('advisor');
l_director_cnt := countByTitle('director');
l_assistant_cnt := countByTitle('assistant');
l_manager_cnt := countByTitle('manager');
l_supervisor_cnt := countByTitle('supervisor');
l_associate_cnt := countByTitle('associate'); 

delete from EmpStats; -- Any previously loaded data is deleted
/* inserting count of employees with title, ‘advisor’.*/
insert into EmpStats values ('advisor',l_advisor_cnt,SYSDATE);
insert into EmpStats values ('director',l_director_cnt,SYSDATE);
insert into EmpStats values ('assistant',l_assistant_cnt,SYSDATE);
insert into EmpStats values ('manager',l_manager_cnt,SYSDATE);
insert into EmpStats values ('supervisor',l_supervisor_cnt,SYSDATE);
insert into EmpStats values ('associate',l_associate_cnt,SYSDATE); 
END;
/
Show errors;