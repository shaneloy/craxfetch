#!/bin/bash

# Reset color
Color_Off='\033[0m'

# Regular Colors
Purple='\033[0;35m'
Green='\033[0;32m'

REPO_URL="https://github.com/shaneloy/craxfetch.git"
REPO_DIR="craxfetch_repo"
CRAXFETCH_SCRIPT="$REPO_DIR/craxfetch"
INSTALL_PATH="/usr/local/bin/craxfetch"

# Check for git
if ! command -v git &> /dev/null; then
    echo "Error: git is required to clone the repository. Please install it."
    exit 1
fi

# Clone the repository
echo "Cloning the craxfetch repository..."
git clone --depth 1 "$REPO_URL" "$REPO_DIR"

if [ $? -ne 0 ]; then
    echo "Error cloning the repository."
    exit 1
fi

# Check if the craxfetch script exists
if [ ! -f "$CRAXFETCH_SCRIPT" ]; then
    echo "Error: craxfetch script not found in the cloned repository."
    exit 1
fi

# Make it executable
echo "Making craxfetch executable..."
sudo chmod +x "$CRAXFETCH_SCRIPT"

if [ $? -ne 0 ]; then
    echo "Error making craxfetch executable. You might need sudo privileges."
    exit 1
fi

# Install craxfetch
echo "Installing craxfetch to $INSTALL_PATH..."
sudo mv "$CRAXFETCH_SCRIPT" "$INSTALL_PATH"

if [ $? -ne 0 ]; then
    echo "Error installing craxfetch. You might need sudo privileges."
    exit 1
fi

# Clean up the cloned repository
echo "Cleaning up..."
rm -rf "$REPO_DIR"

# Print confirmation message
printf "${Purple} 
███████ ██    ██  ██████  ██████ ███████ ███████ ███████ 
██      ██    ██ ██      ██      ██      ██      ██      
███████ ██    ██ ██      ██      █████   ███████ ███████ 
     ██ ██    ██ ██      ██      ██           ██      ██ 
███████  ██████   ██████  ██████ ███████ ███████ ███████
${Color_Off}\n"
printf "Yay! Craxfetch has been successfully installed to ${Green}$INSTALL_PATH${Color_Off}.\n"
printf "You can now run it by typing: ${Green}craxfetch${Color_Off}\n"
printf "For a full list of commands: ${green}craxfetch -h${Color_off}\n"

exit 0
