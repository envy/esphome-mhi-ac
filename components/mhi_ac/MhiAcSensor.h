#pragma once

#include "MhiAc.h"

#include "esphome/components/climate/climate.h"

//#include "esphome/components/sensor/sensor.h"

#define TROOM_FILTER_LIMIT 0.25

using namespace esphome;
using namespace esphome::climate;

class MhiAcSensor : public Component, public MhiAcSensorCallback
{
private:
	MhiAc *mhi_ac = nullptr;

    sensor::Sensor* mhi_outdoor_temperature = nullptr;

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

    void mhi_set_outdoor_temperature(float temp) {
        if (mhi_outdoor_temperature) {
            mhi_outdoor_temperature->publish_state(temp);
        }
    }

    /*
	void set_current_sensor(sensor::Sensor *current) {
		current_ = current;
	}
	*/
	

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