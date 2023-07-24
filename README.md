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
  # No configuration needed/possible (yet)

climate:
  - platform: mhi_ac
    name: "AC"

sensor:
  - platform: mhi_ac
    # All sensors below are optional.
    outdoor_temperature:
      name: Outdoor Temperature
    current:
      name: Current
    energy:
      name: Energy

```

