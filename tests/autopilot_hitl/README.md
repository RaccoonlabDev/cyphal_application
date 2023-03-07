# Ardupilot HITL

## 1. Usage

1. Run gazebo simulator

```bash
gz sim -v4 -r -s --headless-rendering iris_runway.sdf
```

2. Connect CAN-sniffer to PC and create SLCAN

3. Build and Run Cyphal HITL application

```bash
# Build once
git clone git@github.com:RaccoonlabDev/libcanard_cyphal_application.git --recursive
cd libcanard_cyphal_application
mkdir -p build/hitl
cd build/hitl
cmake ../../tests/autopilot_hitl && make

# Run
./application
```

4. Connect an autopilot with PC and sniffer

5. Fly when it is ready to fly

## 2. How does it work?

## 2.1. Actuator command flow

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

### 2.1. Sensors data flow

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

### 2.3. Yakut

![](../../assets/hitl/y_mon.png)

