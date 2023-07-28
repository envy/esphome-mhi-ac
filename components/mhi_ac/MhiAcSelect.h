#pragma once

#include "MhiAc.h"

#include "esphome/components/select/select.h"

using namespace esphome;
using namespace esphome::select;

static const char* TAG = "mhi_ac_select";

class MhiAcVerticalVanesSelect : public Component, public Select, public MhiAcVerticalVanesSelectCallback {
private:
	MhiAc *mhi_ac = nullptr;

public:
	void dump_config() override {
		LOG_SELECT("", "MhiAcVerticalVanesSelect", this);
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void control(const std::string &value) override {
		auto options = this->traits.get_options();
		if (value.compare(options[0]) == 0) {
			this->mhi_ac->mhi_set_vertical_vanes(vanes_1);
		} else if (value.compare(options[1]) == 0) {
			this->mhi_ac->mhi_set_vertical_vanes(vanes_2);
		} else if (value.compare(options[2]) == 0) {
			this->mhi_ac->mhi_set_vertical_vanes(vanes_3);
		} else if (value.compare(options[3]) == 0) {
			this->mhi_ac->mhi_set_vertical_vanes(vanes_4);
        } else if (value.compare(options[4]) == 0) {
            this->mhi_ac->mhi_set_vertical_vanes(vanes_swing);
        }
	}

	void mhi_set_vertical_vanes(ACVanes vanes) override {
		auto options = this->traits.get_options();
		switch(vanes) {
		case vanes_unknown:
			publish_state("Unknown");
			break;
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
		}
	}
};
