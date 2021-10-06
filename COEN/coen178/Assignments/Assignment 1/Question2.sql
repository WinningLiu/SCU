Create or Replace Function timeSpentBetweenDatesOn(v_fromdate In Date, v_toDate In Date, activity in VARCHAR)
Return INTEGER IS

total_time INTEGER;

BEGIN

if upper(activity) = 'WORK' then
select sum(work) into total_time from TimeSpent
where cdate between v_fromdate and v_toDate;
end if;

if upper(activity) = 'COMMUTE' then
select sum(commute) into total_time from TimeSpent
where cdate between v_fromdate and v_toDate;
end if;

if upper(activity) = 'MEALS' then
select sum(meals) into total_time from TimeSpent
where cdate between v_fromdate and v_toDate;
end if;


if upper(activity) = 'EXERCISE' then
select sum(exercise) into total_time from TimeSpent
where cdate between v_fromdate and v_toDate;
end if;


if upper(activity) = 'MISC' then
select sum(misc) into total_time from TimeSpent
where cdate between v_fromdate and v_toDate;
end if;

return total_time;

END;
/
Show Errors;