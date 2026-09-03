#!/bin/sh

if [ -n "$1" ]; then
   echo "Setting default path to $1"
   NAME=$1
else
   NAME=/data/zephyr
   echo "Default path of zephyr is $NAME"
fi


while true; do
    read -p "Confirm instalation of zephyr in $NAME ? (y/n): " answer
    case $answer in
        [Yy]* ) break;;     # Continue execution
        [Nn]* ) exit 1;;    # Stop script
        * ) echo "Please answer yes or no.";;
    esac
done

echo "Starting ..."


# if [[ -d "$NAME" ]]; then
#     echo "$NAME exists. Aborting."
# else
#     echo "Creating $NAME."
#     mkdir $NAME
# fi


cd $NAME
# python3 -m venv .venv
source .venv/bin/activate
# # ENV PATH="/workdir/.venv/bin:$PATH"
# pip install west

# # Download and install Zephyr SDK 1.0 (Required for Zephyr 4.4.0)
# wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz \
#     && tar xf zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz --checkpoint=.100 \
#     && rm zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz \
#     && cd zephyr-sdk-1.0.1 \
#     && ./setup.sh -t all -h -c
#     #&& ./setup.sh -t arm-zephyr-eabi -h -c

cd $NAME

# # Initialize the West workspace specifically tracking the v4.4.0 release
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.4.0 . \
     && west update \
     && west zephyr-export

# Install additional Python requirements mandated by the Zephyr 4.4.0 stack
pip install -r zephyr/scripts/requirements.txt

# Set environment variables so CMake automatically locates the SDK
export ZEPHYR_SDK_INSTALL_DIR="$NAME/zephyr-sdk-1.0.1"

# Add source lines to activate env when entering toolbox
## sudo echo "source /zephyr/zephyr/zephyr-env.sh" >> /etc/bashrc
## sudo echo "source /zephyr/zephyr/zephyr-env.sh" >> /etc/zshrc