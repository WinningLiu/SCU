Create Or Replace Trigger NoUpdatePK_trig
After UPDATE ON BANKCUST_6
For each row
BEGIN
if updating ('custno') then
raise_application_error (-20999,'Cannot update a Primary Key');
End if;
END;
/
show errors;