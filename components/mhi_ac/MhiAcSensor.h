#pragma once

#include "MhiAc.h"

using namespace esphome;

class MhiAcSensor : public Component, public MhiAcSensorCallback
{
private:
	MhiAc *mhi_ac = nullptr;

	sensor::Sensor* mhi_outdoor_temperature = nullptr;
	sensor::Sensor* mhi_current = nullptr;
	sensor::Sensor* mhi_energy = nullptr;

public:
	void setup() override {
	}

	void loop() override {
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void set_outdoor_temperature_sensor(sensor::Sensor *outdoor_temperature) {
		mhi_outdoor_temperature = outdoor_temperature;
	}

	void mhi_set_outdoor_temperature(float temp) override {
		if (mhi_outdoor_temperature) {
			mhi_outdoor_temperature->publish_state(temp);
		}
	}

	void set_current_sensor(sensor::Sensor *current) {
		mhi_current = current;
	}

	void mhi_set_current(float current) override {
		if (mhi_current) {
			mhi_current->publish_state(current);
		}
	}

	void set_energy_sensor(sensor::Sensor *energy) {
		mhi_energy = energy;
	}

	void mhi_set_energy(float energy) override {
		if (mhi_energy) {
			mhi_energy->publish_state(energy);
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