#!/bin/sh

code --install-extension cantonios.project-templates
code --install-extension ms-vscode.cmake-tools
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cpptools-extension-pack
# code --install-extension fleexo.cpp-class-creator
# code --install-extension amiralizadeh9480.cpp-helper
# code --install-extension platformio.platformio-ide
# code --install-extension dnut.rewrap-revived
# code --install-extension mhutchie.git-graph
# code --install-extension jebbs.plantuml
# code --install-extension wavetrace.wavetrace
# code --install-extension raspberry-pi.raspberry-pi-pico
code --install-extension ms-vscode.vscode-serial-monitor

tar -C /usr/share/code/data/user-data/User/ -zxvf /install/code-templates.tgz

