CREATE Or Replace TRIGGER titlecountchange_trig
AFTER INSERT ON AlphaCoEmp
FOR EACH ROW
BEGIN
Update EmpStats
set Empcount = Empcount + 1, lastmodified = SYSDATE
where title = :new.title;
END;
/
Show errors;