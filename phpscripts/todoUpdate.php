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

// Get values sent from homepage
$item=$_POST['item'];
$func=$_POST['func'];
$text=$_POST['text'];

// If there are discrepancies (no task or function selected), exit code immediately
if(empty($func)) { goback(); }

// Open todolist file and copy its content into an array
// Not memory efficient, but todolist file is not expected to be large
// This is done to presever file content as writing erases the file
// Appending to the file would not always produce wanted results, hence "w" and not "w+"
$tdfile=fopen("/var/www/wwwdata/todo_list.txt", "r") or die("Error opening todo list");
$tdArray=array();
while(!feof($tdfile)) {
	$tdArray[]=fgets($tdfile);
}
fclose($tdfile);

// Open todolist list for writing
// This erases todolist.txt
$tdfile=fopen("/var/www/wwwdata/todo_list.txt", "w") or die("Error opening todo list");

// Used to generate labels on homepage return
$retStatus="";

switch($func) {
	case 'fin':
		if(empty($item)) {
			writeContent($tdfile, $tdArray);
			goback("?func=fintaskfail");
		}

		$text="";
		$lineCount=1;

		foreach ($tdArray as $line) {
			if($lineCount == $item) {
				$text=substr($line, 9);
				$replaceText="done" . substr($line, 4);
				fwrite($tdfile, $replaceText);
			} else {
				fwrite($tdfile, $line);
			}
			$lineCount++;
		}
 		$retStatus="?func=fintask&text=" . $text;
		break;
	case 'rst':
		if(empty($item)) {
			writeContent($tdfile, $tdArray);
			goback("?func=rsttaskfail");
		}

		$text="";
		$lineCount=1;

		foreach ($tdArray as $line) {
			if($lineCount == $item) {
				$text=substr($line, 9);
				$replaceText="todo" . substr($line, 4);
				fwrite($tdfile, $replaceText);
			} else {
				fwrite($tdfile, $line);
			}
			$lineCount++;
		}

		$retStatus="?func=rsttask&text=" . $text;
		break;
	case 'add':
		// If textbox is empty, go back to homepage
		if(empty($text)) { 
			writeContent($tdfile, $tdArray);
			goback("?func=addtaskfail");
		}

		$todotext = "todo • " . $text . "\n";
		writeContent($tdfile, $tdArray);
		fwrite($tdfile, $todotext);
		$retStatus="?func=addtask&text=" . $text;
		break;
	case 'rem':
		// If no item is selected, go back to homepage
		if(empty($item)) {
			writeContent($tdfile, $tdArray);
			goback("?func=remtaskfail");
		}

		$text="";
		$lineCount=1;

		foreach ($tdArray as $line) {
			if($lineCount == $item) {
				$text=substr($line, 9);
			} else {
				fwrite($tdfile, $line);
			}
			$lineCount++;
		}

		$retStatus="?func=remtask&text=" . $text;
		break;
	default:
		writeReplace($tdfile, $tdArray, -1, "");
		break;
}

fclose($tdfile);

header("Location: ../" . $retStatus);
exit;

?>