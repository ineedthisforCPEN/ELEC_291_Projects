<?php

$modules = $_POST['module'];

if(empty($modules)) {return;}

$msfile=fopen("/var/www/html/moduleSelect.txt", "w");
foreach($modules as $module) {
	fwrite($msfile, $module . "\n");
}

fclose($msfile);

header("Location: ../");
exit;

?>