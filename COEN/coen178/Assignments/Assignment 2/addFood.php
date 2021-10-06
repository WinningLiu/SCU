
<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {

    // collect input data from the form

	// Get the food name
     $foodName = $_POST['fname'];

	// Get the calories
     $calories = $_POST['calories'];

    // Get noOfStars
     $noOfStars = $_POST['noOfStars'];

     if (!empty($foodName)){
		$foodName = prepareInput($foodName);
     }
     if (!empty($calories)){
		$calories = prepareInput($calories);
     }
     if (!empty($noOfStars)){
		$noOfStars = prepareInput($noOfStars);
     }

	insertFoodReviewIntoDB($foodName,$calories,$noOfStars);
	
}

function prepareInput($inputData){
	$inputData = trim($inputData);
  	$inputData  = htmlspecialchars($inputData);
  	return $inputData;
}

function insertFoodReviewIntoDB($foodName,$calories,$noOfStars){
	//connect to your database. Type in sd username, password and the DB path
	$conn = oci_connect('dliu1', '', '//dbserver.engr.scu.edu/db11g');
	
	if (!$conn) {
		$e = oci_error();
		trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
	} 
	if(!$conn) {
	     print "<br> connection failed:";
        exit;
	}
	// Calling the PLSQL procedure, addMovieReview
	$sql = oci_parse($conn, 'begin addFoodReview(:foodname, :calories, :noOfStars); end;');	

	oci_bind_by_name($sql, ':foodname', $foodName);
	oci_bind_by_name($sql, ':calories', $calories);
    oci_bind_by_name($sql, ':noOfStars', $noOfStars);

	// Execute the query
	$res = oci_execute($sql);
	
	if ($res){
		echo '<br><br> <p style="color:green;font-size:20px">';
		echo "Food Review Inserted </p>";
	}
	else{
		$e = oci_error($query);
        	echo $e['message'];
	}
	oci_free_statement($sql);
	OCILogoff($conn);
}
?>
