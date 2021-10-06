/*Exercise 7*/
SELECT deptid AS deptno, count(*) AS empcount from L_EMP
GROUP BY deptid;

/*Exercise 8*/
SELECT deptno, deptname, empcount FROM (SELECT deptid AS deptno, count(*) AS empcount from L_EMP GROUP BY deptid), L_DEPT
WHERE deptno = L_DEPT.deptid;

SELECT deptono, deptname, empcount FROM (SELECT deptid AS deptno, count(*) AS empcount from L_EMP GROUP BY deptid), L_DEPT
WHERE deptno = L_DEPT.deptid
ORDER BY empcount ASC;

/*Exercise 9*/
/*SELECT deptid, max(count(*)) FROM L_EMP
GROUP BY deptid;

SELECT deptid FROM L_EMP
GROUP BY deptid
HAVING count(*) = (SELECT count(*) FROM L_EMP
GROUP BY deptid);*/

SELECT deptid, count(*) FROM L_EMP
GROUP BY deptid
HAVING count(*) = (SELECT max(count(*)) FROM L_EMP
GROUP BY deptid)

/*Exercise 10*/
SELECT * FROM L_EMP NATURAL JOIN L_DEPT;

SELECT L_EMP.deptid, empno, empname, deptname FROM L_EMP, L_DEPT
WHERE L_EMP.deptid = L_DEPT.deptid;