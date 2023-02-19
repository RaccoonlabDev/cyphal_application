# Minimal libcanard application example

## Prerequisites

Software dependencies:
- [libcanard](https://github.com/OpenCyphal/libcanard)
- [platform_specific_components](https://github.com/OpenCyphal-Garage/platform_specific_components)
- [o1heap](https://github.com/pavel-kirienko/o1heap)
- [public_regulated_data_types](https://github.com/OpenCyphal/public_regulated_data_types)
- [libparams](https://github.com/PonomarevDA/libparams/tree/main/libparams)

## Usage:

Step 1. Clone repo with submodules

```bash
git clone <this_repo> --recursive
git submodule update --init --recursive
```

Step 2. In your application's cmake file specify the target, include this CMakeLists.txt and add source code to the executable:

```cmake
set(PLATFORM bxcan) # either bxcan or fdcan or socketcan
include(<path_to_this_repo>/Cyphal/CMakeLists.txt)

add_executable(${EXECUTABLE} ${CYPHAL_SRC})
```

## Usage examples:

- [Mini v2 stm32 node](https://github.com/RaccoonlabDev/mini_v2_node)
- [ubuntu simple application](tests/ubuntu)
