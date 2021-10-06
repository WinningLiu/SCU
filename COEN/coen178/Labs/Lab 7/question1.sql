/*create or replace view CurrentProjects AS
select projectID, title, managerId
from Project7
where SYSDATE < endDate;

create or replace view PastProjects AS
select projectID, title, managerId
from Project7
where SYSDATE > endDate;*/

create or replace view ManagedProjects AS
select projectID, title as Project_Title, empName as ManagerName
from EMP7, Project7
where EMP7.empid = Project7.managerId;