<?php
/* Database connection settings */
$host = 'localhost';
$user = 'root';
$pass = 'csaucd111';
$db = 'csa';
$mysqli = new mysqli($host,$user,$pass,$db) or die($mysqli->error);
