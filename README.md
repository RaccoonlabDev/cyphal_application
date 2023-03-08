# Libcanard application example [![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=RaccoonlabDev_libcanard_stm32_cyphal&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=RaccoonlabDev_libcanard_stm32_cyphal) [![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=RaccoonlabDev_libcanard_stm32_cyphal&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=RaccoonlabDev_libcanard_stm32_cyphal)

The minimal sample application includes the following features:
- [Heartbeat](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/7509.Heartbeat.1.0.dsdl) (send abstract node status information)
- [GetInfo](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/430.GetInfo.1.0.dsdl) (return full node info)
- [Register interface](https://github.com/OpenCyphal/public_regulated_data_types/tree/master/uavcan/register) (read and write paramters)
- [port.List](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/port/7510.List.0.1.dsdl) (send all supported port id)
- [ExecuteCommand](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/435.ExecuteCommand.1.0.dsdl) (handle requests on restart or save parameters commands)

There are also examples of the following features:

- [udral/actuator](https://github.com/OpenCyphal/public_regulated_data_types/tree/master/reg/udral/service/actuator) ([setpoint](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/actuator/common/sp/Vector4.0.1.dsdl), [readiness](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/common/Readiness.0.1.dsdl) and [feedback](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/actuator/common/Feedback.0.1.dsdl)),
- barometer ([pressure](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/pressure/Scalar.1.0.dsdl) and [temperature](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/temperature/Scalar.1.0.dsdl)),
- magnetometer ([magnetic field](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/magnetic_field_strength/Vector3.1.0.dsdl)),
- gnss ([point](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/physics/kinematics/geodetic/PointStateVarTs.0.1.dsdl), [status](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl), [sats](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl), [pdop](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl)),
- imu ([accelerometer](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/acceleration/Vector3.1.0.dsdl), [gyroscope](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/angular_velocity/Vector3.1.0.dsdl)).

## 1. Prerequisites

Software dependencies:
- [libcanard](https://github.com/OpenCyphal/libcanard)
- [platform_specific_components](https://github.com/OpenCyphal-Garage/platform_specific_components)
- [o1heap](https://github.com/pavel-kirienko/o1heap)
- [public_regulated_data_types](https://github.com/OpenCyphal/public_regulated_data_types)
- [libparams](https://github.com/PonomarevDA/libparams/tree/main/libparams)

## 2. Usage:

**Step 1. Clone repo with submodules**

```bash
git clone <this_repo> --recursive
git submodule update --init --recursive
```

**Step 2. Include CMakeList.txt**

In your application's cmake file specify the target, include this CMakeLists.txt and add source code to the executable:

```cmake
# 1. select either bxcan or fdcan or socketcan
set(PLATFORM bxcan)

# 2. include CMakeLists.txt
include(<path_to_this_repo>/Cyphal/CMakeLists.txt)

# 3. add ${CYPHAL_SRC} to our source files
add_executable(<executable_name> ${CYPHAL_SRC})
```

## 3. Usage examples:

- [stm32 (Mini v2 node)](https://github.com/RaccoonlabDev/mini_v2_node)
- [ubuntu minimal application example](examples/ubuntu_minimal)
- [ubuntu publisher example](examples/ubuntu_publisher_example)
- [ubuntu Ardupilot HITL application](examples/autopilot_hitl)
