#!/bin/bash

# Reset color
Color_Off='\033[0m'

# Regular Colors
Purple='\033[0;35m'

# Check for root privileges (for sudo)
if ! sudo -v &> /dev/null; then
    echo "This script requires sudo privileges to install."
    exit 1
fi

# Download craxfetch
echo "Downloading craxfetch..."
wget -q https://github.com/shaneloy/craxfetch/raw/main/craxfetch -O /tmp/craxfetch

if [ $? -ne 0 ]; then
    echo "Error downloading craxfetch. Please check the URL and your internet connection."
    exit 1
fi

# Move and make executable
echo "Installing craxfetch to /usr/local/bin..."
sudo mv /tmp/craxfetch /usr/local/bin/craxfetch
sudo chmod +x /usr/local/bin/craxfetch

if [ $? -ne 0 ]; then
    echo "Error installing craxfetch. Please ensure you have write permissions to /usr/local/bin."
    exit 1
fi

# Print confirmation message
printf "${Purple} ____          _
/ ___| _  _ ___ ___ ___  ___ ___| |
\\___ \\| | | |/ __/ __/ _ \\/ __/ __| |
 ___) | |_| | (_| (_|  __/\\__ \\__ \\_|
|____/ \\__,_|\\___\\___\\___||___/___(_)
${Color_Off}\n"
printf "Craxfetch has been successfully installed to /usr/local/bin.\n"
printf "You can now run it by typing: ${Green}craxfetch${Color_Off}\n"

exit 0
