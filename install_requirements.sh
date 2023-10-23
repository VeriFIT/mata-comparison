#!/bin/bash

VM_USER=tacas23

# installs requirements (need to run with sudo! )
if [ "$EUID" -ne 0 ]; then
  echo -e "\e[31merror: ./install_requirements.sh must be run as root\e[0m"
  exit
fi

# Installing mata / vata / awali prerequisities
echo "============= Installing Cpp Prerequisities =============="
dpkg -i packages/cpp/*.deb || echo "Error installing C++ prerequisities"

# Installing automata.net prerequisities
echo "============ Installing .NET Prerequisities =============="
dpkg -i packages/dotnet/*.deb || echo "Error installing .NET prerequisities"

# Installing automatalib / brics prerequisities
echo "============ Installing Java Prerequisities =============="
dpkg -i packages/java/*.deb || echo "Error installing java prerequisities"

echo "================== Installing libmata ===================="
pushd tools/mata
echo "[!] Note: we are building libmata library only."
echo "[!] To build libmata complete with tests and examples, enable internet and run:"
echo "  $ make -C tools/mata release"
make release-lib
sudo make install
popd

echo "=================== Installing awali ====================="
pushd tools/awali
./install-awali.sh
popd

echo "============== Installing Python packages ================"
sudo -u $VM_USER pip3 install pip-packages/deps/*.whl || echo "Error installing Python dependencies"
sudo -u $VM_USER pip3 install --no-deps pip-packages/libmata/libmata-1.0.0-*.whl || echo "Error installing libmata"
sudo -u $VM_USER pip3 install --no-deps pip-packages/pyinterpret/pyinterpret-1.0.0-*.whl || echo "Error installing pyinterpret"

echo "==================== Setting Path ========================"
echo "export PATH=\$PATH:/home/$VM_USER/.local/bin" >> ~/.bashrc
echo "echo \"export PATH=\$PATH:/home/\$VM_USER/.local/bin\" >> ~/.bashrc"
echo "Please, run:'export PATH=\$PATH:/home/$VM_USER/.local/bin' to set \$PATH."
