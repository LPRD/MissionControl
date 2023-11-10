## MissionControlTelemetry<a name="TELEMETRY"></a> ##

Read the top level README file for more information about this service

### Setup Instructions ###

- (Future Option) Pull and run a fully configured docker container
    - this container doesn't need too much configuration, so this future option is not a high priority

```bash
docker pull < TBD Container >
```


- Create and configure a new container
    - use the docker-compose file to create a configured container
    - add datasources from the grafana UI (see instructions in docker-compose file)
    - add additional auto refresh intervals by: 
        - going to a specific dashboard
            - going to "Dashboard settings"
                - adding "100ms,500ms,1s," to the beginning of the auto refresh list
                - selecting "Save Dashboard"
    - create dashboards and visualizations as desired
        - database query commands are useful for getting the information you want to show


Example postgresql queries
- - - -

Simulated Sensor Data vs. Run time
(use trend plot)

```bash
WITH 
    t AS (
        SELECT * FROM load_cell_data ORDER BY run_time DESC LIMIT 300
        )
SELECT 
    run_time,
    LC1_FORCE as lc1,
    LC2_FORCE as lc2,
    LC3_FORCE as lc3,
    LC4_FORCE as lc4 
FROM 
    t 
ORDER BY 
    run_time ASC; 
```


Simulated Sensor Data vs. Time
(use Time Series plot)


```bash
WITH 
    t AS (
        SELECT * FROM load_cell_data ORDER BY run_time DESC LIMIT 10000
        ),
    f AS (
        SELECT LC1_FORCE,LC2_FORCE,LC3_FORCE,LC4_FORCE FROM t
        )
-- SELECT * from t
-- SELECT * from f
SELECT 
    TIME_STR as timestamp,
    -- AVG (LC1_FORCE):: NUMERIC(10,2)
    AVG (LC1_FORCE):: NUMERIC(10,2) as lc1,
    AVG (LC2_FORCE):: NUMERIC(10,2) as lc2,
    AVG (LC3_FORCE):: NUMERIC(10,2) as lc3,
    AVG (LC4_FORCE):: NUMERIC(10,2) as lc4
FROM 
    t 
GROUP BY 
    TIME_STR
ORDER BY 
    TIME_STR ASC
```

Also need to use a 
"Convert field type"
from text to time

With time in the following format:
YYYY-MM-DD HH:mm:ss

- - - -



Simulated Sensor Data vs. Run time
where the sum of all 4 sensors is graphed
(use trend plot)

```bash
WITH 
    t AS (
        SELECT * FROM load_cell_data ORDER BY run_time DESC LIMIT 300
        )
SELECT 
    run_time,
    SUM (LC1_FORCE + LC2_FORCE + LC3_FORCE + LC4_FORCE) as lc_sum
FROM 
    t
GROUP BY 
    run_time 
ORDER BY 
    run_time ASC;
```

- - - -





TODO:
- [ ] find out if there is a way for Grafana to interpret/plot timestamps with a decimal number of seconds (ISO 8601 Timestamps have seconds as ss.ssss)

