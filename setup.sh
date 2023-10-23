#!/bin/bash

VM_USER=tacas23

echo "==================== Setting Path ========================"
echo 'PATH=$PATH:/home/$VM_USER/.local/bin' >> ~/.bashrc
export "PATH=$PATH:/home/$VM_USER/.local/bin"
echo "$PATH"
