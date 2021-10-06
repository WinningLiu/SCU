Create Table FoodsILike (
    foodName varchar(20),
    calories integer,
    noOfStars integer
);

Create or Replace procedure addFoodReview (v_foodName IN
varchar, v_calories IN integer, v_noOfStars IN integer) AS
BEGIN
insert into FoodsILike values (v_foodName,v_calories, v_noOfStars);
END;
/
Show Errors;