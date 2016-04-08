#!/bin/bash
# This script handles everything that needs to be done during th Raspberry Pi's startup
# This includes:
#	Starting SSH server (and displaying RSA and DSA keys
#	Starting the apache server

#--------------------------------------------------
# START SSH SERVER
#--------------------------------------------------
SERVER_ADDR4=$(ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1 }')
SERVER_ADDR6=$(ifconfig wlan0 | grep 'inet6 addr:' | awk '{ print $3 }')
RSA_HOST_KEY=$(ssh-keygen -l -f /etc/ssh/ssh_host_rsa_key | awk '{print $2}')
DSA_HOST_KEY=$(ssh-keygen -l -f /etc/ssh/ssh_host_dsa_key | awk '{print $2}')
echo $SERVER_ADDR4 > /var/www/wwwdata/ip.txt

echo -e "$(tput bold)========================================$(tput sgr0)"
echo -e "Starting SSH server"
sudo /etc/init.d/ssh start
echo -e "SSH server started and running\n"
echo -e "Server name:     IPv4 - $(tput setaf 2)pi@$SERVER_ADDR4$(tput sgr0)"
echo -e "                 IPv6 - $(tput setaf 3)[$SERVER_ADDR6]$(tput sgr0)\n"
echo -e "Website Address: $(tput setaf 6)$SERVER_ADDR4$(tput sgr0)"
echo -e "Host RSA Key:    $(tput setaf 2)$RSA_HOST_KEY$(tput sgr0)"
echo -e "Host DSA Key:    $(tput setaf 2)$DSA_HOST_KEY$(tput sgr0)"

#--------------------------------------------------
# START APACHE SERVER
#--------------------------------------------------

echo -e "$(tput bold)========================================$(tput sgr0)"
echo -e "Starting apache server...\n"
sudo apachectl start
echo -e "\nApache server started and running"

echo -e "$(tput bold)========================================$(tput sgr0)"

#--------------------------------------------------
# BEGIN RUNNING THE UPDATE AND RFSRBTCHECK SCRIPTS
#--------------------------------------------------

sudo /home/pi/update_script &   # Update script - updates arduino values and compliment
sudo /home/pi/frsrbtcheck &     # Checks if the website wants the mirror to reboot or refresh


#--------------------------------------------------
# WRITE FILE TO ENSURE ALL MODULES ARE IN THE
# MODULE_SELECT FILE
#--------------------------------------------------

if [ ! -f "/var/www/wwwdata/module_select.txt" ]
then
    touch /var/www/wwwdata/module_select.txt"
fi

echo -e "time\nitmep\\nweather\ntodo\ncompliments" > /var/www/wwwdata/module_select.txt
