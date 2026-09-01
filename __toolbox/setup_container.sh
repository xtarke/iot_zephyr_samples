#!/bin/sh

if [ -n "$1" ]; then
   echo "Setting name of toolbox to $1"
   NAME=$1
else
   NAME=dev
   echo "Default name of toolbox is $NAME"
fi

# Check if container and image exists
podman container list -a | grep $NAME > /dev/null
if [ $? -eq 0 ]; then
   echo "Container named $NAME already exists!"
   while true; do
	read -p "Confirm removal of container ? (y/n): " answer
	case $answer in
		[Yy]* ) break;;     # Continue execution
        	[Nn]* ) exit 1;;    # Stop script
        	* ) echo "Please answer (y)es or (n)o.";;
    	esac
   done
   podman stop $NAME
   podman rm $NAME
fi

podman images | grep $NAME > /dev/null
if [ $? -eq 0 ]; then
   echo "Image localhost/$NAME-toolbox:latest already exists!"
   while true; do
        read -p "Confirm removal of image ? (y/n): " answer
        case $answer in
                [Yy]* ) break;;     # Continue execution
                [Nn]* ) exit 1;;    # Stop script
                * ) echo "Please answer (y)es or (n)o.";;
        esac
   done
   podman rmi localhost/$NAME-toolbox:latest
fi

while true; do
    read -p "Confirm creation of $NAME toolbox ? (y/n): " answer
    case $answer in
        [Yy]* ) break;;     # Continue execution
        [Nn]* ) exit 1;;    # Stop script
        * ) echo "Please answer yes or no.";;
    esac
done

echo "Starting ..."

podman build --squash --network=host --tag $NAME-toolbox:latest Container/
toolbox create -i $NAME-toolbox:latest $NAME
toolbox run --container=$NAME setup_extensions.sh
