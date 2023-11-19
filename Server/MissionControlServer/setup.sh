#!/usr/bin/env bash
# setup the MissionControlServer Environment

# install packages
apt-get install g++ -y;
apt-get install build-essential -y; #make
apt-get install libpqxx-dev -y;
echo 'Packages installed!';
sleep 1;

# optional but helpful packages
# apt update -y && apt upgrade -y;
# apt-get install dnsutils -y;              #host <ip>
# apt-get install nbtscan -y;               #nbtscan <ip>
# apt-get install -y procps;                #for "ps aux" command to "kill -9" a PID
# apt install cmake -y;                     #cmake for build
# apt install iproute2 -y;                  #ip -c a
# apt-get install iputils-ping -y;          #ping
# apt-get -y install vim;
# apt-get -y install tree;
# apt-get install wget -y;
# apt-get install netcat-traditional -y;
# apt-get install nmap -y;
# apt-get install man -y;


# set the correct time
rm /etc/localtime;
ln -s /usr/share/zoneinfo/America/Chicago /etc/localtime;

# build
cd /home/MissionControl/Server/MissionControlServer;
mkdir -p ./build
mkdir -p ./output

# build examples
./buildExamples.sh;

# build main application - TO BE CREATED
# ./build.sh;

echo 'MissionControl environment set up!';
