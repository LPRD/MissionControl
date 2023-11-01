# MissionControl<a name="TOP"></a> #

Repo that handles all applications/services that are needed on a mission control laptop

<details>
    <summary>Section Links</summary>
    <p>[Server](#SERVER) | [Telemetry](#TELEMETRY) | [Database](#DATABASE) | [Interface](#INTERFACE) | [Adapter](#ADAPTER)</p>
</details>

### Where each service runs ###
First Header  | Second Header
| :---: | :---:
------------- | -------------
MissionControlServer  | Docker Container
MissionControlTelemetry Cell  | Docker Container
MissionControlDatabase  | Docker Container
MissionControlInterface  | Locally
MissionControlAdapter  | Locally

TODO:
- [ ] add useful links list

### Docker Setup Instructions  ###


#### Windows  ####
Running MissionControl Containers in Windows 10/11




#### Linux  ####
Running MissionControl Containers in a Linux distribution




#### Docker Development Setup Instructions ####
Modifying MissionControl Containers/making your own version

Requirements
- VSCode with the following extentions
    - Remote Development
    - Dev Containers
    - Remote Explorer (might only be relevant for SSH)




### Github Desktop and git Setup Instructions  ###







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



#### Usage Instructions ####





## MissionControlTelemetry<a name="TELEMETRY"></a> ##


TODO:
- [x] Grafana Dashboard (in-progress)
- [ ] Three.js Apps (various 3D visualizations)






## MissionControlDatabase<a name="DATABASE"></a> ##








## MissionControlInterface<a name="INTERFACE"></a> ##








## MissionControlAdapter<a name="ADAPTER"></a> ##








[Return to Top of Page](#TOP)
[Useful README markdown guide for making these pages](https://github.com/tchapi/markdown-cheatsheet/blob/master/README.md)
