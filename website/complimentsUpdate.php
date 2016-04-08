<?php

// Purpose: This function taskes $content and writes it into $file
function writeContent($file, $content) {
	foreach ($content as $line) {
		fwrite($file, $line);
	}
}

// Returns user back to homepage
function goback($where) {
	header("Location: ../" . $where);
	exit;
}

$item=$_POST['item'];
$func=$_POST['func'];
$text=$_POST['text'];

if(empty(func)) { goback(); }

$cfile=fopen("/var/www/wwwdata/compliment_list.txt", "r");
$cArray=array();
while(!feof($cfile)) {
	$cArray[]=fgets($cfile);
}
fclose($cfile);

$cfile=fopen("/var/www/wwwdata/compliment_list.txt", "w");
$retStatus="";

switch($func) {
	case('add'):
		if(empty($text)) {
			writeContent($cfile, $cArray);
			goback('?func=addcompfail');
		}

		writeContent($cfile, $cArray);
		fwrite($cfile, $text . "\n");
		$retStatus='?func=addcomp&text=' . $text;
		break;
	case('rem'):
		if(empty($item)) {
			writeContent($cfile, $cArray);
			goback('?func=remcompfail');
		}

		$text="";
		$lineCount=1;

		foreach ($cArray as $line) {
			if($lineCount == $item) {
				$text=$line;
			} else {
				fwrite($cfile, $line);
			}
			$lineCount++;
		}

		$retStatus="?func=remcomp&text=" . $text;
		break;
	default:
		break;
}

fclose($cfile);

header("Location: ../" . $retStatus);
exit;

?>