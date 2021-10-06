Create or Replace Procedure DisplayMessage (message in varchar)
As
Begin
    DBMS_OUTPUT.put_line('Hello ' || message);
End;
/
Show Errors;