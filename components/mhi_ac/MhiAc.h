#pragma once

#include "MHI-AC-Ctrl-core.h"

#include "esphome/components/climate/climate.h"

using namespace esphome;

class MhiAcClimateCallback {
public:
	virtual void mhi_set_power(bool) = 0;
	virtual void mhi_set_climate_mode(climate::ClimateMode) = 0;
	virtual void mhi_set_fan_mode(climate::ClimateFanMode) = 0;
	virtual void mhi_set_vertical_swing_mode(bool) = 0;
	virtual void mhi_set_horizontal_swing_mode(bool) = 0;
	virtual void mhi_update_swing_mode() = 0;
	virtual void mhi_set_room_temperature(float) = 0;
	virtual void mhi_set_target_temperature(float) = 0;
};

class MhiAcSensorCallback {
public:
	virtual void mhi_set_outdoor_temperature(float) = 0;
	virtual void mhi_set_current(float) = 0;
	virtual void mhi_set_energy(float) = 0;
	virtual void mhi_set_protection_state(int) = 0;
};

class MhiAcVerticalVanesSelectCallback {
public:
	virtual void mhi_set_vertical_vanes(ACVanes) = 0;
};

class MhiAcHorizontalVanesSelectCallback {
public:
	virtual void mhi_set_horizontal_vanes(ACVanesLR) = 0;
};

class MhiAc : public Component, public CallbackInterface_Status {
private:
	MHI_AC_Ctrl_Core ac;
	MhiAcClimateCallback *climateCb = nullptr;
	MhiAcSensorCallback *sensorCb = nullptr;
	MhiAcVerticalVanesSelectCallback *verticalVanesSelectCb = nullptr;
	MhiAcHorizontalVanesSelectCallback *horizontalVanesSelectCb = nullptr;

	uint16_t processing_time = 100;
	bool use_extended_frame_format = false;

public:
	void setup() override {
		ac.MHIAcCtrlStatus(this);
		ac.init();
		if (use_extended_frame_format) {
			ac.set_frame_size(33);
		}
		ac.reset_old_values();
		// We call ac.loop() once to test communication, otherwise mark us as failed
		if (ac.loop(this->processing_time) < 0) {
			this->mark_failed();
		}
	}

	void loop() override {
		ac.loop(this->processing_time);
	}

	void set_processing_time(uint16_t time) {
		this->processing_time = time;
	}

	void set_use_extended_frame_format(bool b) {
		this->use_extended_frame_format = b;
	}

	void set_climate_cb(MhiAcClimateCallback *cb) {
		this->climateCb = cb;
	}

	void set_sensor_cb(MhiAcSensorCallback *cb) {
		this->sensorCb = cb;
	}

	void set_vertical_vanes_select_cb(MhiAcVerticalVanesSelectCallback *cb) {
		this->verticalVanesSelectCb = cb;
	}

	void set_horizontal_vanes_select_cb(MhiAcHorizontalVanesSelectCallback *cb) {
		this->horizontalVanesSelectCb = cb;
	}

	void mhi_set_power(bool power) {
		ESP_LOGD("mhi_ac", "Setting power to %s", YESNO(power));
		this->ac.set_power(power);
	}

	void mhi_set_climate_mode(climate::ClimateMode mode) {
		ESP_LOGD("mhi_ac", "Setting climate mode to %d", mode);
		switch (mode) {
		case climate::CLIMATE_MODE_OFF:
			this->ac.set_power(false);
			break;
		case climate::CLIMATE_MODE_COOL:
			this->ac.set_mode(mode_cool);
			this->ac.set_power(true);
			break;
		case climate::CLIMATE_MODE_HEAT:
			this->ac.set_mode(mode_heat);
			this->ac.set_power(true);
			break;
		case climate::CLIMATE_MODE_FAN_ONLY:
			this->ac.set_mode(mode_fan);
			this->ac.set_power(true);
			break;
		case climate::CLIMATE_MODE_DRY:
			this->ac.set_mode(mode_dry);
			this->ac.set_power(true);
			break;
		case climate::CLIMATE_MODE_AUTO:
			this->ac.set_mode(mode_auto);
			this->ac.set_power(true);
			break;
		default:
			// Other modes not supported.
			ESP_LOGW("mhi_ac", "Got unsupported climate mode %d", mode);
			break;
		}
	}

