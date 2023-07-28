# esphome-mhi-ac
Component for esphome to use SPI to connect to a Mitsubishi Heavy Industries AC

# Authors
This code is based on https://github.com/tfyoung/esphome-MHIHeatPump


## ESPHome Config
Example config:

```yaml
external_components:
  - source: github://envy/esphome-mhi-ac

# Exactly one mhi_ac must exist in the configuration
mhi_ac:
  # Optional configuration with default values below
  processing_time: 100  # Time in ms that the component should communicate with the AC during each loop cycle.

climate:
  - platform: mhi_ac
    name: AC
    # Optional configuration with default values below
    vertical_swing_off_position: 2  # When swing is turned off, this is the position the vertical vanes will go to. Valid values: 1, 2, 3, 4

sensor:
  - platform: mhi_ac
    # All sensors below are optional.
    outdoor_temperature:
      name: Outdoor Temperature
    current:
      name: Current
    energy:
      name: Energy

select:
  - platform: mhi_ac
    # All selects below are optional
    vertical_vanes:
      name: Vertical Vanes

```

