#!/bin/bash

file ="/var/www/wwwdata/rstflag"
if [-f "$file" ] 
then
	/home/pi/bin/conky_restart	
fi

file ="/var/www/wwwdata/rbtflag"
if [-f "$file" ] 
then
	sudo reboot
fi

