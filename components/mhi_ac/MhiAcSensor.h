#pragma once

#include "MhiAc.h"

#include "esphome/components/sensor/sensor.h"

using namespace esphome;
using namespace esphome::sensor;

class MhiAcSensor : public Component, public MhiAcSensorCallback
{
private:
	MhiAc *mhi_ac = nullptr;

	Sensor* mhi_outdoor_temperature = nullptr;
	Sensor* mhi_current = nullptr;
	Sensor* mhi_energy = nullptr;
	Sensor* mhi_protection_state = nullptr;

public:
	void setup() override {
	}

	void loop() override {
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void set_outdoor_temperature_sensor(Sensor *outdoor_temperature) {
		mhi_outdoor_temperature = outdoor_temperature;
	}

	void mhi_set_outdoor_temperature(float temp) override {
		if (mhi_outdoor_temperature) {
			mhi_outdoor_temperature->publish_state(temp);
		}
	}

	void set_current_sensor(Sensor *current) {
		mhi_current = current;
	}

	void mhi_set_current(float current) override {
		if (mhi_current) {
			mhi_current->publish_state(current);
		}
	}

	void set_energy_sensor(Sensor *energy) {
		mhi_energy = energy;
	}

	void mhi_set_energy(float energy) override {
		if (mhi_energy) {
			mhi_energy->publish_state(energy);
		}
	}

	void set_protection_state_sensor(Sensor *protection_state) {
		mhi_protection_state = protection_state;
	}

	void mhi_set_protection_state(int state) override {
		if (mhi_protection_state) {
			mhi_protection_state->publish_state(state);
		}
	}
	

	/*
	sensor::Sensor* error_code_;
	sensor::Sensor* return_air_temperature_;
	sensor::Sensor* outdoor_unit_fan_speed_;
	sensor::Sensor* indoor_unit_fan_speed_;
	sensor::Sensor* compressor_frequency_;
	sensor::Sensor* indoor_unit_total_run_time_;
	sensor::Sensor* compressor_total_run_time_;
	sensor::Sensor* current_;
	*/
	//sensor::BinarySensor* defrost_;
};