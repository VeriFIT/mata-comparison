#!/bin/bash
# installs requirements (need to run with sudo! )

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
make release
sudo make install
popd

echo "=================== Installing awali ====================="
pushd tools/awali
./install-awali.sh
popd

echo "============== Installing Python packages ================"
sudo -u ae pip3 install pip-packages/* || echo "Error installing Python messages"
