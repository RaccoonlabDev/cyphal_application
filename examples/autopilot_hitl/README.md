# Ardupilot HITL

## 1. Prerequisites

Hardware:

- ArduPilot compatible autopilot with at least 2 MB flash memory and CAN bus
- CAN-sniffer

Software:

- QGC or any other ground control station software,
- Cyphal related utilies such as Yukon, Yakut, nunavut, etc

## 2. Usage

1. Install Gazebo Garden and the ArduPilot Gazebo plugin according to [the official ArduPilot instructions](https://ardupilot.org/dev/docs/sitl-with-gazebo.html#sitl-with-gazebo) and run Gazebo Simulator

```bash
gz sim -v4 -r -s --headless-rendering iris_runway.sdf
```

> It is recommended to use the headless mode because the simulator is very real-time sensitive.

2. Connect CAN-sniffer to PC and create SLCAN

An example of how you can create SLCAN:

```bash
source scripts/init.sh
```

3. Build and Run Cyphal HITL application

```bash
# Clone with submodules
git clone git@github.com:RaccoonlabDev/libcanard_cyphal_application.git --recursive
cd libcanard_cyphal_application

# Generate DSDL
./scripts/nnvg_generate_c_headers.sh

# Build
mkdir -p build/hitl
cd build/hitl
cmake ../../examples/autopilot_hitl && make

# Run
./application
```

4. Connect an autopilot with PC and sniffer

> It is expected that your firmware is based on custom branch [cyphal-hitl](https://github.com/PonomarevDA/ardupilot/tree/cyphal-hitl)

```bash
cd Ardupilot
git checkout cyphal-hitl
./waf configure --board CUAVv5
./waf --targets bin/arducopter --upload
```

In result your connection scheme might be as follows:

![](https://github.com/RaccoonlabDev/innopolis_vtol_dynamics/blob/master/docs/img/sniffer_connection.png?raw=true)

> At the time of writing, only CAN1 is supported.

5. Configure autopilot and reboot

```bash
cd scripts
./ardupilot_gazebo_cyphal_quadcopter.py
```

6. Wait until you get a message `EKF3 IMU0 is using GPS` and then fly

## 3. How does it work?

## 3.1. Actuator command flow

```mermaid
flowchart LR

hw(autopilot)
hitl_node(HITL Cyphal node)
ap_gz_plugin(<a href='https://github.com/ArduPilot/ardupilot_gazebo'>Ardupilot Gazebo plugin</a>)
gz(Gazebo Gardens)

setpoint[<a href='https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/actuator/common/sp/Vector4.0.1.dsdl'>setpoint</a>, 2000, 200 Hz]
readiness[ <a href='https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/common/Readiness.0.1.dsdl'>readiness</a>, 2001, 20 Hz]

hw --> setpoint --> hitl_node
hw --> readiness --> hitl_node
hitl_node --> ap_gz_plugin --> gz

```

### 3.1. Sensors data flow

```mermaid
flowchart LR

hw(autopilot)
hitl_node(HITL Cyphal node)
ap_gz_plugin(<a href='https://github.com/ArduPilot/ardupilot_gazebo'>Ardupilot Gazebo plugin</a>)
gz(Gazebo Gardens)


hitl_node --> gps_point[ gps.point, 2010, 10 Hz] --> hw
hitl_node --> gps_sats[ gps.sats, 2011, 10 Hz] --> hw
hitl_node --> gps_status[ gps.satatus, 2012, 10 Hz] --> hw
hitl_node --> gps_pdop[ gps.pdop, 2013, 10 Hz] --> hw
hitl_node --> baro_pressure[ baro.pressure, 2021, 10 Hz] --> hw
hitl_node --> baro_temperature[ baro.temperature, 2022, 10 Hz] --> hw
hitl_node --> mag[ mag, 2030, 50 Hz] --> hw
hitl_node --> imu_accel[ imu.accel, 2040, 200 Hz] --> hw
hitl_node --> imu_gyro[ imu.gyro, 2041, 200 Hz] --> hw
gz --> ap_gz_plugin --> hitl_node
```

### 3.3. Yakut

![](../../assets/hitl/y_mon.png)
