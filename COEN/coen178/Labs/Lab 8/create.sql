Create table MovieReviews (
    movieName varchar(25), 
    comments varchar(40)
);

Create or Replace procedure addMovieReview (v_movieName IN varchar, v_comment IN varchar) AS
BEGIN
insert into MovieReviews values (v_movieName,v_comment);
END;
/
Show Errors;