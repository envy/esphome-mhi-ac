#pragma once

#include "MhiAc.h"

#include "esphome/components/select/select.h"

using namespace esphome;
using namespace esphome::select;

static const char* TAG = "mhi_ac_select";
static const char* UNKNOWN = "UNKNOWN";

class MhiAcVerticalVanesSelect : public Component, public Select, public MhiAcVerticalVanesSelectCallback {
private:
	MhiAc *mhi_ac = nullptr;

public:
	void setup() override {
		const auto &options = this->traits.get_options();
		publish_state(UNKNOWN);
	}

	void dump_config() override {
		LOG_SELECT("", "MhiAcVerticalVanesSelect", this);
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void control(size_t index) override {
		switch(index) {
			case 0:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_1);
				break;
			case 1:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_2);
				break;
			case 2:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_3);
				break;
			case 3:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_4);
				break;
			case 4:
				this->mhi_ac->mhi_set_vertical_vanes(vanes_swing);
				break;
		}
	}

	void mhi_set_vertical_vanes(ACVanes vanes) override {
		const auto &options = this->traits.get_options();
		switch(vanes) {
		case vanes_1:
			publish_state(options[0]);
			break;
		case vanes_2:
			publish_state(options[1]);
			break;
		case vanes_3:
			publish_state(options[2]);
			break;
		case vanes_4:
			publish_state(options[3]);
			break;
		case vanes_swing:
			publish_state(options[4]);
			break;
		default:
			publish_state(UNKNOWN);
			break;
		}
	}
};

class MhiAcHorizontalVanesSelect : public Component, public Select, public MhiAcHorizontalVanesSelectCallback {
private:
	MhiAc *mhi_ac = nullptr;

public:
	void setup() override {
		const auto &options = this->traits.get_options();
		publish_state(UNKNOWN);
	}

	void dump_config() override {
		LOG_SELECT("", "MhiAcHorizontalVanesSelect", this);
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void control(size_t index) override {
		switch(index) {
			case 0:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_1);
				break;
			case 1:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_2);
				break;
			case 2:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_3);
				break;
			case 3:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_4);
				break;
			case 4:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_5);
				break;
			case 5:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_6);
				break;
			case 6:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_7);
				break;
			case 7:
				this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_swing);
				break;
		}
	}

	void mhi_set_horizontal_vanes(ACVanesLR vanes) override {
		const auto &options = this->traits.get_options();
		switch(vanes) {
		case vanesLR_1:
			publish_state(options[0]);
			break;
		case vanesLR_2:
			publish_state(options[1]);
			break;
		case vanesLR_3:
			publish_state(options[2]);
			break;
		case vanesLR_4:
			publish_state(options[3]);
			break;
		case vanesLR_5:
			publish_state(options[4]);
			break;
		case vanesLR_6:
			publish_state(options[5]);
			break;
		case vanesLR_7:
			publish_state(options[6]);
			break;
		case vanesLR_swing:
			publish_state(options[7]);
			break;
		default:
			publish_state(UNKNOWN);
			break;
		}
	}
};