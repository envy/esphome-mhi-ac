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
  use_extended_frame_format: False  # Enable features such as horizontal vanes and 3D Auto.

climate:
  - platform: mhi_ac
    name: AC
    # Optional configuration with default values below
    vertical_swing_off_position: 2  # When swing is turned off, this is the position the vertical vanes will go to. Valid values: 1, 2, 3, 4
    horizontal_swing_off_position: 2  # When swing is turned off, this is the position the horizontal vanes will go to. Valid values: 1, 2, 3, 4, 5, 6, 7

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
      # It is possible to configure the names in the select, e.g., to translate them like so:
      up: Up
      mid_up: Mid Up
      mid_down: Mid Down
      down: Down
      swing: Swing
    horizontal_vanes:
      name: Horizontal Vanes
      # It is possible to configure the names in the select, e.g., to translate them like so:
      left: Left
      mid_left: Mid Left
      middle: Middle
      mid_right: Mid Right
      right: Right
      inner: Inner
      outer: Outer
      swing: Swing

```

