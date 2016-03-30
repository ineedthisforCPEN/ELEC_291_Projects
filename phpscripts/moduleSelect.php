<?php

$modules = $_POST['module'];

if(empty($modules)) {return;}

$msfile=fopen("/var/www/wwwdata/module_select.txt", "w");
foreach($modules as $module) {
	fwrite($msfile, $module . "\n");
}

fclose($msfile);

header("Location: ../?func=rfsmirror");
exit;

?>