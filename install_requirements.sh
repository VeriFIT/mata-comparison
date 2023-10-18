#!/bin/bash
# installs requirements (need to run with sudo! )

# Newer libc
# echo "================== Installing libc ===================="
# DEBIAN_FRONTEND=noninteractive dpkg --auto-deconfigure --force-all -i packages/libc/*.deb || echo "Error installing libc"

# Newer Python
# echo "================== Installing Python ===================="
# dpkg -i packages/python3/*.deb || echo "Error installing Python (don't panic, this is expected ; pip3 should still work)"

echo "============== Installing Python packages ================"
sudo -u ae pip3 install pip-packages/* || echo "Error installing Python messages"
