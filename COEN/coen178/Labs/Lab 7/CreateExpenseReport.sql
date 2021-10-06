
-- the size of one page
SET PAGESIZE 20
-- the size of a line
SET LINESIZE 120

BREAK ON TODAY

/*
To use the system date in the header or elsewhere in the report is to execute a query that returns the current date and use the NEW_VALUE clause of the COLUMN command to get that date into a substitution variable. That substitution variable sticks around for the duration of the session and can be used in subsequent reports.	
*/

/* The NEW_VALUE option of the COLUMN command is used to update the user variable report_date with the current value of SYSDATE as returned from the database. 
*/
COLUMN TODAY NEW_VALUE report_date
SELECT TO_CHAR(SYSDATE, 'fmMonth DD, YYYY') TODAY
FROM DUAL;

-- Sets terminal output off

set termout off
-- Show the title of your report at the top of the page

ttitle center "Expense Report "  report_date skip 2

 /* Places and formats a specified title at the bottom of each report page */

btitle center "Daren Liu"

--After the SPOOL command, anything entered or displayed on
-- standard output is written to the spool file, report.html.

spool ExpenseReport.txt

-- change column headings and format number columns
column Groceries format $99,999.99 heading "Groceries"
column Entertainment format $99,999.99 heading "Entertainment"
column Gas format $99,999.99 heading "Gas"

BREAK ON REPORT ON ROW SKIP 1
COMPUTE AVG LABEL Average MAX LABEL Maximum SUM LABEL Total OF Groceries Entertainment Gas ON REPORT

-- Run the query
select Groceries, Entertainment, Gas 
from MyExpenses;
spool off;

--clear all formatting commands ...

CLEAR COLUMNS
CLEAR BREAK
TTITLE OFF 
BTITLE OFF
SET VERIFY OFF 
SET FEEDBACK OFF
SET RECSEP OFF
SET ECHO OFF
