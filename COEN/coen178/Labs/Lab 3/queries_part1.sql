/*Exercise 1*/
SELECT first || last AS fullname, salary FROM Staff
WHERE salary >= ALL (SELECT salary from Staff);

SELECT first || last AS fullname, salary FROM Staff
WHERE salary = (SELECT max(salary) FROM Staff);

/*Exercise 2*/
/*Select last, salary from Staff
where salary = (select salary from Staff where last = 'Zichal');*/

Select last, salary from Staff
where salary = (select salary from Staff where upper(last) = 'ZICHAL');

/*SELECT last, salary FROM Staff
WHERE salary = (SELECT salary FROM Staff WHERE last = 'Young');*/


SELECT last, salary FROM Staff
WHERE salary IN (SELECT salary FROM Staff WHERE UPPER(last) = 'YOUNG');

/*Exercise 3*/
SELECT count(salary) as SALARIES_ABOVE_100K FROM Staff
WHERE salary > 100000;

/*Exercise 4*/
SELECT salary, count(salary) as SALARIES_ABOVE_100K FROM Staff
WHERE salary > 100000
GROUP BY salary;

/*Exercise 5*/
SELECT salary, count(salary) as SALARIES_ABOVE_100K FROM Staff
WHERE salary > 100000
GROUP BY salary
HAVING count(*) >= 10;

/*Exercise 6*/
SELECT last FROM Staff
WHERE REGEXP_LIKE (last, '([aeiou])\1', 'i');