Create or Replace Procedure setEmpSalary(p_name in VARCHAR, low in INTEGER, high in INTEGER)
As
/* Define the local variables you need */
l_salary AlphaCoEmp.salary%type;
BEGIN
/* since name is a primary key, select the salary
Of the employee where name = p_name.
With an update statement, set the salary of that employee
With a randomly generated value between the low and high
passed
In as parameters
*/
    l_salary := ROUND(DBMS_RANDOM.VALUE(low, high));
    update AlphaCoEmp
    set salary = l_salary
    where name = p_name;
END;
/
show errors;