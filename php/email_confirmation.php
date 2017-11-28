<?php
require "connect.php";
$email = $_GET['email'];
$code = $_GET['code'];

$query= "SELECT * from users where email like '$email';";
$result = mysqli_query($conn ,$mysql_check_user);
while($row = mysqli_fetch_assoc($query)){
  $db_code = $row['confirmcode'];
}
if($code == $db_code){
  mysqli_query("UPDATE 'client_users' SET 'email_confirmed'='1'");
  mysqli_query("UPDATE 'client_users' SET 'confirmcode'='0'");
  echo"Thank you. Your email has been confirmed and you may now login!";
}
else{
  echo "Username and code do not match";
}
?>
