<?php
require "connection.php";
$user_name = $_POST["user_name"];
$user_pass = $_POST["password"];
$email = $_POST["email"];



$mysql_qry = "insert into users (username, password, email, contribution_points)
values ('$user_name', '$user_pass', '$email',0)";


if($conn->query($mysql_qry) === TRUE){
  echo "registration successful";
}
else{
  echo "Error: ". $mysql_qry . "<br>" . $conn->error;
}
$conn->close();
?>
