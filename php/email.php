<?php
require "connect.php";
$confirmcode = rand();
$user_name = "test";
$email = "daviswong123@gmail.com";
$message =
"
Confirm your E-mail
Click the link below to verify your account.
http://45.55.13.44/email_confirmation.php?username=$user_name&code=$confirmcode
";
if(mail($email,"CSA Confirm Email",$message)){
  echo "true";
}
else{
  echo"false";
}
echo $username;
echo $email;
echo $message;
?>
