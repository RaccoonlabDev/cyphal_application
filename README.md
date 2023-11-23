# Cyphal application template [![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=RaccoonlabDev_libcanard_stm32_cyphal&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=RaccoonlabDev_libcanard_stm32_cyphal) [![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=RaccoonlabDev_libcanard_stm32_cyphal&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=RaccoonlabDev_libcanard_stm32_cyphal)

A minimal application always includes the following features:

| № | Type      | Message  | Comment | Rate |
| - | --------- | -------- | ------- | ---- |
| 1 | pub | [uavcan.node.Heartbeat](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/7509.Heartbeat.1.0.dsdl) | Abstract node status info | 1 Hz |
| 2 | pub | [uavcan.node.port.List](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/port/7510.List.0.1.dsdl) | Node network capabilities | 0.2 Hz |
| 3 | RPC-service | [uavcan.node.GetInfo](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/430.GetInfo.1.0.dsdl) | Full node info | |
| 4 | RPC-service | [uavcan.node.ExecuteCommand](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/node/435.ExecuteCommand.1.0.dsdl) | Restart, save or reset registers |
| 5 | RPC-service | [uavcan.register.List](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/register/385.List.1.0.dsdl) </br> [uavcan.register.Access](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/register/384.Access.1.0.dsdl) | Register interface |

There are also examples of the following auxilliary features:

| Interface | Port | Message |
| --------- | ---- | ------- |
| [udral/actuator](https://github.com/OpenCyphal/public_regulated_data_types/tree/master/reg/udral/service/actuator) | sub.setpoint </br> sub.readiness </br> pub.feedback | [udral.service.actuator.common.sp.Vector31](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/actuator/common/sp/Vector31.0.1.dsdl) </br> [reg.udral.service.common.Readiness](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/service/common/Readiness.0.1.dsdl) </br> [zubax.telega.CompactFeedback](https://github.com/Zubax/zubax_dsdl/blob/master/zubax/telega/CompactFeedback.1.0.dsdl) |
| Barometer | pub.pressure </br> pub.temperature | [uavcan.si.sample.pressure.Scalar](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/pressure/Scalar.1.0.dsdl) </br> [uavcan.si.sample.temperature.Scalar](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/temperature/Scalar.1.0.dsdl) |
| Magnetometer | pub.mag | [uavcan.si.sample.magnetic_field_strength.Vector3](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/magnetic_field_strength/Vector3.1.0.dsdl) |
| [udral/gnss](https://nunaweb.opencyphal.org/api/storage/docs/docs/reg/index.html#reg_drone_service_gnss) | pub.point </br> pub.status </br> pub.sats </br> pub.pdop | [reg.udral.physics.kinematics.geodetic.PointStateVarTs](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/reg/udral/physics/kinematics/geodetic/PointStateVarTs.0.1.dsdl) </br> [uavcan.primitive.scalar.Integer16](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl) </br> [uavcan.primitive.scalar.Integer16](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl) </br> [uavcan.primitive.scalar.Integer16](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/primitive/scalar/Integer16.1.0.dsdl) |
| imu | pub.accelerometer </br> pub.gyro | [uavcan.si.sample.acceleration.Vector3](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/acceleration/Vector3.1.0.dsdl) </br> [uavcan.si.sample.angular_velocity.Vector3](https://github.com/OpenCyphal/public_regulated_data_types/blob/master/uavcan/si/sample/angular_velocity/Vector3.1.0.dsdl) |

Supported platforms:

| PLATFORM | Implementation details | Note |
| -------- | ---------------------- | ---- |
| bxcan | [platform_specific_components](https://github.com/OpenCyphal-Garage/platform_specific_components/tree/master/stm32/libcanard/bxcan) | Tested on stm32f103 only |
| fdcan | STM32 HAL | Tested on stm32g0 only |
| socketcan | [platform_specific_components](https://github.com/OpenCyphal-Garage/platform_specific_components/tree/master/socketcan/libcanard) | Tested on ubuntu 22.04 |

## 1. Prerequisites

The library depends on a few other repositories.

Already inlcuded in the repository:
- [libcanard](https://github.com/OpenCyphal/libcanard)
- [platform_specific_components](https://github.com/OpenCyphal-Garage/platform_specific_components)
- [o1heap](https://github.com/pavel-kirienko/o1heap)
- [public_regulated_data_types](https://github.com/OpenCyphal/public_regulated_data_types)

External dependency:
- [libparams](https://github.com/PonomarevDA/libparams/tree/main/libparams)

## 2. Usage:

**Step 1. Clone repo with dependency**

```bash
git clone https://github.com/RaccoonlabDev/cyphal_application.git
cd cyphal_application
git clone --branch v0.7.0 https://github.com/PonomarevDA/libparams.git Libs/libparams
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

**Step 3. (Optionally) Provide an additional info**

It is nice to fill GetInfo with an information specific to your application:

|||
|-|-|
| Software version | Defined on the compile time with `APP_VERSION_MAJOR` define. |
| Hardware version | Should be defined on the initialization time. |
| Certificate of authenticity | Not supported by this example yet. |
| Unique ID | It is expected to provide `HAL_GetUIDw0`, `HAL_GetUIDw1` and `HAL_GetUIDw2`. By default, it is provided by STM32 HAL. |
| Software VCS Revision ID | Defined on the compile time with `GIT_HASH` define. |

## 3. Usage examples:

| Platform  | Example |
| --------- | ------- |
| stm32f103 | [Mini v2 example](https://github.com/RaccoonlabDev/mini_v2_node) |
| Ubuntu    | [Minimal application example](examples/ubuntu_minimal) |
| Ubuntu    | [Publisher example](examples/ubuntu_publisher_example) |
| Ubuntu    | [PX4/ArduPilot HITL communicator (Cyphal <-> ROS converter)](https://github.com/RaccoonlabDev/cyphal_communicator) |
