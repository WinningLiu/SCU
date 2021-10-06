Create or Replace FUNCTION getEmpSalary(p_name in VARCHAR)
Return NUMBER IS
/* Define the local variables you need.
You need a variable to hold the salary returned */
l_salary AlphaCoEmp.salary%type;
BEGIN
Select salary into l_salary from AlphaCoEmp
where name = p_name;

return l_salary;
END;
/
show errors;