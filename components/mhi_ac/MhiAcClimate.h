#pragma once

#include "MhiAc.h"

#include "esphome/components/climate/climate.h"

using namespace esphome;
using namespace esphome::climate;

class MhiAcClimate : public Component, public Climate, public MhiAcClimateCallback
{
private:
	MhiAc *mhi_ac = nullptr;
	bool mhi_on;
	ClimateMode mhi_climate_mode;

	uint8_t mhi_vertical_swing_off_position = vanes_2; // mid-up
	uint8_t mhi_horizontal_swing_off_position = vanesLR_3; // middle

	bool mhi_last_vertical_swing;
	bool mhi_last_horizontal_swing;

public:
	void setup() override {
		this->mhi_on = false;
		auto restore = this->restore_state_();
		if (restore.has_value()) {
			restore->to_call(this).perform();
		} else {
			this->current_temperature = NAN;
			this->target_temperature = 22.0f;
			this->mode = CLIMATE_MODE_OFF;
			this->fan_mode = CLIMATE_FAN_AUTO;
			this->swing_mode = CLIMATE_SWING_OFF;
		}
	}

	void loop() override {
		ESP_LOGD("mhi_ac", "climate loop called");
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void set_vertical_swing_off_position(uint8_t pos) {
		this->mhi_vertical_swing_off_position = pos;
	}

	void set_horizontal_swing_off_position(uint8_t pos) {
		this->mhi_horizontal_swing_off_position = pos;
	}

	void control(const ClimateCall &call) override {
		if (call.get_mode().has_value()) {
			ClimateMode mode = call.get_mode().value();
			if (mode == CLIMATE_MODE_OFF) {
				this->mhi_climate_mode = CLIMATE_MODE_OFF;
				// Setting climate mode to off does not work for some reason?!?!
				// this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_OFF);
				this->mhi_ac->mhi_set_power(false);
				this->mhi_on = false;
			} else if (mode == CLIMATE_MODE_HEAT) {
				this->mhi_climate_mode = CLIMATE_MODE_HEAT;
				this->mhi_on = true;
				this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_HEAT);
			} else if (mode == CLIMATE_MODE_COOL) {
				this->mhi_climate_mode = CLIMATE_MODE_COOL;
				this->mhi_on = true;
				this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_COOL);
			} else if (mode == CLIMATE_MODE_FAN_ONLY) {
				this->mhi_climate_mode = CLIMATE_MODE_FAN_ONLY;
				this->mhi_on = true;
				this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_FAN_ONLY);
			} else if (mode == CLIMATE_MODE_DRY) {
				this->mhi_climate_mode = CLIMATE_MODE_DRY;
				this->mhi_on = true;
				this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_DRY);
			} else if (mode == CLIMATE_MODE_AUTO) {
				this->mhi_climate_mode = CLIMATE_MODE_AUTO;
				this->mhi_on = true;
				this->mhi_ac->mhi_set_climate_mode(CLIMATE_MODE_AUTO);
			}
			
			this->mode = mode;
			this->publish_state();
		}

		if (call.get_target_temperature().has_value()) {
			this->mhi_ac->mhi_set_target_temperature(call.get_target_temperature().value());
			this->target_temperature = call.get_target_temperature().value();
			this->publish_state();
		}

		if (call.get_fan_mode().has_value()) {
			this->mhi_ac->mhi_set_fan_mode(call.get_fan_mode().value());
			this->fan_mode = call.get_fan_mode().value();
			this->publish_state();
		}

		if (call.get_swing_mode().has_value()) {
			auto mode = call.get_swing_mode().value();
			switch (mode) {
			case CLIMATE_SWING_OFF:
				this->mhi_ac->mhi_set_vertical_vanes((ACVanes)this->mhi_vertical_swing_off_position);
				this->mhi_ac->mhi_set_horizontal_vanes((ACVanesLR)this->mhi_horizontal_swing_off_position);
				break;
			case CLIMATE_SWING_VERTICAL:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_swing);
				break;
			case CLIMATE_SWING_HORIZONTAL:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_swing);
				break;
			case CLIMATE_SWING_BOTH:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_swing);
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_swing);
				break;
			default:
				ESP_LOGD("mhi_ac", "Got unsupported swing mode %d", mode);
				break;
			}
		}
	}

	ClimateTraits traits() override {
		// The capabilities of the climate device
		auto traits = ClimateTraits();
		traits.add_feature_flags(CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
		traits.set_supported_modes({CLIMATE_MODE_OFF, CLIMATE_MODE_COOL, CLIMATE_MODE_HEAT, CLIMATE_MODE_FAN_ONLY, CLIMATE_MODE_DRY});
		traits.set_supported_fan_modes({CLIMATE_FAN_QUIET, CLIMATE_FAN_LOW, CLIMATE_FAN_MEDIUM, CLIMATE_FAN_HIGH, CLIMATE_FAN_AUTO});
		traits.set_supported_swing_modes({CLIMATE_SWING_OFF, CLIMATE_SWING_VERTICAL, CLIMATE_SWING_HORIZONTAL, CLIMATE_SWING_BOTH});
		traits.set_visual_min_temperature(18.0f);
		traits.set_visual_max_temperature(30.0f);
		traits.set_visual_target_temperature_step(1.0f);
		traits.set_visual_current_temperature_step(0.25f);

		return traits;
	}

	void mhi_set_power(bool power) override {
		this->mhi_on = power;
		ESP_LOGD("mhi_ac_climate", "Sending power mode %s", YESNO(power));
		if (this->mhi_on) {
			this->mode = this->mhi_climate_mode;
			publish_state();
		} else {
			this->mode = CLIMATE_MODE_OFF;
			publish_state();
		}
	}

	void mhi_set_climate_mode(ClimateMode mode) override {
		// Always save last received climate mode
		this->mhi_climate_mode = mode;

		if (this->mhi_on) {
			ESP_LOGD("mhi_ac_climate", "Sending climate mode %d", mode);
			this->mode = this->mhi_climate_mode;
			publish_state();
		}
	}

	void mhi_set_fan_mode(ClimateFanMode mode) override {
		if (this->mhi_on) {
			ESP_LOGD("mhi_ac_climate", "Sending fan mode %d", mode);
			this->fan_mode = mode;
			publish_state();
		}
	}

	void mhi_set_vertical_swing_mode(bool swing) override {
		this->mhi_last_vertical_swing = swing;
	}

	void mhi_set_horizontal_swing_mode(bool swing) override {
		this->mhi_last_horizontal_swing = swing;
	}

	void mhi_update_swing_mode() override {
		if (this->mhi_on) {
			if (mhi_last_vertical_swing && mhi_last_horizontal_swing) {
				this->swing_mode = CLIMATE_SWING_BOTH;
			} else if (mhi_last_vertical_swing && !mhi_last_horizontal_swing) {
				this->swing_mode = CLIMATE_SWING_VERTICAL;
			} else if (!mhi_last_vertical_swing && mhi_last_horizontal_swing) {
				this->swing_mode = CLIMATE_SWING_HORIZONTAL;
			} else {
				this->swing_mode = CLIMATE_SWING_OFF;
			}
			ESP_LOGD("mhi_ac_climate", "Sending swing mode %s", climate_swing_mode_to_string(this->swing_mode));
			publish_state();
		}
	}



	void mhi_set_room_temperature(float temp) override {
		ESP_LOGD("mhi_ac_climate", "Sending room temperature %f °C", temp);
		this->current_temperature = temp;
		publish_state();
	}

	void mhi_set_target_temperature(float temp) override {
		ESP_LOGD("mhi_ac_climate", "Sending target temperature %f °C", temp);
		this->target_temperature = temp;
		publish_state();
	}

/*
	void cbiStatusFunction(ACStatus status, int value)
	{
		float offset = m_ac.get_troom_offset();
		float tmp_value;
		static byte status_troom_old=0xff;
		switch(status)
		{
			case opdata_return_air: 
				ESP_LOGI("custom", "Got opdata_return_air temp %f", (value - 61) / 4.0);
				this->current_temperature = (value - 61) / 4.0;
				publish_state();
				break;
			case opdata_outdoor: 
				{
				const float temp = (value - 94) * 0.25f;
				ESP_LOGI("custom", "Got outdoor temp %f", temp);
				if (this->outdoor_temperature_)
					this->outdoor_temperature_->publish_state(temp);
				break;
				}
			case opdata_iu_fanspeed: 
				ESP_LOGI("custom", "Got indoor fan speed %d", value);
				break;
			case opdata_ou_fanspeed: 
				ESP_LOGI("custom", "Got outdoor fan speed %d", value);
				break;
			case opdata_total_comp_run: 
				ESP_LOGI("custom", "Got opdata_total_comp_run %d", value);
				break;
			
			case opdata_thi_r1:
				ESP_LOGI("custom", "Got thi1 %d", (0.327f * value - 11.4f));
				break;
			case opdata_thi_r2:
				ESP_LOGI("custom", "Got thi2 %d", (0.327f * value - 11.4f));
				break;
			case opdata_thi_r3:
				ESP_LOGI("custom", "Got thi3 %d", (0.327f * value - 11.4f));
				break;
			case opdata_tho_r1:
				ESP_LOGI("custom", "Got tho-r1 %d", (0.327f * value - 11.4f));
				break;
			case opdata_total_iu_run:
				ESP_LOGI("custom", "Got opdata_total_iu_run %d", value * 100);
				break;
			case opdata_ou_eev1:
				ESP_LOGI("custom", "Got v %d %d", value, opdata_ou_eev1);
				break;
			case opdata_comp:
				ESP_LOGI("custom", "Got opdata_comp %f", highByte(value) * 25.6f + 0.1f * lowByte(value));
				break;
			case opdata_ct:
				{
					const double amps = value * 14 / 51.0f;
					ESP_LOGI("custom", "Got opdata_ct %f %f", amps, amps * 240);
					if (this->current_) {
						this->current_->publish_state(amps);
					}
					break;
				}
			case opdata_tsetpoint:
				ESP_LOGI("custom", "Got opdata_tsetpoint %d (%d)", value, status);
				break;
			case status_tsetpoint:
				ESP_LOGI("custom", "Got status_tsetpoint %d (%d)", value, status);
				tmp_value = (value & 0x7f)/ 2.0;
				offset = round(tmp_value) - tmp_value;  // Calculate offset when setpoint is changed
				m_ac.set_troom_offset(offset);
				this->target_temperature = (value & 0x7f)/ 2.0;
				publish_state();
				break;
			case status_vanes:
				ESP_LOGI("custom", "Got status_vanes %d", value);
				break;
			case opdata_tdsh:
				ESP_LOGI("custom", "Got opdata_tdsh %d", value);
				break;
			case opdata_td:
				ESP_LOGI("custom", "Got Discharge Pipe Temperature %d", value);
				break;
			case opdata_defrost:
				ESP_LOGI("custom", "Got opdata_defrost %d", value);
				break;
			case opdata_protection_no:
				ESP_LOGI("custom", "Got opdata_protection_no %d", value);
				break;
		//opdata_mode = type_opdata, opdata_tsetpoint, opdata_return_air, opdata_outdoor, opdata_tho_r1, opdata_iu_fanspeed, opdata_thi_r1, opdata_thi_r2, opdata_thi_r3,
	//opdata_ou_fanspeed, opdata_total_iu_run, opdata_total_comp_run, opdata_comp, opdata_ct, opdata_td,
	//opdata_tdsh, opdata_protection_no, opdata_defrost, opdata_ou_eev1, opdata_unknwon,
	
			default:
				ESP_LOGD("custom", "Got status callback %d %d", status, value);
				break;
		}
	}
*/
	/*
	void set_outdoor_temperature_sensor(sensor::Sensor *outdoor_temperature) {
		outdoor_temperature_ = outdoor_temperature;
	}

	void set_current_sensor(sensor::Sensor *current) {
		current_ = current;
	}
	*/
	

	/*
	sensor::Sensor* error_code_;
	sensor::Sensor* outdoor_temperature_;
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