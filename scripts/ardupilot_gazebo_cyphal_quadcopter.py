#!/usr/bin/env python3
import time
from vehicle import Vehicle

class ArdupilotConfigurator:
    def __init__(self) -> None:
        self.vehicle = Vehicle(autopilot="ArduPilot")
        self.vehicle.connect()

    def reset_params_to_default(self):
        print("")
        print("Step 1. Reset params")
        self.vehicle.reset_params_to_default()

    def configure_common(self):
        print("")
        print("Step 2. Set common params")
        self.vehicle.configure("ardupilot_cyphal_common_params.yaml", verbose=True)

    def configure_registers(self):
        print("")
        print("Step 3. Set cyphal registers")
        self.vehicle.configure("ardupilot_cyphal_gazebo_registers.yaml", verbose=True)

    def force_calibrate(self):
        print("")
        print("Step 4. Force calibrate")
        time.sleep(5)
        self.vehicle.force_calibrate()
        self.vehicle.reboot()

if __name__ == "__main__":
    configurator = ArdupilotConfigurator()
    configurator.reset_params_to_default()
    configurator.configure_common()
    configurator.configure_registers()
    configurator.force_calibrate()
    print("Done")