	void mhi_set_fan_mode(climate::ClimateFanMode mode) {
		ESP_LOGD("mhi_ac", "Setting fan mode to %d", mode);
		switch (mode) {
		case climate::CLIMATE_FAN_QUIET:
			this->ac.set_fan(0);
			break;
		case climate::CLIMATE_FAN_LOW:
			this->ac.set_fan(1);
			break;
		case climate::CLIMATE_FAN_MEDIUM:
			this->ac.set_fan(2);
			break;
		case climate::CLIMATE_FAN_HIGH:
			this->ac.set_fan(6);
			break;
		case climate::CLIMATE_FAN_AUTO:
			this->ac.set_fan(7);
			break;
		default:
			// Other modes not supported.
			ESP_LOGW("mhi_ac", "Got unsupported fan mode %d", mode);
			break;
		}
	}

	void mhi_set_target_temperature(float temp) {
		ESP_LOGD("mhi_ac", "Setting target temperature to %f", temp);
		float t = roundf(clamp(temp, 18.0f, 30.0f));
		this->ac.set_tsetpoint((byte)(t * 2));
	}

	void mhi_set_vertical_vanes(ACVanes value) {
		ESP_LOGD("mhi_ac", "Setting vertical vanes to %d", value);
		this->ac.set_vanes(value);
	}

	void mhi_set_horizontal_vanes(ACVanesLR value) {
		ESP_LOGD("mhi_ac", "Setting horizontal vanes to %d", value);
		this->ac.set_vanesLR(value);
	}

	static optional<climate::ClimateMode> modeToClimate(const int &mode) {
		switch (mode) {
			case mode_heat: return climate::CLIMATE_MODE_HEAT;
			case mode_cool: return climate::CLIMATE_MODE_COOL;
			case mode_dry: return climate::CLIMATE_MODE_DRY;
			case mode_fan: return climate::CLIMATE_MODE_FAN_ONLY;
			case mode_auto: return climate::CLIMATE_MODE_AUTO;
			default:
				ESP_LOGD("mhi_ac", "Got unknown climate mode %d", mode);
				break;
		}
		return {};
	}

	static optional<climate::ClimateFanMode> modeToFan(const int &mode) {
		switch (mode) {
			case 0: return climate::CLIMATE_FAN_QUIET;
			case 1: return climate::CLIMATE_FAN_LOW;
			case 2: return climate::CLIMATE_FAN_MEDIUM;
			case 6: return climate::CLIMATE_FAN_HIGH;
			case 7: return climate::CLIMATE_FAN_AUTO;
			default:
				ESP_LOGD("mhi_ac", "Got unknown fan mode %d", mode);
				break;
		}
		return {};
	}

