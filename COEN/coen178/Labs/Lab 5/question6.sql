CREATE Or Replace TRIGGER countchange_trig
AFTER INSERT or DELETE ON AlphaCoEmp
FOR EACH ROW
BEGIN
IF DELETING THEN
Update EmpStats
set Empcount = Empcount -1, lastmodified = SYSDATE
where title = :old.title;

END IF;
IF Inserting THEN
Update EmpStats
/* Complete the query here */
-------------------
set Empcount = Empcount + 1, 
lastmodified = SYSDATE
where title = :new.title;
End IF;
END;
/
Show errors;