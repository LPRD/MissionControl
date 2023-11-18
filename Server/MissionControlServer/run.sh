#!/usr/bin/env bash
# start MissionControlServer
cd /home/MissionControl/Server/MissionControlServer; 

# DATABASE_IP="10.80.22.9"    # individual docker ip
DATABASE_IP="10.80.23.9"    # combined docker ip

SERVER_PORT="13056"

# Run Main application - TO BE CREATED
# cd ./output;
# ./missioncontrolserver ${DATABASE_IP} ${SERVER_PORT};

# Run example
# cd ./output;
# ./tcpScaleServer ${DATABASE_IP} ${SERVER_PORT};
cd ./examples/tcpScaleServer;
make run ARGS="${DATABASE_IP} ${SERVER_PORT}";

# afterwards
bash;
