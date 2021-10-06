Create or Replace Function timeSpentByDate (v_date In Date) Return
Number IS
total_time INTEGER;

BEGIN

select (sum(work) + sum(commute) + sum(meals) + sum(exercise) + sum(misc)) into total_time from TimeSpent
where cdate = v_date;

return total_time;

END;
/
Show Errors;