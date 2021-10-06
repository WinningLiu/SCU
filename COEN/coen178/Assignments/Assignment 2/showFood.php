<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="utf-8"/>
      <title>Show Food Stats</title>
   </head>
   <body>

   <!-- Edit the link -->
   <a href='http://linux.students.engr.scu.edu/~dliu1/showFood.php?show=true'>Show 5 Star Foods</a>
   <br><br>
<?php

if (isset($_GET['show'])) {
    showStats();
}

function showStats(){
	//connect to your database. Type in your username, password and the DB path
	
	$conn = oci_connect('dliu1', '', '//dbserver.engr.scu.edu/db11g');
	if (!$conn) {
		$e = oci_error();
		trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
	} 
	if(!$conn) {
	     print "<br> connection failed:";
        exit;
	}
	// Query to find the food names and calories with 5 stars
	$query1 = oci_parse($conn, "SELECT foodName, calories FROM FoodsILike WHERE noOfStars = 5");

	// Execute the query
	oci_execute($query1);
	
	// Now access each row fetched in the loop
	while (($row = oci_fetch_array($query1, OCI_BOTH)) != false) {
		// We can use either numeric indexed starting at 0
		// or the column name as an associative array index to access the colum value
		// Use the uppercase column names for the associative array indices
		echo "<font color='black'>Food: </font>";
		echo "<font color='green'>  $row[0] </font>";
		echo "<font color='black'>No. of Calories: </font> <font color='red'> $row[1]</font></br>";		
	}
	
	OCILogoff($conn);
}
?>
<!-- end PHP script -->
   </body>
</html>
