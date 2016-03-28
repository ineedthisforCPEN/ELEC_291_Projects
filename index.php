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
    <!-- jQuery scripts used to run php scripts that change server files -->
    <script>
        // This is a helper function for updateArduino()
        function timeOutput(time) {
            if (time < 0) {                 // Return 0 if time is less than zero
                return "0 seconds";
            } else if (time > 24*3600) {    // Return max value if input is larger than one day
                return "day";
            } else {                        // Return time in minutes and seconds otherwise
                var minutes = 0;
                while (time > 60) {
                    time -= 60;
                    minutes++;
                }
                return "" + minutes + " minutes and " + time + " seconds";
            }
        }

        // This function runs the php script rebootMirror.php, sends no parameters
        function rebootMirror() {
            $.ajax({
                url: './phpscripts/rebootMirror.php',       // Location of php script
                type: 'post',
                success: function() {
                    alert("Rebooting mirror. Please wait.");
                }
            });
            return false;
        }

        // This function runs the php script refreshMirror.php, sends no parameters
        function refreshMirror() {
            $.ajax({
                url: './phpscripts/refreshMirror.php'       // Location of php script
                type: 'post',
                success: function() {
                    alert("Refreshing mirror. Please wait.");
                }
            });
            return false;
        }

        // This function runs the php script updateArduino.php, sends parameter "updateTime"
        function updateArduino(updateTime) {
            $.ajax({
                url: './phpscripts/updateArduino.php',      // Location of php script
                data: {updateTime: 'updateTime'},           // What parameters to send
                type: 'post',
                success: function() {
                    alert("Changed indoor temperature data to update once every "
                        + timeOutput(updateTime)) + ".";
                }
            });
            return false;
        }

        // This function runs the php script updateCompliments.php, sends parameter "updateTime"
        function updateCompliments(updateTime) {
            $.ajax({
                url: './phpscripts/updateCompliments.php',  // Location of php script
                data: {updateTime: 'updateTime'},           // What parameters to send
                type: 'post',
                success: function() {
                    alert("Displaying random compliment once every "
                        + timeOutput(updateTime)) + ".";
                }
            });
            return false;
        }

        // This function runs updateTODOList.php, sends "status" and "toDo" parameters
        function TODO(status, toDo) {
            $.ajax({
                url: './phpscripts/updateTODOList.php',     // Location of php script
                data: {status: 'status', toDo: 'toDo'},     // What parameters to send
                type: 'post',
                success: function(output) {
                    alert(output);
                }
            });
            return false;
        }

        // This function runs addCompliment.php, sends parameter "compliment"
        function addCompliment(compliment) {
            $.ajax({
                url: './phpscripts/addCompliment.php',      // Location of php script
                data: {compliment: 'compliment'},           // What parameters to send
                type: 'post',
                success: function() {
                    alert("Added \"" + compliment + "\" to compliment list.");
                }
            });
            return false;
        }

        // This function runs removeCompliment.php, sends parameter "index"
        function removeCompliment(index) {
            $.ajax({
                url: './phpscripts/removeCompliment.php',   // Location of php script
                data: {action: 'test'},                     // What parameters to send
                type: 'post',
                success: function() {
                    alert("Removed the seleted compliment.");
                }
            });
            return false;
        }
    </script>
    <!-- ==================================================================================================== -->
    <div class="container">
        <!-- ==================================================================================================== -->
        <div class="jumbotron">
            <!--
            Read info from ip.txt
            This reads the website address
            -->
             <?php
                $ipv4file = fopen("/var/www/html/ip.txt", "r") or die("xxx.xxx.xxx.xxx");
                $currentipv4 = fread($ipv4file,filesize("/var/www/html/ip.txt"));
                fclose($ipv4file);
            ?> 

            <h1>Smart Mirror Settings</h1>
            <p> Welcome to the smart mirror settings page. Here, you can see what data the smart mirror has received. You may also turn on or off mirror modules (e.g., you may turn off the TODO list if you wish).
            <p>Current Web Address: <kbd><?php echo $currentipv4; ?></kbd></p>
        </div>
        <p style="color: #888888"><small>Try refreshing the page if you see something wrong. This might fix the problem.</small></p>
        <!-- ==================================================================================================== -->
        <div class="row">
            <div class="col-md-2">
                <button class="btn btn-block btn-success" onclick="history.go(0)" id="refresh">Refresh Settings Page</button>
            </div>
            <div class="col-md-2 col-md-offset-6">
                <button class="btn btn-block btn-primary" onclick="refreshMirror()">Refresh Mirror</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-primary btn-block" onclick="rebootMirror()">Reboot Mirror</button>
            </div>
        </div>
        <!-- ==================================================================================================== -->
        <div class="row">
            <!-- ==================================================================================================== -->

            <!-- Column for weather, internal temperature, and other measurements-->
            <div class="col-md-4">
                <!-- This script gathers information from the arduino -->
                <?php
                    $tfile = fopen("/var/www/html/temperature.txt", "r") or die("--");  // Open temperature file
                    $hfile = fopen("/var/www/html/humidity.txt", "r") or die("--");     // Open humidity file
                    $indoorTemp = fgets($tfile);                                        // Read line from temperature file
                    $indoorHumi = fgets($hfile);                                        // Read line form humidity file
                    fclose($tfile);                                                     // Close temperature file
                    fclose($hfile);                                                     // Close humidity file
                ?>

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
                <div class="well well-sm">
                    <!-- Display every compliment in the compliments.txt file -->
                    <!-- Each compliment gets its own line -->
                    <?php
                        $cfile = fopen("/var/www/html/compliments.txt", "r") or die("<p>-- You have no compliments --</p>");
                        $cindex = 0;
                        while(! feof($cfile)) {
                            echo "<p><input type=\"checkbox\" value=\"" . $cindex . "\"> " . fgets($cfile) . "</p>";     // Print next line of the file
                            $cindex++;
                        }
                        fclose($cfile);
                    ?>
                </div>
                <div class="well well-sm">
                    <div class="form-group">
                        <label for="Compliment">Your Compliment:</label>
                        <input type="Compliment" class="form-control" id="Compliment">
                    </div>
                </div>
                <div class="btn-group btn-group-justified">
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="addCompliment()">Add Custom Compliment</a></button></div>
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="removeCompliment()">Remove Selected Compliment</a></button></div>
                </div>


                <!-- One section for the todo list -->
                <div class="page-header">
                    <h2>TODO List</h2>
                </div>
                <div class="well well-sm">
                    <!-- Display every todo item in the todolist.txt file -->
                    <!-- Every item gets its own line -->
                    <!-- Items that have been marked done are crossed through -->
                    <?php
                        $tdfile = fopen("/var/www/html/todolist.txt", "r");
                        $tdindex = 0;
                        while (! feof($tdfile)) {
                            $line = fgets($tdfile);
                            $linearray = explode(' ', trim($line));
                            $first = $linearray[0];
                            $rest = substr(strstr($line, ' '), 4);
                            $tdindex = 0;
                            if (strcmp($first, "todo") == 0) {
                                echo "<p><input type=\"checkbox\" value=\"" . $tdindex . "\"> " . $rest . "</p>";
                            } else {
                                echo "<p><input type=\"checkbox\" value=\"" . $tdindex . "\"><del> " . $rest . "</del></s>";
                            }
                            $tdindex++;
                        }
                        fclose($tdfile);
                    ?>
                </div>
                <div class="well well-sm">
                    <div class="form-group">
                        <label for="Todo">Your TODO Item:</label>
                        <input type="Todo" class="form-control" id="Todo">
                    </div>
                </div>
                <div class="btn-group btn-group-justified">
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="TODO("finish")">Finished Task</a></button></div>
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="TODO("unfinish")">Reset Task</a></button></div>
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="TODO("add")">Add Task</a></button></div>
                    <div class="btn-group"><button class="btn btn-default"><a href="" onclick="TODO("remove")">Remove Task</a></button></div>
                </div>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for changing settings (drop down menus, checkboxes and buttons) -->
            <div class="col-md-3">
                <!-- Turn mirror modules on and off -->
                <div class="page-header">
                    <h2>Modules Displayed</h2>
                </div>
                <span class="label label-warning">Requires Mirror Refresh or Reboot</span>
                <div class="well well-sm">
                    <form>
                        <p><input type="checkbox" value="time"> <b>Time</b> <small>Module</small></p>
                        <p><input type="checkbox" value="itemp"> <b>Indoor Temperatue</b> <small>Module</small></p>
                        <p><input type="checkbox" value="weather"> <b>Weather</b> <small>Module</small></p>
                        <p><input type="checkbox" value="todo"> <b>Todo</b> <small>Module</small></p>
                        <p><input type="checkbox" value="compliments"> <b>Compliments</b> <small>Module</small></p>
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
                            <li><a href="" onclick="updateArduino(30)">30 seconds</a></li>
                            <li><a href="" onclick="updateArduino(60)">1 minute</a></li>
                            <li><a href="" onclick="updateArduino(120)">2 minutes</a></li>
                            <li><a href="" onclick="updateArduino(180)">3 minutes</a></li>
                            <li><a href="" onclick="updateArduino(240)">4 minutes</a></li>
                            <li><a href="" onclick="updateArduino(300)">5 minutes</a></li>
                        </ul>
                    </div>

                    <!-- Dropdown button for changing compliment updates -->
                    <div class="btn-group">
                        <button class="btn btn-default dropdown-toggle" data-toggle="dropdown">Change Compliments <span class="caret"></span></button>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="" onclick="updateCompliments(60)">1 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">2 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">3 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">4 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">5 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">10 minute</a></li>
                            <li><a href="" onclick="updateCompliments(60)">15 minute</a></li>
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
            <button class="btn btn-block btn-primary" onclick="refreshMirror()">Refresh Mirror</button>
        </div>
        <div class="col-md-2">
            <button class="btn btn-primary btn-block" onclick="rebootMirror()">Reboot Mirror</button>
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