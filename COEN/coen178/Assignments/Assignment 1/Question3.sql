Create or Replace Trigger checkOverTime 
after insert or update on TimeSpent
for each row

BEGIN

if inserting and :NEW.work > 8 then
    insert into Overtime values (SYSDATE, (:new.work-8));
end if;

if updating and :NEW.work > 8 then
    insert into Overtime values (SYSDATE, (:new.work-8));
end if;
END;
/
Show Errors;