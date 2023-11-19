#!/usr/bin/env bash
# Docker Setup Instructions for Debian and related distros
# https://docs.docker.com/engine/install/debian/

# Step 1
for pkg in docker.io docker-doc docker-compose podman-docker containerd runc; do sudo apt-get remove $pkg; done

# Step 2
# cat /etc/os-release to find and set VERSION_CODENAME
VERSION_CODENAME=bookworm
echo ${VERSION_CODENAME}
# test if the VERSION_CODENAME is correct
grep -i "${VERSION_CODENAME}" /etc/os-release;
if [ $? != 0 ]; then
  echo 'VERSION_CODENAME incorrect!';
  exit 1;
else
  echo 'VERSION_CODENAME set correctly!';
fi;

# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl gnupg
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg

# Add the repository to Apt sources:
echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update


# Step 3
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# Step 4
sudo docker run hello-world
