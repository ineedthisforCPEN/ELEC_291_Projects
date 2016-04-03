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
            <p> Welcome to the smart mirror settings page. Here, you can see what data the smart mirror has received. You may also turn on or off mirror modules (e.g., you may turn off the TODO list if you wish).</p>
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
                    // If a compliment is added, display an alert that shows that the compliment was added successfully, as well as the compliment text
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Added \"" . $text . "\" to compliments list</a></div>";
                    break;
                case 'addcompfail':
                    // If a compliment could not be added, display and alert that shows that the compliment could not be added successfully
                    // There is only one reason why a compliment cannot be added - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to add compliment to compliments list - no compliment written in text box</a></div>";
                    break;
                case 'remcomp':
                    // If a compliment is removed, display an alert that shows that the compliment was removed successfully, as well as the compliment text
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Removed \"" . $text . "\" from compliments list</a></div>";
                    break;
                case 'remcompfail':
                    // If a compliment could not be removed, display and alert that shows that the compliment could not be removed successfully
                    // There is only one reason why a compliment cannot be removed - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to remove compliment from compliments list - no compliment selected</a></div>";
                    break;
                case 'fintask':
                    // If a task is completed, display an alert that shows which task was successfully completed
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Completed Task \"" . $text . "\"</a></div>";
                    break;
                case 'fintaskfail':
                    // If a task could not be completed, display an alert that shows that a task could not be completed
                    // There is only one reason why a task cannot be completed - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to complete task - no task selected</a></div>";
                    break;
                case 'rsttask':
                    // If a task is reset, display an alert that shows which task was successfully reset
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Reset Task \"" . $text . "\"</a></div>";
                    break;
                case 'rsttaskfail':
                    // If a task could not be reset, display an alert that shows that a task could not be reset
                    // There is only one reason why a task cannot be reset - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to reset task - no task selected</a></div>";
                    break;
                case 'addtask':
                    // If a task is added, display an alert that shows which task was successfully added
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Added Task \"" . $text . "\"</a></div>";
                    break;
                case 'addtaskfail':
                    // If a task could not be added, display an alert that shows that a task could not be added
                    // There is only one reason why a task cannot be added - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to add task - no task written in text box</a></div>";
                    break;
                case 'remtask':
                    // If a task is removed, display an alert that shows which task was successfully removed
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-success\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Removed Task \"" . $text . "\"</a></div>";
                    break;
                case 'remtaskfail':
                    // If a task could not be removed, display an alert that shows that a task could not be removed
                    // There is only one reason why a task cannot be removed - this reason is displayed on the alert
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-danger\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Failed to remove task - no task selected</a></div>";
                    break;
                case 'itemp':
                    // Make sure only acceptable values are input (this prevents users from inputting unreasonable values in the address bar)
                    if($updatetime % 30 != 0) { break; }

                    // Open and update the arduino_update file
                    $auFile = fopen("/var/www/wwwdata/update_times/arduino_update.
                        txt", "w") or die("Failed to change update time.<a onClick=\"location.href='../'\">Click here to go back</a>");
                    fwrite($auFile, $updatetime);
                    fclose($auFile);

                    // Display an alert that shows the user that the update interval has changed, as well as the new update interval
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-info\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Internal conditions will update once every " . formatTime($updatetime) . " - this will take effect after mirror reboot</a></div>";
                    break;
                case 'ccomp':
                    // Make sure only acceptable values are input (this prevents users from inputting unreasonable values in the address bar)
                    if($updatetime % 30 != 0) { break; }

                    // Open and update the compliment_update file
                    $coFile = fopen("/var/www/wwwdata/update_times/compliment_update.txt", "w") or die("Failed to change compliment change time.<a onClick=\"location.href='../'\">Click here to go back</a>");
                    fwrite($coFile, $updatetime);
                    fclose($coFile);

                    // Display an alert that shows the user that the update interval has changed, as well as the new update interval
                    // Exiting the alert will bring the user back to the main page
                    echo "<div class=\"alert alert-info\"><a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\" onClick=\"location.href='../'\">&times;</a>" .
                         "Compliments now change once every " . formatTime($updatetime) . " - this will take effect after mirror reboot</a></div>";
                    break;
                default:
                    // The default action is to do nothing
                    break;
            }
        ?>

        <!-- Message to users the let them know refreshing the page may solve some issues they may experience -->
        <p style="color: #888888"><small>Try refreshing the page if you see something wrong. This might fix the problem.</small></p>

        <!-- ==================================================================================================== -->
        <!-- BUTTONS FOR REFRESHING PAGE, REFRESHING MIRROR, REBOOTING MIRROR -->
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
        <!-- MAIN WEBSITE DESIGN -->
        <!-- ==================================================================================================== -->

        <div class="row">

            <!-- ==================================================================================================== -->
            <!-- LEFT COLUMN - TIME, WEATHER, ARDUINO MEASUREMENTS -->
            <!-- ==================================================================================================== -->

            <div class="col-md-4">
                <!-- This script gathers information from the files that have arduino sensor data -->
                <?php
                    $tfile = fopen("/var/www/wwwdata/temperature.txt", "r") or die("--");   // Open temperature file
                    $hfile = fopen("/var/www/wwwdata/humidity.txt", "r") or die("--");      // Open humidity file
                    $indoorTemp = fgets($tfile);                                            // Read line from temperature file
                    $indoorHumi = fgets($hfile);                                            // Read line form humidity file
                    fclose($tfile);                                                         // Close temperature file
                    fclose($hfile);                                                         // Close humidity file
                ?>


	           <!-- Section for time (By James) -->
                <div class="page-header">
                    <h2>Time</h2>
                </div>
                <div class="well well-sm">
            		<p align="center" style="padding-center: 5%; font-size: 350%;">
                        <script>
                            var x = new Date();
                            var time = 'AM';
                            var chara = '0';
                            var hours = x.getHours();
                            var minutes = x.getMinutes().toString();
                            var array1 = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
                            var array2 = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'];
                            
                            if(x.getHours() > 12) {
                                time = 'PM';
                                hours -= 12;
                            }
            		    
                            if(x.getMinutes() < 10) {
                                minutes = chara.concat(minutes);
                            }
            				
                            document.write(hours + ":" + minutes + " " + time);
                        </script>
                    </p>
                    <div class="panel-group">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" href="#extratimeinfo">Click For Extra Information</a>
                                </h4>
                            </div>
                            <div id="extratimeinfo" class="panel-collapse collapse">
				                <div class="panel-body">Date:
                                    <span style="float: right;">
                                        <script>
                                            document.write(array2[x.getDay()] + "," + " " + array1[x.getMonth()] + " " + x.getDate() + "," + " " +  x.getFullYear());
                                        </script>
                                    </span>
                                </div>
                                <div class="panel-body">Time Counter:
                                    <span style="float: right;">
                                        <script></script>
                                    </span>
                                </div> 
                            </div>
                        </div>
                    </div>
                </div>

                <!-- ==================================================================================================== -->
                <!-- WEATHER -->
                <!-- ==================================================================================================== -->
                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>Weather</h2>
                </div>
                <div class="well well-sm">
                    <!-- Script for getting weather data from the openweathermap API -->
                    <?php
                        // Request data from the openweathermap API - limit is 60 requests per hour
                        //$request = 'http://api.openweathermap.org/data/2.5/weather?q=Vancouver,can&appid=ce1664b9a59f64885ad5bcfc8ee4f1a4';
                        //$response  = file_get_contents($request);
                        //$jsonobj  = json_decode($response, true);

                        // This code is useful for testing - it will not lock our website out of the openweathermap API
                        $jsonobj = json_decode('{"coord":{"lon":-123.12,"lat":49.25},"weather":[{"id":802,"main":"Clouds","description":"scattered clouds","icon":"03n"}],"base":"cmc stations","main":{"temp":280.45,"pressure":1017,"humidity":81,"temp_min":276.65,"temp_max":282.15},"wind":{"speed":6.7,"deg":210},"clouds":{"all":40},"dt":1459142270,"sys":{"type":1,"id":3359,"message":0.0136,"country":"CA","sunrise":1459173385,"sunset":1459219138},"id":6173331,"name":"Vancouver","cod":200}', true);
                        
                        // Get city name from the JSON object
                        $owmCity = $jsonobj['name'];

                        // coord objects
                        // Gets longitude and latitude from JSON object
                        $owmLatitude = $jsonobj['coord']['lat'];
                        $owmLongitude = $jsonobj['coord']['lon'];

                        // weather objects
                        // Gets conditions as well as extra weather descriptions from JSON object
                        $owmCondition = $jsonobj['weather'][0]['main'];
                        $owmConditionExtra = $jsonobj['weather'][0]['description'];

                        // main objects
                        // Gets temperature (max, min, current), humidity, and pressure from JSON object
                        $owmTemperature = ($jsonobj['main']['temp'] - 273.15);
                        $owmPressure = $jsonobj['main']['pressure'];
                        $owmHumidity = $jsonobj['main']['humidity'];
                        $owmTempMin = ($jsonobj['main']['temp_min'] - 273.15);
                        $owmTempMax = ($jsonobj['main']['temp_max'] - 273.15);
                        
                        // sys objects
                        // Gets country, sunset time, and sunrise time from JSON object
                        $owmCountry = $jsonobj['sys']['country'];
                        $owmSunrise = $jsonobj['sys']['sunrise'];
                        $owmSunset = $jsonobj['sys']['sunset'];
                    ?>
                    <div class="row">
                        <!-- Dispay temperature (current, max, min) and conditions -->
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
                    <!-- Display additional information in a drop-down menu (i.e. not visible unless menu is expanded) -->
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
                                <div class="panel-body">Sunset:<span style="float: right;"><?php echo date('h:i a', $owmSunset); ?></span></div>
                                <div class="panel-body">Pressure:<span style="float: right;"><?php echo $owmPressure . " "; ?>kPa</span></div>
                                <div class="panel-body">Humidity:<span style="float: right;"><?php echo $owmHumidity . " "; ?>%</span></div>
                            </div>
                        </div>
                    </div>
                </div>

                <!-- ==================================================================================================== -->
                <!-- ARDUINO MEASUREMENTS -->
                <!-- ==================================================================================================== -->
                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>Mirror Measurements</h2>
                </div>
                <div class="well well-sm">
                    <h4>Current Indoor Temperature<small style="float: right;"><?php echo $indoorTemp; ?>&#8451</small></h4>
                    <h4>Current Indoor Humidity<small style="float: right;"><?php echo $indoorHumi; ?>%</small></h4>
                </div>
            </div>

            <!-- ==================================================================================================== -->
            <!-- MIDDLE COLUMN - COMPLIMENTS, TODO LIST -->
            <!-- ==================================================================================================== -->

            <div class="col-md-5">
                <!-- ==================================================================================================== -->
                <!-- COMPLIMENTS -->
                <!-- ==================================================================================================== -->
                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>List of Compliments</h2>
                </div>
                <!-- Create a form that will send information to a php script -->
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

                            // Re-open file to read from the beginning
                            // Display every line except last one (last line is always a newline character i.e. empty)
                            $cfile = fopen("/var/www/wwwdata/compliment_list.txt", "r") or die("<p>-- You have no compliments --</p>");
                            $cindex = 1;
                            while(--$numlines) {
                                //
                                echo "<div class='radio'><label><input type='radio' name='item' value='" . $cindex . "''> " . fgets($cfile) . "</label></div>";
                                $cindex++;
                            }
                            fclose($cfile);
                        ?>
                    </div>
                    <div class="well well-sm">
                        <!-- Input text - allows user to write their own compliment here -->
                        <div class="form-group">
                            <label for="Compliment">Your Compliment:</label>
                            <input type="text" class="form-control" name="text">
                        </div>
                    </div>
                    <div class="btn-group btn-group-justified">
                        <!-- Buttons that will run the php script - one adds a compliment, one removes a compliment -->
                        <div class="btn-group"><button type="submit" name="func" value="add" class="btn btn-default">Add Custom Compliment</button></div>
                        <div class="btn-group"><button type="submit" name="func" value="rem" class="btn btn-default">Remove Selected Compliment</button></div>
                    </div>
                </form>

                <!-- ==================================================================================================== -->
                <!-- TODO LIST -->
                <!-- ==================================================================================================== -->
                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>TODO List</h2>
                </div>
                <div class="well well-sm">
                <!-- Create a form that will send information to a php script -->
                    <form action="todoUpdate.php" method="post">
                        <!-- Display every todo item in the todolist.txt file -->
                        <!-- Every item gets its own line -->
                        <!-- Items that have been marked done are crossed through -->
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
                                    // If item is not completed, display it as normal text
                                    echo "<div class='radio'><label><input type='radio' name='item' value='" . $tdindex . "'> " . $rest . "</label></div>";
                                } else {
                                    // If item is completed, make text crossed out
                                    echo "<div class='radio'><label><input type='radio' name='item' value='" . $tdindex . "'><del> " . $rest . "</del></label></div>";
                                }
                                $tdindex++;
                            }
                            fclose($tdfile);
                        ?>
                        <div class="form-group">
                            <!-- Input text - allows user to write their own task here -->
                            <label for="Todo">Your TODO Item:</label>
                            <input type="text" class="form-control" name="text">
                        </div>
                        <div class="btn-group btn-group-justified">
                            <!-- Buttons that will run the php script - one finished as task, one resets a task, one adds a task, one removes a task -->
                            <div class='btn-group'><button type="submit" name="func" value="fin" class="btn btn-default">Complete Task</button></div>
                            <div class='btn-group'><button type="submit" name="func" value="rst" class="btn btn-default">Reset Task</button></div>
                            <div class='btn-group'><button type="submit" name="func" value="add" class="btn btn-default">Add Task</button></div>
                            <div class='btn-group'><button type="submit" name="func" value="rem" class="btn btn-default">Remove Task</button></div>
                        </div>
                    </form>
                </div>
            </div>

            <!-- ==================================================================================================== -->
            <!-- RIGHT COLUMN - MODULES DISPLAYED, UPDATE TIMES -->
            <!-- ==================================================================================================== -->

            <div class="col-md-3">
                <!-- ==================================================================================================== -->
                <!-- MODULES DISPLAYED -->
                <!-- ==================================================================================================== -->
                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>Modules Displayed</h2>
                </div>
                <!-- Warn users that, in order for this to take effect, the mirror must be refreshed -->
                <span class="label label-warning">Requires Mirror Refresh</span>
                <div class="well well-sm">
                    <!-- Create a form that will send information to a php script -->
                    <form action="moduleSelect.php" method="post">
                        <?php
                            // Try to open file where displayed modules are written
                            $msfile=fopen("/var/www/wwwdata/module_select.txt", "r") or die("Error loading modules. <a onClick=\"location.href='../'\">Click here to go back</a>");
                            $modArray=array();

                            // For each module in the module_select file, add it to an array modArray
                            while(!feof($msfile)) {
                                $line=fgets($msfile);
                                $modArray[]=$line;
                            }
                            
                            fclose($msfile);

                            // Display checkbox for time module
                            echo "<p><input type='checkbox' name='module[]' value='time'";
                            if (in_array("time\n", $modArray)) { echo " checked='' "; }         // Checkbox is checked if the time module is being displayed on the mirror
                            echo "> <b>Time</b> <small>Module</small></p>";

                            // Display checkbox for arduino module
                            echo "<p><input type='checkbox' name='module[]' value='itemp'";
                            if (in_array("itemp\n", $modArray)) { echo " checked='' "; }        // Checkbox is checked if the arduino module is being displayed on the mirror
                            echo "> <b>Indoor Conditions</b> <small>Module</small></p>";

                            // Display checkbox for weather module
                            echo "<p><input type='checkbox' name='module[]' value='weather'";
                            if (in_array("weather\n", $modArray)) { echo " checked='' "; }      // Checkbox is checked if the weather module is being displayed on the mirror
                            echo "> <b>Weather</b> <small>Module</small></p>";

                            // Display checkbox for todo list module
                            echo "<p><input type='checkbox' name='module[]' value='todo'";
                            if (in_array("todo\n", $modArray)) { echo " checked='' "; }         // Checkbox is checked if the todo list module is being displayed on the mirror
                            echo "> <b>Todo</b> <small>Module</small></p>";

                            // Display checkbox for compliments module
                            echo "<p><input type='checkbox' name='module[]' value='compliments'";
                            if (in_array("compliments\n", $modArray)) { echo " checked='' "; }  // Checkbox is checked if the compliments module is being displayed on the mirror
                            echo "> <b>Compliments</b> <small>Module</small></p>";
                        ?>
                        <!-- Button that sends this data to the php script -->
                        <button type="submit" class="btn btn-default btn-block">Refresh Mirror</button>
                    </form>
                </div>

                <!-- ==================================================================================================== -->
                <!-- UPDATE TIMES -->
                <!-- ==================================================================================================== -->

                <div class="page-header">
                    <!-- Display header to let users know what data lies below -->
                    <h2>Update Times</h2>
                </div>
                <div class="well well-sm">
                    <!-- Dropdown button for changing arduino update time -->
                    <div class="btn-group">
                        <button class="btn btn-default dropdown-toggle btn-block" data-toggle="dropdown">Indoor Temperature <span class="caret"></span></button>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="?func=itemp&time=30" >30 seconds</a></li>      <!-- Update arduino measurements once every 30 seconds -->
                            <li><a href="?func=itemp&time=60" >1 minute</a></li>        <!-- Update arduino measurements once every minutes    -->
                            <li><a href="?func=itemp&time=120">2 minutes</a></li>       <!-- Update arduino measurements once every 2 minutes  -->
                            <li><a href="?func=itemp&time=180">3 minutes</a></li>       <!-- Update arduino measurements once every 3 minutes  -->
                            <li><a href="?func=itemp&time=240">4 minutes</a></li>       <!-- Update arduino measurements once every 4 minutes  -->
                            <li><a href="?func=itemp&time=300">5 minutes</a></li>       <!-- Update arduino measurements once every 5 minutes  -->
                        </ul>
                    </div>

                    <!-- Dropdown button for changing compliment updates -->
                    <div class="btn-group">
                        <button class="btn btn-default dropdown-toggle" data-toggle="dropdown">Change Compliments <span class="caret"></span></button>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="?func=ccomp&time=60" >1 minute</a></li>        <!-- Update compliment once every minute     -->
                            <li><a href="?func=ccomp&time=120">2 minutes</a></li>       <!-- Update compliment once every 2 minutes  -->
                            <li><a href="?func=ccomp&time=180">3 minutes</a></li>       <!-- Update compliment once every 3 minutes  -->
                            <li><a href="?func=ccomp&time=240">4 minutes</a></li>       <!-- Update compliment once every 4 minutes  -->
                            <li><a href="?func=ccomp&time=300">5 minutes</a></li>       <!-- Update compliment once every 5 minutes  -->
                            <li><a href="?func=ccomp&time=600">10 minutes</a></li>      <!-- Update compliment once every 10 minutes -->
                            <li><a href="?func=ccomp&time=900">15 minutes</a></li>      <!-- Update compliment once every 15 minutes -->
                        </ul>
                    </div>
                </div>
                
                <!-- ==================================================================================================== -->
                <!-- Translink -->
                <!-- ==================================================================================================== -->
        
                <div class="page-header">
                <!-- Display header to let users know what data lies below -->
                    <h2>Bus Info</h2>
                </div>
                <div class="well well-sm">
                    <!-- Script for getting transit data from the openweathermap API -->
			<?php 
			    $headers = array('Content-Type: application/json',);
			    //Request data from the Translink API - limit is 10 000 requests per day
			  $request = 'http://api.translink.ca/rttiapi/v1/stops/59270/estimates?apikey=1Y8IBRRxW0yYIhxyWswH';
			 
			    $response  = file_get_contents($request);

                        $jsonobj  = json_decode($response, true);

                        // This code is useful for testing - Translink API
                        //$jsonobj = json_decode('{"RouteNo":"099","RouteName":"COMMERCIAL-BROADWAY\/UBC (B-LINE)  ","Direction":"EAST","RouteMap":{"Href":"http:\/\/nb.translink.ca\/geodata\/099.kmz"},"Schedules":[{"Pattern":"EB1","Destination":"COMM\'L-BDWAY STN","ExpectedLeaveTime":"6.42pm 2016-04-01","ExpectedCountdown":-2,"ScheduleStatus":" ","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06:38:04 pm"},{"Pattern":"EB1","Destination":"COMM\'L-BDWAY STN","ExpectedLeaveTime":"6:47pm 2016-04-01","ExpectedCountdown":3,"ScheduleStatus":" ","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06:39:17 pm"},{"Pattern":"EB1","Destination":"COMM\'L-BDWAY STN","ExpectedLeaveTime":"6.56pm 2016-04-01","ExpectedCountdown":12,"ScheduleStatus":"-","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06.43.47 pm"},{"Pattern":"EB1","Destination":"COMM\'L-BDWAY STN","ExpectedLeaveTime":"7:00pm 2016-04-01","ExpectedCountdown":16,"ScheduleStatus":"-","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06:37:20 pm"},{"Pattern":"EB3","Destination":"TO BOUNDARY B-LINE","ExpectedLeaveTime":"7:03pm 2016-04-01","ExpectedCountdown":19,"ScheduleStatus":"-","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06:43:07 pm"},{"Pattern":"EB3","Destination":"TO BOUNDARY B-LINE","ExpectedLeaveTime":"7:07pm 2016-04-01","ExpectedCountdown":23,"ScheduleStatus":"*","CancelledTrip":false,"CancelledStop":false,"AddedTrip":false,"AddedStop":false,"LastUpdate":"06:07:02 pm"}]}', true);
                                    
                        // Get city name from the JSON object
                        $route = $jsonobj['RouteNo'];

                        // Get route name
                        $routeName = $jsonobj['RouteName'];
                        
                        // Get bus destination
                        $destination = $jsonobj['Schedules'][0]['Destination'];

                        // Obtain the estimated departure times for the next 6 buses
                        $bus0 = $jsonobj['Schedules'][0]['ExpectedCountdown'];
                        $bus1 = $jsonobj['Schedules'][1]['ExpectedCountdown'];
                        $bus2 = $jsonobj['Schedules'][2]['ExpectedCountdown'];
                        $bus3 = $jsonobj['Schedules'][3]['ExpectedCountdown'];
                        $bus4 = $jsonobj['Schedules'][4]['ExpectedCountdown'];
                        $bus5 = $jsonobj['Schedules'][5]['ExpectedCountdown'];
                    ?>
                    <div class="row">
                        <!-- Dispay temperature (current, max, min) and conditions -->
                        <div class="col-md-4">
                           
                            <p style="padding-left: 5%; font-size: 120%; position: relative; bottom: 15px; color: #888888;"><small><?php echo "Route: " . $routeName;  ?></small></p>
                        </div>
			<div class="col-md-8">
			    <p align="right" style="padding-right: 5%; font-size: 120%; color: #888888;"><small>UBC Loop Bay 6</small></p>
                            <p align="right" style="padding-right: 5%; font-size: 200%;"><?php echo $route; ?></p>
                            <p align="right" style="padding-right: 5%; font-size: 100%;">First Bus Arrival Time:</p>
                            <p align="right" style="padding-right: 5%; font-size: 300%;"><?php echo $bus1; ?></p>
                        </div>
                    </div>
                  
                    <!-- Display additional information in a drop-down menu (i.e. not visible unless menu is expanded) -->
                    <div class="panel-group">
                        <div class="panel panel-default">
                            <div class="panel-heading">
                                <h4 class="panel-title">
                                    <a data-toggle="collapse" href="#extratranslinkinfo">More Departure Times</a>
                                </h4>
                            </div>
                            <div id="extratranslinkinfo" class="panel-collapse collapse">
                                <div class="panel-body">Second bus leave time:<span style="float: right;"><?php echo $bus1 . " "; ?>min</span></div>
                                <div class="panel-body">Third bus leave time:<span style="float: right;"><?php echo $bus2 . " "; ?>min</span></div>
                                <div class="panel-body">Fourth bus leave time:<span style="float: right;"><?php echo $bus3 . " "; ?>min</span></div>
                                <div class="panel-body">Fifth bus leave time:<span style="float: right;"><?php echo $bus4 . " "; ?>min</span></div>
                                <div class="panel-body">Sixth bus leave time:<span style="float: right;"><?php echo $bus5 . " "; ?>min</span></div>
                            </div>    
                        </div>
                    </div>
                </div>
            </div>
	   </div>
    
        <!-- ==================================================================================================== -->
        <!-- BUTTONS FOR REFRESHING PAGE, REFRESHING MIRROR, REBOOTING MIRROR -->
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
        <!-- FOOTER THAT PROVIDES THE USED WITH ADDITIONAL, NON-CRITIAL INFORMATION  -->
        <!-- ==================================================================================================== -->

        <p></br></p>    <!-- Break inserted to make page look a little nicer -->
        <div class="footer" style="background-color: #EEEEEE;">
            <div class="container">
                <p><br/>Smart Mirror page is designed by Group 6A and 6B for the purpose of ELEC 291 L2C Project 2. For source code, please visit<br/><code>https://github.com/ineedthisforCPEN/ELEC_291_Projects/tree/Project2</code></p>
            </div>
        </div>
    </div>
</body>
</html>
