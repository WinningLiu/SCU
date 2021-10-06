Create or Replace Function countByTitle(p_title in AlphaCoEmp.title%type)
RETURN NUMBER IS
l_cnt Integer;
BEGIN
/* Complete the query below */
Select count(*) into l_cnt from AlphaCoEmp
Group by title
Having title = p_title;
return l_cnt;
END;
/
show errors;