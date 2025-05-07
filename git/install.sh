#!/bin/bash

# Reset color
Color_Off='\033[0m'

# Regular Colors
Purple='\033[0;35m'
Green='\033[0;32m'
Red='\033[0;31m'
Yellow='\033[0;33m'


# Check if the script is being run as root
if [ "$(id -u)" -eq 0 ]; then
    echo "${Yellow}Running as root. Proceeding without sudo.${Color_Off}"
    USE_ROOT=true
else
    # Attempt to validate sudo privileges early
    if ! sudo -v; then
        printf "${Red}Error: This script requires sudo privileges to install to /usr/local/bin/.\n"
        printf "Please run the script with sudo: ${Green}sudo ./install.sh${Color_Off}\n"
        exit 1
    fi
fi
BINARY_NAME="craxfetch"
REPO_OWNER="shaneloy"
REPO_NAME="craxfetch"
RELEASE_DOWNLOAD_URL="https://github.com/$REPO_OWNER/$REPO_NAME/releases/download/$BINARY_NAME/$BINARY_NAME"
INSTALL_PATH="/usr/local/bin/$BINARY_NAME"
REPO_URL="https://github.com/$REPO_OWNER/$REPO_NAME.git"
REPO_DIR="${REPO_NAME}_repo"
SOURCE_INSTALL_SCRIPT="$REPO_DIR/$BINARY_NAME" # Adjust if your script has a different name or location

# Check for wget
if ! command -v wget &> /dev/null; then
    printf "${Yellow}Warning: wget not found. Attempting installation from Git repository.${Color_Off}\n"
    USE_GIT=true
fi

# Function to attempt downloading from release
install_from_release() {
    echo "Downloading $BINARY_NAME from the latest release..."
    wget -q "$RELEASE_DOWNLOAD_URL" -O "$INSTALL_PATH"
    if [ $? -eq 0 ]; then
        echo "Successfully downloaded $BINARY_NAME."
        chmod +x "$INSTALL_PATH"
        if [ $? -ne 0 ]; then
            printf "${Red}Error making $INSTALL_PATH executable.${Color_Off}\n"
            return 1
        fi
        return 0 # Success
    else
        echo "${Yellow}Download from release failed. Attempting installation from Git repository.${Color_Off}"
        return 1 # Failure
    fi
}

# Function to install from Git repository
install_from_git() {
    # Check for git
    if ! command -v git &> /dev/null; then
        printf "${Red}Error: git is required to clone the repository. Please install it.${Color_Off}\n"
        return 1
    fi

    echo "Cloning the $REPO_NAME repository..."
    git clone --depth 1 "$REPO_URL" "$REPO_DIR"
    if [ $? -ne 0 ]; then
        printf "${Red}Error cloning the repository.${Color_Off}\n"
        return 1
    fi

    # Check if the source install script exists (adjust path if needed)
    if [ ! -f "$SOURCE_INSTALL_SCRIPT" ]; then
        printf "${Red}Error: $BINARY_NAME script not found in the cloned repository at $SOURCE_INSTALL_SCRIPT.${Color_Off}\n"
        return 1
    fi

    echo "Making $BINARY_NAME executable..."
    sudo chmod +x "$SOURCE_INSTALL_SCRIPT"
    if [ $? -ne 0 ]; then
        printf "${Red}Error making $BINARY_NAME executable. You might need sudo privileges.${Color_Off}\n"
        return 1
    fi

    echo "Installing $BINARY_NAME to $INSTALL_PATH..."
    sudo mv "$SOURCE_INSTALL_SCRIPT" "$INSTALL_PATH"
    if [ $? -ne 0 ]; then
        printf "${Red}Error installing $BINARY_NAME. You might need sudo privileges.${Color_Off}\n"
        return 1
    fi

    # Clean up
    echo "Cleaning up..."
    rm -rf "$REPO_DIR"
    return 0 # Success
}

# Main installation logic
if [ -z "$USE_GIT" ]; then
    if install_from_release; then
        # Installation successful from release
        success=true
    else
        if install_from_git; then
            success=true
        else
            success=false
        fi
    fi
else
    if install_from_git; then
        success=true
    else
        success=false
    fi
fi

if "$success"; then

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

fi
exit 0