	void cbiStatusFunction(ACStatus status, int value) override {
		// https://www.hrponline.co.uk/media/pdf/41/42/ed/Beijer-Ref-Service-Support-Handbook-19cWKESQUhzVIy5.pdf#page=9
		switch (status) {
		case status_mode:
		case opdata_mode:
			ESP_LOGD("mhi_ac", "Got opdata_mode %x (%d)", value, status);
			{
				auto mode = modeToClimate(value);
				if (mode.has_value() && climateCb) {
					climateCb->mhi_set_climate_mode(mode.value());
				}
			}
			break;
		case erropdata_mode:
			ESP_LOGD("mhi_ac", "Got erropdata_mode %x", value);
			{
				auto mode = modeToClimate(value);
				if (mode.has_value() && climateCb) {
					climateCb->mhi_set_climate_mode(mode.value());
				}
			}
			break;
		case status_power:
			ESP_LOGD("mhi_ac", "Got status_power %d", value);
			{
				if (climateCb) {
					climateCb->mhi_set_power(value == power_on);
				}
			}
			break;
		case status_troom:
			ESP_LOGD("mhi_ac", "Got status_troom %f °C", (value - 61) / 4.0);
			{
				if (climateCb) {
					climateCb->mhi_set_room_temperature((value - 61) / 4.0);
				}
			}
			break;
		case status_fan:
			ESP_LOGD("mhi_ac", "Got status_fan %d", value);
			{
				auto mode = modeToFan(value);
				if (mode.has_value() && climateCb) {
					climateCb->mhi_set_fan_mode(mode.value());
				}
				
			}
			break;
		case status_tsetpoint:
			ESP_LOGD("mhi_ac", "Got status_tsetpoint %d", value);
			{
				float temp = (value & 0x7f) / 2.0;
				if (climateCb) {
					climateCb->mhi_set_target_temperature(temp);
				}
			}
			break;
		case status_vanes:
			ESP_LOGD("mhi_ac", "Got status_vanes %d", value);
			{
				if (verticalVanesSelectCb) {
					verticalVanesSelectCb->mhi_set_vertical_vanes((ACVanes)value);
				}
				if (climateCb) {
					climateCb->mhi_set_vertical_swing_mode(value == vanes_swing);
					climateCb->mhi_update_swing_mode();
				}
			}
			break;
		case status_vanesLR:
			ESP_LOGD("mhi_ac", "Got status_vanesLR %d", value);
			{
				if (horizontalVanesSelectCb) {
					horizontalVanesSelectCb->mhi_set_horizontal_vanes((ACVanesLR)value);
				}
				if (climateCb) {
					climateCb->mhi_set_horizontal_swing_mode(value == vanesLR_swing);
					climateCb->mhi_update_swing_mode();
				}
			}
			break;
		case status_errorcode:
			ESP_LOGD("mhi_ac", "Got status_errorcode %d", value);
			{
			}
			break;
		case opdata_tsetpoint:
			ESP_LOGD("mhi_ac", "Got opdata_tsetpoint %d", value);
			{
				float temp = (value & 0x7f) / 2.0;
				if (climateCb) {
					climateCb->mhi_set_target_temperature(temp);
				}
			}
			break;
		case opdata_outdoor:
		case erropdata_outdoor:
			ESP_LOGD("mhi_ac", "Got opdata_outdoor %d (%d)", value, status);
			{
				float temp = (value - 94) * 0.25f;
				if (sensorCb) {
					sensorCb->mhi_set_outdoor_temperature(temp);
				}
			}
			break;
		case opdata_ct:
		case erropdata_ct:
			ESP_LOGD("mhi_ac", "Got opdata_ct %d (%d)", value, status);
			{
				float current = (value * 14.0f) / 51.0f;
				if (sensorCb) {
					sensorCb->mhi_set_current(current);
				}
			}
			break;
		case opdata_kwh:
			ESP_LOGD("mhi_ac", "Got opdata_kwh %d", value);
			{
				float energy = value * 0.25f;
				if (sensorCb) {
					sensorCb->mhi_set_energy(energy);
				}
			}
			break;
		case opdata_iu_fanspeed:
		case erropdata_iu_fanspeed:
			// TODO: indoor fan speed sensor, unit same as what you can set
			break;
		case opdata_ou_fanspeed:
		case erropdata_ou_fanspeed:
			// TODO: outdoor fan speed sensor, unit ???
			break;
		case opdata_defrost:
			// TODO: defrost binary sensor
			break;
		case opdata_total_comp_run:
		case erropdata_total_comp_run:
			// TODO: compressor run time sensor, (value * 100), unit ???
			break;
		case opdata_protection_no:
			ESP_LOGD("mhi_ac", "Got opdata_protection_no %d", value);
			{
				if (sensorCb) {
					sensorCb->mhi_set_protection_state(value);
				}
			}
			break;
		case opdata_comp:
		case erropdata_comp:
			// TODO: compressor frequency sensor
			break;
		case opdata_return_air:
		case erropdata_return_air:
			// TODO: return air temp sensor, (value * 0.25f - 15), unit °C ?
			break;
		case opdata_total_iu_run:
		case erropdata_total_iu_run:
			// TODO: indoor unit run time, (value * 100), unit ???
			break;
		case opdata_tdsh:
			// outdoor unit discharge pipe super heat ???
			break;
		case opdata_td:
		case erropdata_td:
			// outdoor unit discharge pipe ???
			break;
		case opdata_tho_r1:
		case erropdata_tho_r1:
			// Outdoor unit some temperature?, (0.327f * value - 11.4f), unit °C?
			break;
		case opdata_thi_r1:
		case erropdata_thi_r1:
			// Indoor heat exchanger temperature 1 (u-bend), (0.327f * value - 11.4f), unit °C?
			break;
		case opdata_thi_r2:
		case erropdata_thi_r2:
			// Indoor heat exchanger temperature 2 (capillary), (0.327f * value - 11.4f), unit °C?
			break;
		case opdata_thi_r3:
		case erropdata_thi_r3:
			// Indoor heat exchanger temperature 3 (suction heater), (0.327f * value - 11.4f), unit °C?
			break;
		case opdata_ou_eev1:
		case erropdata_ou_eev1:
			// Outdoor unit expansion valve ???
			break;
		default:
			ESP_LOGD("mhi_ac", "Got unknown callback, status: %d, value: %d", status, value);
			break;
		}
	}
};
