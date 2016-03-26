<!DOCTYPE html>
<html>
<head>  
    <title>Smart Mirror Settings</title>
    <meta charset="utf-8">

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
        <div class="jumbotron">
            <!-- ==================================================================================================== -->
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
        <div class="row">
            <!-- ==================================================================================================== -->

            <!-- Column for weather, internal temperature, and other measurements-->
            <div class="col-md-4">
                <!-- Section for weather -->
                <div class="page-header">
                    <h2>Weather</h2>
                </div>
                <div class="well well-sm">
                    <p>Weather</p>
                </div>

                <!-- Section for local data -->
                <div class="page-header">
                    <h2>Indoor Temperature</h2>
                </div>
                <div class="well well-sm">
                    <p>Indoor Temperature</p>
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
                            while(! feof($cfile)) {
                                echo "<p>" . fgets($cfile) . "</p>";     // Print next line of the file
                            }
                            fclose($cfile);
                        ?>
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
                        while (! feof($tdfile)) {
                            $line = fgets($tdfile);
                            $linearray = explode(' ', trim($line));
                            $first = $linearray[0];
                            $rest = substr(strstr($line, ' '), 1);
                            if (strcmp($first, "todo") == 0) {
                                echo "<p>" . $rest . "</p>";
                            } else {
                                echo "<p><del>" . $rest . "</del></s>";
                            }
                        }
                        fclose($tdfile);
                    ?>
                </div>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for changing settings (drop down menus, checkboxes and buttons) -->
            <div class="col-md-3">
                <!-- Turn mirror modules on and off -->
                <div class="page-header">
                    <h2>Modules Displayed</h2>
                </div>
                <div class="well well-sm">
                    <p>Setting 0</p>
                    <p>Setting 1</p>
                    <p>Setting 2</p>
                    <p>Setting 3</p>
                    <p>Setting 4</p>
                    <p>Setting 5</p>
                    <p>Setting 6</p>
                    <p>Setting 7</p>
                    <p>Setting 8</p>
                    <p>Setting 9</p>
                </div>

                <!-- Set update times -->
                <div class="page-header">
                    <h2>Update Times</h2>
                </div>
                <div class="well well-sm">
                    <div class="btn-group">
                        <button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown">Indoor Temperature<span class="caret"></span></button>
                        <ul class="dropdown-menu" role="menu">
                        <!--
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(30)">30 seconds</a></li>
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(60)">1 minute</a></li>
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(120)">2 minutes</a></li>
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(180)">3 minutes</a></li>
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(240)">4 minutes</a></li>
                            <li role="presentation"><a role="menu-item" href="#" onclick="updateArduino(300)">5 minutes</a></li>
                        -->
                        </ul>
                    </div>
            </div>

            <!-- ==================================================================================================== -->

            <div class="row">
                <button type="button" class="btn btn-default pull-right">Reboot SmartMirror</button>
            </div>
        </div>
    </div>
</body>
</html>

<!--http://www.accuweather.com/en/free-weather-widgets-->
<!--http://100widgets.com-->