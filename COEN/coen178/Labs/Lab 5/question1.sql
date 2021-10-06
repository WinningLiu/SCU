Create or Replace Procedure assignJobTitlesAndSalaries
As
    type titlesList IS VARRAY(6) OF AlphaCoEmp.title%type; 
    type salaryList IS VARRAY(6) of AlphaCoEmp.salary%type; 
    v_titles titlesList; 
    v_salaries salaryList; 
Cursor Emp_cur IS 
    Select * from AlphaCoEmp; 
    l_emprec Emp_cur%rowtype; 
    l_title AlphaCoEmp.title%type; 
    l_salary AlphaCoEmp.salary%type; 
    l_randomnumber INTEGER := 1; 

Begin
v_titles := titlesList('advisor', 'director', 'assistant', 'manager', 'supervisor', 'associate'); 
v_salaries := salaryList(130000, 100000, 600000, 500000, 800000, 400000);
for l_emprec IN Emp_cur 
LOOP
    l_randomnumber := dbms_random.value(1,6);
    l_title := v_titles(l_randomnumber);
    l_salary := v_salaries(l_randomnumber); 

    update AlphaCoEmp
    set title = l_title 
    where name = l_emprec.name;

    update AlphaCoEmp 
    set salary = l_salary 
    where name = l_emprec.name; 

END LOOP; 

commit; 
END; 
/ 
Show errors; 