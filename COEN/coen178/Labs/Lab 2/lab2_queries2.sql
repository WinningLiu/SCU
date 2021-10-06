/*Exercise 1*/
SELECT empname FROM L_EMP, L_DEPT
WHERE L_DEPT.deptname = 'Testing'
AND L_EMP.deptid = L_DEPT.deptid;