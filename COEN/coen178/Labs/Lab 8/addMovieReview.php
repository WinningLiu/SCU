
<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {

    // collect input data from the form

	// Get the movie name
     $movieName = $_POST['mname'];

	// Get the comment
     $comment = $_POST['comment'];

     if (!empty($movieName)){
		$movieName = prepareInput($movieName);
     }
     if (!empty($comment)){
		$comment = prepareInput($comment);
     }
	 
	// Call the function to insert moviename and comments
	// into MovieReviews table

	insertMovieReviewIntoDB($movieName,$comment);
	
}

function prepareInput($inputData){
	$inputData = trim($inputData);
  	$inputData  = htmlspecialchars($inputData);
  	return $inputData;
}

function insertMovieReviewIntoDB($movieName,$comment){
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
	$sql = oci_parse($conn, 'begin addMovieReview(:moviename, :comment); end;');	

	oci_bind_by_name($sql, ':moviename', $movieName);
	oci_bind_by_name($sql, ':comment', $comment);

	// Execute the query
	$res = oci_execute($sql);
	
	if ($res){
		echo '<br><br> <p style="color:green;font-size:20px">';
		echo "Movie Review Inserted </p>";
	}
	else{
		$e = oci_error($query);
        	echo $e['message'];
	}
	oci_free_statement($sql);
	OCILogoff($conn);
}
?>
