## MissionControlServer<a name="SERVER"></a> ##

Read the top level README file for more information about this service

### Setup Instructions ###

- (Future Option) Pull and run a fully configured docker container

```bash
docker pull < TBD Container >
```


- Create and configure a new container
    - use the docker-compose file to create a configured container
    - install needed packages (if not already done through the docker-compose)
    - clone the MissionControl repo
    - move the .vscode and MissionControlServer folders to the /home directory
    - move the .profile file to the /home directory if the system time does not match the correct time zone
        - use the "date" command to see the system time



TODO:
- [ ] Finish Examples
- [ ] Create Main Application
- [ ] Add method to automatically start up main app upon starting container
- [ ] Create fully configured docker container
