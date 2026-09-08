# Zephyr Development Environment Setup Guide

This guide explains how to set up a Zephyr RTOS development environment using Podman toolbox containers. The setup process is divided into two phases: creating the toolbox container on your host machine, and subsequently installing the Zephyr SDK and requirements inside that container.

## Step 1: Setting up the Toolbox Container

The `setup_container.sh` script automates the creation of a Podman toolbox container based on Fedora 41.

### Usage
```bash
chmod +x setup_container.txt
./setup_container.txt [container_name]
```

* **Container Name:** By default, the script creates a toolbox container named `zephyr`. You can specify a custom name by passing it as the first argument.
* **Cleanup Prompts:** The script checks if a container or image with the specified name already exists [cite: 2]. If found, it will prompt you for confirmation to stop and remove them before proceeding.
* **Creation:** It requires confirmation to start, after which it builds a new Podman image from a local `Container/` directory using `--squash` and `--network=host`, creates the toolbox, and runs a `setup_extensions.sh` script inside the container.

## Step 2: Entering the Toolbox Container

Once the container setup is complete, you must enter the toolbox environment to proceed with the Zephyr installation:

```bash
toolbox enter zephyr
```
*(Replace `zephyr` with your custom name if you provided one in Step 1).*

## Step 3: Setting up Zephyr RTOS

Inside the toolbox container, use the `setup_zephyr.txt` script to download and configure the Zephyr SDK, West workspace, and required Python packages.

### Usage
```bash
chmod +x setup_zephyr.txt
./setup_zephyr.txt [install_path]
```

* **Installation Path:** By default, the script installs Zephyr in `/data/zephyr`. You can provide a custom installation path by passing it as the first argument.
* **What it does:** 
  * Asks for confirmation before creating the directory.
  * Creates a Python 3 virtual environment (`.venv`) in the target directory and activates it.
  * Installs the `west` build tool via `pip`.
  * Downloads, extracts, and sets up the Zephyr SDK version 1.0.1 (`zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz`).
  * Initializes a West workspace specifically tracking the Zephyr `v4.4.0` release (`--mr v4.4.0`), updates it, and exports it.
  * Installs additional Python requirements mandated by the Zephyr stack (`zephyr/scripts/requirements.txt`).
* **Environment Persistence:** The script adds a new profile script at `/etc/profile.d/activate_venv.sh`. This ensures that the Python virtual environment, the `zephyr-env.sh` script, and the `CMAKE_PREFIX_PATH` are automatically sourced during future logins to the container.