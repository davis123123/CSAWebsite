<?php
require "connection.php";
$user_name = $_POST["user_name"];
$user_pass = $_POST["password"];

$mysql_qry = "select * from users where username like '$user_name' and password like '$user_pass';";

$result = mysqli_query($conn ,$mysql_qry);
if(mysqli_num_rows($result) > 0){
  $response = array();
  while($row = mysqli_fetch_array($result)){
    array_push($response, array("user_name"=>$row[1],
    "password"=>$row[2],"email"=>$row[3],
    "membership"=>$row[4], "contribution_points"=>$row[5]));
      $id = $row[0];
  }
}
else{
  echo "login failed";
}
?>
