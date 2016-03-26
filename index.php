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

    <!--Set up bootstrap from external sources -->
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js"></script>
    <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>
</head>
<body>
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
        <div class="row">
            <!-- ==================================================================================================== -->

            <!-- Column for weather, internal temperature, and other measurements-->
            <div class="col-md-4">
                <!-- Row for weather -->
                <div class="page-header">
                    <h2>Weather</h2>
                </div>
                <div class="well well-sm">
                    <p>Weather</p>
                </div>

                <!-- Internal temperature information goes here -->
                <div class="page-header">
                    <h2>Internal Temperature</h2>
                </div>
                <div class="well well-sm">
                    <p>Internal Temperature</p>
                </div>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for compliments, todo list, and any other entries that might be needed -->
            <div class="col-md-5">
                <!-- One row for the compliments -->
                <div class="page-header">
                    <h2>List of Compliments</h2>
                </div>
                <div class="well well-sm">
                    <p>So pretty!</p>
                    <p>Beautiful!</p>
                    <p>Dayum, gurl...</p>
                </div>

                <!-- One row for the todo list -->
                <div class="page-header">
                    <h2>TODO List</h2>
                </div>
                <div class="well well-sm">
                    <p>Buy food</p>
                    <p>Don't starve</p>
                    <p>Seriously, buy food!</p>
                    <p>Stop browsing 4chan</p>
                </div>
            </div>

            <!-- ==================================================================================================== -->

            <!-- Column for changing settings (drop down menus, checkboxes and buttons) -->
            <div class="col-md-3">
                <div class="page-header">
                    <h2>Settings</h2>
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
            </div>

            <!-- ==================================================================================================== -->
        </div>
    </div>
</body>
</html>

<!--http://www.accuweather.com/en/free-weather-widgets-->
<!--http://100widgets.com-->