# MissionControl<a name="TOP"></a> #

Repo that handles all applications/services that are needed on a mission control laptop

### Section Links ###

[Docker Setup](#DOCKER_SETUP "Goto DOCKER_SETUP")

[Git/Github Setup](#GIT_SETUP "Goto GIT_SETUP")

[Server](#SERVER "Goto SERVER")

[Telemetry](#TELEMETRY "Goto TELEMETRY")

[Database](#DATABASE "Goto DATABASE")

[Interface](#INTERFACE "Goto INTERFACE")

[Adapter](#ADAPTER "Goto ADAPTER")


### Where each service runs ###
First Header  | Second Header
| :---: | :---:
MissionControlServer  | Docker Container
MissionControlTelemetry | Docker Container
MissionControlDatabase  | Docker Container
MissionControlInterface  | Locally
MissionControlAdapter  | Locally

TODO:
- [ ] add useful links list
- [ ] add pictures of each GUI
- [ ] add draw.io diagram outlining how everything interracts



### Docker Setup Instructions<a name="DOCKER_SETUP"></a>  ###

#### Windows  ####
Running MissionControl Containers in Windows 10/11

Download [Docker Desktop](https://www.docker.com/products/docker-desktop/) for Windows


#### Linux  ####
Running MissionControl Containers in a Linux distribution

Download [Docker Desktop](https://www.docker.com/products/docker-desktop/) for Linux

Alternatively, you can install and use the command line version of docker (for more advanced users)

```bash
#This does not work on a container running on Windows due to WSL
#In that scenario, just install Docker Desktop 
$ sudo apt-get update
$ sudo apt-get install curl
$ curl -fsSL https://get.docker.com/ | sh

#to run docker commands as a non-root user
$ sudo usermod -aG docker < your_username >

#to verify everything is set up correctly
$ docker run hello-world
```


### Docker Usage Instructions ###
use the command line to pull the desired Docker image 
```bash
# public iamge
$ docker pull debian:trixie

# example team image that does not exist
$ docker pull lprd/MissionControlServer:latest
```
[docker pull documentation](https://docs.docker.com/engine/reference/commandline/pull/)

OR

type the desired Docker container name in the Docker Desktop top search bar and select pull.



- - - -



Once an image is pulled, it can be ran using the command line
```bash
# run a customized container based on a public image
$ docker run -it  --network host --hostname MCS -v /docker --name MissionControlServer1.2 debian:trixie bash

# run an example team container that is already configured (does not exist)
# use -d instead of -it to have the container run in the background
$ docker run -it lprd/MissionControlServer:latest
```
[docker run documentation](https://docs.docker.com/engine/reference/commandline/run/)

OR

click the start button in the Docker Desktop app (first navigate to the containers tab)


- - - -


#### Docker Development Setup Instructions ####
Modifying MissionControl Containers/making your own version

Requirements
- VSCode with the following extentions
    - Remote Development
    - Dev Containers
    - Remote Explorer (might only be relevant for SSH)

1. Press the <kbd>F1</kbd> key or click on the search bar at the top when in VSCode 
    1. search for ">Dev Containers: Attach to Running Container"
    2. select that option and select the desired container to connect to
2. You now have a VSCode session in the container!

[This VSCode documentation page](https://code.visualstudio.com/docs/devcontainers/containers) has more information


### Github Desktop and git Setup Instructions<a name="GIT_SETUP"></a> ###

#### Windows  ####
A manager app for Windows 10/11 that makes repository development easier

Download [Github Desktop](https://desktop.github.com/) for for Windows


#### Linux  ####
You can install and use git on the command line if you are using a Linux image (no Github Desktop available)
git allows you to do version controlled development for this repository or others!

```bash
# If you’re on Fedora (or any closely-related RPM-based distribution, 
# such as RHEL or CentOS), you can use dnf:
$ sudo dnf install git-all

# If you’re on a Debian-based distribution, such as Ubuntu, try apt:
$ sudo apt install git-all
```

Read the [git documentation](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) for more information


## MissionControlServer<a name="SERVER"></a> ##
MissionControlServer is a Docker Container that runs a C++ Application responsible for:
- collecting data from mission hardware
    - Ground Support Equipment (GSE)
    - Flight Computer
        - may interract with an adapter service that runs locally on the host machine
    - any other hardware that can benefit from sending its data to a centralized database
- sending commands to mission hardware
- interracting with the database on the mission control laptop
    - mostly adding information

The MissionControlServer directory contains:
- [x] C++ Application Source Code (in-progress)
- [x] a docker-compose file for setting up the correct container to run the server from
- [ ] a publicly available container that has the server installed and set to
  run when the container is started

[Docker Compose Documentation](https://docs.docker.com/engine/reference/commandline/compose/) for understanding docker-compose.yml files

Note: the server application requires the latest version of libpqxx (7.8.1).
Only certain Linux images support recent versions of libpqxx.

[This Repology page](https://repology.org/project/libpqxx/versions) shows many
linux distributions and the most recent libpqxx library version available. 

The image currently being used is 
[Debian 13 (trixie)](https://hub.docker.com/_/debian)



## MissionControlTelemetry<a name="TELEMETRY"></a> ##
MissionControlTelemetry is a Docker Container that runs a customizable Dashboard app, called Grafana, for viewing data.
[Grafana Overview and Site](https://grafana.com/grafana/)

Our use case for Grafana is viewing:
- live data collected from mission hardware
    - Ground Support Equipment (GSE)
    - Flight Computer
    - any other hardware that can benefit from data visualization
- previously collected data
- export data

The MissionControlTelemetry directory contains:
- [x] a docker-compose file for setting up a properly configured Grafana container
- [ ] a publicly available Grafana container that has been customized for our use case


The second checkbox is low priority/unnecessary at the moment becuase
- Grafana already runs upon container start up, eliminating the need for a customized container
- the Grafana container can be configured enough for our use case with the docker-compose file

The image currently being used is 
[Grafana](https://hub.docker.com/r/grafana/grafana)

TODO:
- [x] Grafana Dashboard (in-progress)
- [ ] (at some point in the future) Three.js Apps (various 3D visualizations)



## MissionControlDatabase<a name="DATABASE"></a> ##
MissionControlDatabase is a Docker Container that runs a PostgreSQL database for storing and providing mission data.
[PostgreSQL Overview and Site](https://www.postgresql.org/)

Our use case for a PostgreSQL database is:
- providing an easy to connect data source for Grafana to use
- providing an easy to connect data path for web-based (such as javascript) applications
    - javascript applications *cannot* easily open files continuously to show the latest data that was added to it
        - ex: having MissionControlServer add the latest data to a data.json file *won't* work
    - javascript applications *cannot* easily connect with TCP Socket or other ethernet servers to share data
        - ex: having MissionControlServer send or recieve data to/from a javascript GUI app over a TCP Socket *won't* work
    - web applications can easily access databases

The MissionControlDatabase directory contains:
- [x] a docker-compose file for setting up a properly configured Postgres container
- [ ] a publicly available Postgres container that has been customized for our use case


The second checkbox is low priority/unnecessary at the moment becuase
- Postgres already runs upon container start up, eliminating the need for a customized container
- the Postgres container can be configured enough for our use case with the docker-compose file

The image currently being used is 
[Postgres](https://hub.docker.com/_/postgres)



## MissionControlInterface<a name="INTERFACE"></a> ##
MissionControlInterface is a local javascript app that runs a basic GUI for sending commands.

Our use case for MissionControlInterface is to:
- provide a minimalistic, easy to use, dedicated app for sending commands to the mission hardware

This application is *separate* from the telemetry visualization app (Grafana) to:
- increase the reliability of this application as it is more safety-critical than a telemetry dashboard
- allow for the Grafana dashboard to be used for telemetry visualization in multiple locations (this would be unwise if the dashboard easily allowed for commands to be sent)

The MissionControlInterface directory contains:
- [ ] a javascript app that can populate a special command table in the database container

MissionControlServer will have the ability to periodically read the latest commands added and forward them to the appropriate mission hardware.



## MissionControlAdapter<a name="ADAPTER"></a> ##
MissionControlAdapter is a local C++ app that allows for communication between
Mission Control Laptop (COM/Serial/USB/UART) devices and MissionControlServer.

MissionControlServer cannot easily access serial devices connected to the host
machine, such as a serial radio, since MissionControlServer runs in a docker
container.

MissionControlAdapter can solve this problem by locally running on the host
machine and forwarding serial data (and any other data if applicable) to
MissionControlServer via a TCP Socket. MissionControlAdapter will also be
responsible for forwarding commands from MissionControlServer to a serial
device. The main type of hardware connected over serial is a serial radio. A
serial radio, such as an XBee or RFD900, allows for fast, wireless communication
with mission hardware that is far away. 

Our use case for MissionControlAdapter is to:
- provide a way for MissionControlServer to communicate with hardware connected only via serial

The MissionControlAdapter directory contains:
- [ ] a C++ app for allowing MissionControlServer to communicate with serial hardware



[Return to Top of Page](#TOP)

[Useful README markdown guide for making these pages](https://github.com/tchapi/markdown-cheatsheet/blob/master/README.md)
