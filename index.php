<!DOCTYPE html>
<html>
<head>  
    <title>Smart Mirror Settings</title>
    <meta charset="utf-8">

    <!-- Script to limit the refresh rate of the user -->
    <script type="text/javascript">
        $('#refresh').delay(5000).show(0);
    </script>

    <!--
    link to external CSS -
    make sure index.php and main.css are in the same directory
    -->
    <link rel="stylesheet" type="text/css" href="./main.css"> 

    <!-- Set up bootstrap from external sources -->
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
    <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>

    <!-- jQuery to run php script to change server files -->
    <script type="text/javascript" src="http://code.jquery.com/jquery-latest.min.js"></script>
</head>
<body>
    <!-- ==================================================================================================== -->
    <div class="container">
        <!-- ==================================================================================================== -->
        <div class="jumbotron">
            <!--
            Read info from ip.txt
            This reads the website address
            -->
             <?php
                $ipv4file = fopen("/var/www/wwwdata/ip.txt", "r") or die("xxx.xxx.xxx.xxx");
                $currentipv4 = fgets($ipv4file);
                fclose($ipv4file);
            ?> 

            <h1>Smart Mirror Settings</h1>
            <p> Welcome to the smart mirror settings page. Here, you can see what data the smart mirror has received. You may also turn on or off mirror modules (e.g., you may turn off the TODO list if you wish).
            <p>Current Web Address: <kbd><?php echo $currentipv4;?></kbd></p>
        </div>

        <?php
            // Helper function to display update times in minutes and seconds
            function formatTime($toFormat) {
                if ($toFormat < 0) {
                    return "0 seconds";
                } else if ($toFormat > 24*3600) {
                    return "day";
                } else if ($toFormat < 60) {
                    return (string)$toFormat . " seconds";
                } else {
                    $numMins=0;
                    while ($toFormat >= 60) {
                        $numMins++;
                        $toFormat-=60;
                    }
        
                    if ($toFormat <= 0) {
                        if ($numMins == 1) {
                            return (string) "1 minute";
                        }
                        return (string)$numMins . " minutes";
                    }

                    if ($numMins == 1) {
                            return (string) "1 minute and " . $toFormat . "seconds";
                        }        
                    return (string)$numMins . " minutes and " . $toFormat . " seconds";
                }
                return "";
            }

            // Get the func parameter from the address bar
            $function = $_GET['func'];
            $updatetime = $_GET['time'];
            $text = $_GET['text'];

            // Determine what to do with the func parameter
            switch($function) {
                case 'rbtmirror':
                    // Create file used as a flag
                    // If the file exists, the mirror will reboot
                    $rbtfile = fopen("/var/www/wwwdata/rbtflag", "w") or die("Error rebooting mirror. <a onClick=\"location.href='../'\">Click here to go back</a>");
                    fclose($rbtfile);

                    echo "<div class=\"alert alert-warning\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Rebooting mirror - this might take some time...</a></div>";
                    break;
                case 'rfsmirror':
                    // Create file used as a flag
                    // If the file exists, the mirror will refresh
                    $rfsfile = fopen("/var/www/wwwdata/rfsflag", "w") or die("Error refreshing mirror. <a onClick=\"location.href='../'\">Click here to go back</a>");
                    fclose($rfsfile);

                    echo "<div class=\"alert alert-warning\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Refreshing mirror - this might take some time...</a></div>";

                    break;
                case 'addcomp':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Added \"" . $text . "\" to compliments list</a></div>";
                    break;
                case 'addcompfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to add compliment to compliments list - no compliment written in text box</a></div>";
                    break;
                case 'remcomp':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Removed \"" . $text . "\" from compliments list</a></div>";
                    break;
                case 'remcompfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to remove compliment from compliments list - no compliment selected</a></div>";
                    break;
                case 'fintask':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Completed Task \"" . $text . "\"</a></div>";
                    break;
                case 'fintaskfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to complete task - no task selected</a></div>";
                    break;
                case 'rsttask':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Reset Task \"" . $text . "\"</a></div>";
                    break;
                case 'rsttaskfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to reset task - no task selected</a></div>";
                    break;
                case 'addtask':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Added Task \"" . $text . "\"</a></div>";
                    break;
                case 'addtaskfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to add task - no task written in text box</a></div>";
                    break;
                case 'remtask':
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Removed Task \"" . $text . "\"</a></div>";
                    break;
                case 'remtaskfail':
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to remove task - no task selected</a></div>";
                    break;
                case 'itemp':
                    // Make sure only acceptable values are input (this prevents users from inputting unreasonable values in the address bar)
                    if($updatetime % 30 != 0) { break; }
                    // Write arduinoUpdate file
                    $auFile = fopen("/var/www/wwwdata/update_times/arduino_update.txt", "w") or die("Failed to change update time.<a onClick=\"location.href='../'\">Click here to go back</a>");
                    fwrite($auFile, $updatetime);
                    fclose($auFile);

                    echo "<div class=\"alert alert-info\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Internal conditions will update once every " . formatTime($updatetime) . " - this will take effect after mirror reboot</a></div>";
                    break;
                case 'ccomp':
                    // Make sure only acceptable values are input (this prevents users from inputting unreasonable values in the address bar)
                    if($updatetime % 30 != 0) { break; }

                    // Write arduinoUpdate file
                    $coFile = fopen("/var/www/wwwdata/update_times/compliment_update.txt", "w") or die("Failed to change compliment change time.<a onClick=\"location.href='../'\">Click here to go back</a>");
                    fwrite($coFile, $updatetime);
                    fclose($coFile);

                    echo "<div class=\"alert alert-info\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Compliments now change once every " . formatTime($updatetime) . " - this will take effect after mirror reboot</a></div>";
                    break;
                default:
                    break;
            }
        ?>

        <p style="color: #888888"><small>Try refreshing the page if you see something wrong. This might fix the problem.</small></p>
        <!-- ==================================================================================================== -->
        <div class="row">
            <div class="col-md-2">
                <button class="btn btn-block btn-success" onClick="location.href='../'">Refresh Settings Page</button>
            </div>
            <div class="col-md-2 col-md-offset-6">
                <a href="?func=rfsmirror"><button class="btn btn-block btn-primary">Refresh Mirror</button></a>
            </div>
            <div class="col-md-2">
                <a href="?func=rbtmirror"><button class="btn btn-primary btn-block" onclick="rebootMirror()">Reboot Mirror</button></a>
            </div>
        </div>
        <!-- ==================================================================================================== -->
        <div class="row">
            <!-- ==================================================================================================== -->

            <!-- Column for weather, internal temperature, and other measurements-->
            <div class="col-md-4">
                <!-- This script gathers information from the arduino -->
                <?php
                    $tfile = fopen("/var/www/wwwdata/temperature.txt", "r") or die("--");  // Open temperature file
                    $hfile = fopen("/var/www/wwwdata/humidity.txt", "r") or die("--");     // Open humidity file
                    $indoorTemp = fgets($tfile);                                        // Read line from temperature file
                    $indoorHumi = fgets($hfile);                                        // Read line form humidity file
                    fclose($tfile);                                                     // Close temperature file
                    fclose($hfile);                                                     // Close humidity file
                ?>


	<!-- Section for time (By James) -->
                <div class="page-header">
                    <h2>Time</h2>
                </div>
                <div class="well well-sm">
	      
                   
		<p align="center" style="padding-center: 5%; font-size: 350%;"><script>var x = new Date();
		    var time = 'AM';
		    var chara = '0';
		    var hours = x.getHours();
		    var minutes = x.getMinutes().toString();
		    var array1 = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
		    var array2 = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'];
		    if(x.getHours() > 12)
		    {
			    time = 'PM';
			    hours -= 12;

		    }
		    if(x.getMinutes() < 10)
		    {
			minutes = chara.concat(minutes);
		    }
				document.write(hours + ":" + minutes + " " + time);</script></p>
                           
                       
                   
                    <div class="panel-group">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" href="#extratimeinfo">Click For Extra Information</a>
                                </h4>
                            </div>
                            <div id="extratimeinfo" class="panel-collapse collapse">
                  
				<div class="panel-body">Date:<span style="float: right;"><script>
				
			
				document.write(array2[x.getDay()] + "," + " " + array1[x.getMonth()] + " " + x.getDate() + "," + " " +  x.getFullYear())</script></span></div>
                                <div class="panel-body">Time Counter:<span style="float: right;"><script></script></span></div> 
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Section for weather -->
                <div class="page-header">
                    <h2>Weather</h2>
                </div>
                <div class="well well-sm">
                    <?php
                        //$request = 'http://api.openweathermap.org/data/2.5/weather?q=Vancouver,can&appid=ce1664b9a59f64885ad5bcfc8ee4f1a4';
                        //$response  = file_get_contents($request);
                        //$jsonobj  = json_decode($response, true);
                        $jsonobj = json_decode('{"coord":{"lon":-123.12,"lat":49.25},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}],"base":"cmc stations","main":{"temp":280.45,"pressure":1017,"humidity":81,"temp_min":276.65,"temp_max":282.15},"wind":{"speed":6.7,"deg":210},"clouds":{"all":40},"dt":1459142270,"sys":{"type":1,"id":3359,"message":0.0136,"country":"CA","sunrise":1459173385,"sunset":1459219138},"id":6173331,"name":"Vancouver","cod":200}', true);
                        
                        $owmCity = $jsonobj['name'];

                        // coord objects
                        $owmLatitude = $jsonobj['coord']['lat'];
                        $owmLongitude = $jsonobj['coord']['lon'];

                        // weather objects
                        $owmCondition = $jsonobj['weather'][0]['main'];
                        $owmConditionExtra = $jsonobj['weather'][0]['description'];

                        // main objects
                        $owmTemperature = ($jsonobj['main']['temp'] - 273.15);
                        $owmPressure = $jsonobj['main']['pressure'];
                        $owmHumidity = $jsonobj['main']['humidity'];
                        $owmTempMin = ($jsonobj['main']['temp_min'] - 273.15);
                        $owmTempMax = ($jsonobj['main']['temp_max'] - 273.15);
                        
                        // sys objects
                        $owmCountry = $jsonobj['sys']['country'];
                        $owmSunrise = $jsonobj['sys']['sunrise'];
                        $owmSunset = $jsonobj['sys']['sunset'];
                    ?>
                    <div class="row">
                        <div class="col-md-4">
                            <p style="padding-left: 5%; font-size: 120%; color: #888888;"><small><?php echo "Max: " . $owmTempMax; ?>&#8451</small></p>
                            <p style="padding-left: 5%; font-size: 120%; position: relative; bottom: 15px; color: #888888;"><small><?php echo "Min: " . $owmTempMin;  ?>&#8451</small></p>
                        </div>
                        <div class="col-md-8">
                            <p align="right" style="padding-right: 5%; font-size: 350%;"><?php echo $owmTemperature; ?>&#8451</p>
                            <p align="right" style="padding-right: 5%; font-size: 200%;"><?php echo $owmCondition; ?></p>
                            <p align="right" class="under-right"><small><?php echo $owmConditionExtra; ?></small></p>
                        </div>
                    </div>
                    <p style="padding-left: 5%; font-size: 180%;"><?php echo $owmCity; ?><small style="color: #888888;"><?php echo "\tCountry"; ?></small></p>
                    <p class="under-left" style="font-size: 120%"><small><?php echo $owmLatitude . ", " . $owmLongitude; ?></small></p>
                    <div class="panel-group">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" href="#extraweatherinfo">Click For Extra Information</a>
                                </h4>
                            </div>
                            <div id="extraweatherinfo" class="panel-collapse collapse">
                                <div class="panel-body">Timezone:<span style="float: right;"><?php echo date('e'); ?></span></div>
                                <div class="panel-body">Sunrise:<span style="float: right;"><?php echo date('h:i a', $owmSunrise); ?></span></div>
                                <div class="panel-body">Sunset:<span style="float: right;"><?php echo date('h:i a', $owmSunset); ?></div>
                                <div class="panel-body">Pressure:<span style="float: right;"><?php echo $owmPressure . " "; ?>kPa</span></div>
                                <div class="panel-body">Humidity:<span style="float: right;"><?php echo $owmHumidity . " "; ?>%</div>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- Section for local data -->
                <div class="page-header">
                    <h2>Mirror Measurements</h2>
                </div>
                <div class="well well-sm">
                    <h4>Current Indoor Temperature<small style="float: right;"><?php echo $indoorTemp; ?>&#8451</small></h4>
                    <h4>Current Indoor Humidity<small style="float: right;"><?php echo $indoorHumi; ?>%</small></h4>
                </div>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for compliments, todo list, and any other entries that might be needed -->
            <div class="col-md-5">
                <!-- One section for the compliments -->
                <div class="page-header">
                    <h2>List of Compliments</h2>
                </div>
                <form action="complimentsUpdate.php" method="post">
                    <div class="well well-sm">
                        <!-- Display every compliment in the compliments.txt file -->
                        <!-- Each compliment gets its own line -->
                        <?php
                            // This code section is used to calculate how many lines are in compliments.txt
                            // This is used to prevent the page from displaying an extra, empty checkbox
                            $cfile = fopen("/var/www/wwwdata/compliment_list.txt", "r") or die("<p>-- You have no compliments --</p>");
                            $numlines=0;
                            while(! feof($cfile)) {
                                fgets($cfile);
                                $numlines++;
                            }
                            fclose($cfile);

                            $cfile = fopen("/var/www/wwwdata/compliment_list.txt", "r") or die("<p>-- You have no compliments --</p>");
                            $cindex = 1;
                            while(--$numlines) {
                                echo "<div class='radio'><label><input type='radio' name='item' value='" . $cindex . "''> " . fgets($cfile) . "</label></div>";
                                $cindex++;
                            }
                            fclose($cfile);
                        ?>
                    </div>
                    <div class="well well-sm">
                        <div class="form-group">
                            <label for="Compliment">Your Compliment:</label>
                            <input type="text" class="form-control" name="text">
                        </div>
                    </div>
                    <div class="btn-group btn-group-justified">
                        <div class="btn-group"><button type="submit" name="func" value="add" class="btn btn-default">Add Custom Compliment</button></div>
                        <div class="btn-group"><button type="submit" name="func" value="rem" class="btn btn-default">Remove Selected Compliment</button></div>
                    </div>
                </form>

                <!-- One section for the todo list -->
                <div class="page-header">
                    <h2>TODO List</h2>
                </div>
                <!-- Display every todo item in the todolist.txt file -->
                <!-- Every item gets its own line -->
                <!-- Items that have been marked done are crossed through -->
                <form action="todoUpdate.php" method="post">
                    <div class="well well-sm">
                        <?php
                            // This code section is used to calculate how many lines are in todolist.txt
                            // This is used to prevent the page from displaying an extra, empty checkbox
                            $tdfile = fopen("/var/www/wwwdata/todo_list.txt", "r");
                            $numlines=0;
                            while(! feof($tdfile)) {
                                fgets($tdfile);
                                $numlines++;
                            }
                            fclose($tdfile);

                            $tdfile = fopen("/var/www/wwwdata/todo_list.txt", "r");
                            $tdindex = 1;
                            while (--$numlines) {
                                $line = fgets($tdfile);
                                $linearray = explode(' ', trim($line));
                                $first = $linearray[0];
                                $rest = substr(strstr($line, ' '), 4);
                                if (strcmp($first, "todo") == 0) {
                                    echo "<div class='radio'><label><input type='radio' name='item' value='" . $tdindex . "'> " . $rest . "</label></div>";
                                } else {
                                    echo "<div class='radio'><label><input type='radio' name='item' value='" . $tdindex . "'><del> " . $rest . "</del></label></div>";
                                }
                                $tdindex++;
                            }
                            fclose($tdfile);
                        ?>
                    </div>
                    <div class="well well-sm">
                        <div class="form-group">
                            <label for="Todo">Your TODO Item:</label>
                            <input type="text" class="form-control" name="text">
                        </div>
                    </div>
                    <div class="btn-group btn-group-justified">
                        <div class='btn-group'><button type="submit" name="func" value="fin" class="btn btn-default">Complete Task</button></div>
                        <div class='btn-group'><button type="submit" name="func" value="rst" class="btn btn-default">Reset Task</button></div>
                        <div class='btn-group'><button type="submit" name="func" value="add" class="btn btn-default">Add Task</button></div>
                        <div class='btn-group'><button type="submit" name="func" value="rem" class="btn btn-default">Remove Task</button></div>
                    </div>
                </form>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for changing settings (drop down menus, checkboxes and buttons) -->
            <div class="col-md-3">
                <!-- Turn mirror modules on and off -->
                <div class="page-header">
                    <h2>Modules Displayed</h2>
                </div>
                <span class="label label-warning">Requires Mirror Refresh</span>
                <div class="well well-sm">
                    <form action="moduleSelect.php" method="post">
                        <?php
                            $msfile=fopen("/var/www/wwwdata/module_select.txt", "r") or die("Error loading modules. <a onClick=\"location.href='../'\">Click here to go back</a>");
                            $modArray=array();

                            while(!feof($msfile)) {
                                $line=fgets($msfile);
                                $modArray[]=$line;
                            }
                            
                            fclose($msfile);

                            // Display checkbox for time module
                            echo "<p><input type='checkbox' name='module[]' value='time'";
                            if (in_array("time\n", $modArray)) { echo " checked='' "; }
                            echo "> <b>Time</b> <small>Module</small></p>";

                            // Display checkbox for indoor conditions module
                            echo "<p><input type='checkbox' name='module[]' value='itemp'";
                            if (in_array("itemp\n", $modArray)) { echo " checked='' "; }
                            echo "> <b>Indoor Conditions</b> <small>Module</small></p>";

                            // Display checkbox for time module
                            echo "<p><input type='checkbox' name='module[]' value='weather'";
                            if (in_array("weather\n", $modArray)) { echo " checked='' "; }
                            echo "> <b>Weather</b> <small>Module</small></p>";

                            // Display checkbox for time module
                            echo "<p><input type='checkbox' name='module[]' value='todo'";
                            if (in_array("todo\n", $modArray)) { echo " checked='' "; }
                            echo "> <b>Todo</b> <small>Module</small></p>";

                            // Display checkbox for time module
                            echo "<p><input type='checkbox' name='module[]' value='compliments'";
                            if (in_array("compliments\n", $modArray)) { echo " checked='' "; }
                            echo "> <b>Compliments</b> <small>Module</small></p>";
                        ?>
                        <button type="submit" class="btn btn-default btn-block">Refresh Mirror</button>
                    </form>
                </div>

                <!-- Set update times -->
                <div class="page-header">
                    <h2>Update Times</h2>
                </div>
                    <span class="label label-danger">Requires Mirror Reboot to Take Effect</span>
                <div class="well well-sm">
                <!-- Dropdown button for changing arduino update time -->
                <div class="btn-group">
                    <button class="btn btn-default dropdown-toggle btn-block" data-toggle="dropdown">Indoor Temperature <span class="caret"></span></button>
                    <ul class="dropdown-menu" role="menu">
                        <li><a href="?func=itemp&time=30" >30 seconds</a></li>
                        <li><a href="?func=itemp&time=60" >1 minute</a></li>
                        <li><a href="?func=itemp&time=120">2 minutes</a></li>
                        <li><a href="?func=itemp&time=180">3 minutes</a></li>
                        <li><a href="?func=itemp&time=240">4 minutes</a></li>
                        <li><a href="?func=itemp&time=300">5 minutes</a></li>
                    </ul>
                </div>

                <!-- Dropdown button for changing compliment updates -->
                <div class="btn-group">
                    <button class="btn btn-default dropdown-toggle" data-toggle="dropdown">Change Compliments <span class="caret"></span></button>
                    <ul class="dropdown-menu" role="menu">
                        <li><a href="?func=ccomp&time=60" >1 minute</a></li>
                        <li><a href="?func=ccomp&time=120">2 minutes</a></li>
                        <li><a href="?func=ccomp&time=180">3 minutes</a></li>
                        <li><a href="?func=ccomp&time=240">4 minutes</a></li>
                        <li><a href="?func=ccomp&time=300">5 minutes</a></li>
                        <li><a href="?func=ccomp&time=600">10 minutes</a></li>
                        <li><a href="?func=ccomp&time=900">15 minutes</a></li>
                    </ul>
                </div>
            </div>
        </div>
    </div>
    <!-- ==================================================================================================== -->
    <div class="row">
        <p><br/></p>
        <div class="col-md-2">
            <button class="btn btn-block btn-success" onclick="history.go(0)" id="refresh">Refresh Settings Page</button>
        </div>
        <div class="col-md-2 col-md-offset-6">
            <a href="?func=rfsmirror"><button class="btn btn-block btn-primary">Refresh Mirror</button></a>
        </div>
        <div class="col-md-2">
            <a href="?func=rbtmirror"><button class="btn btn-primary btn-block" onclick="rebootMirror()">Reboot Mirror</button></a>
        </div>
    </div>
    <!-- ==================================================================================================== -->
    <p></br></p>    <!-- Break inserted to make page look a little nicer -->
    <div class="footer" style="background-color: #EEEEEE;">
        <div class="container">
            <p><br/>Smart Mirror page is designed by Group 6A and 6B for the purpose of ELEC 291 L2C Project 2. For source code, please visit<br/><code>https://github.com/ineedthisforCPEN/ELEC_291_Projects/tree/Project2</code></p>
        </div>
    </div>
</body>
</html>
