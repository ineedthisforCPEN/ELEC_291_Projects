<?php

$kill_conky = system('~/bin/conky_end', $ret_kill_conky);
$restart_conky = system('~/bin/conky_start');

?>