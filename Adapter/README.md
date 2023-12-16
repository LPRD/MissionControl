## MissionControlAdapter<a name="ADAPTER"></a> ##

Read the top level README file for more information about this service

### Setup Instructions ###

#### Linux Setup Instructions ####

- clone and follow the installation instructions for [CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial)
    - make sure your user is added to the dialout group (CppLinuxSerial FAQ) 
    - Read the [mbedded.ninja linux serial blog post](https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/) for detailed usage of this library 
- build an example or main executable
- run the executable

```bash
# may require a system restart for dialout 
# to show in 'groups' command and for your 
# user to actually be part of the group
sudo usermod –a –G dialout username
```

Future container
```bash
docker pull < TBD Container >
```

TODO:
- Create a MissionControlAdapter container for Linux Systems
    - use the ["--device" docker run flag](https://docs.docker.com/engine/reference/commandline/run/#device) to allow a container running on linux to access a specific usb device  
- Create a MissionControlAdapter container for Windows systems
    - see the [WSL section of CppLinuxSerial](https://github.com/gbmhunter/CppLinuxSerial#wsl) for USB passthrough information
















