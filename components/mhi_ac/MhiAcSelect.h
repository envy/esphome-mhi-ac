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

class MhiAcHorizontalVanesSelect : public Component, public Select, public MhiAcHorizontalVanesSelectCallback {
private:
	MhiAc *mhi_ac = nullptr;

public:
	void dump_config() override {
		LOG_SELECT("", "MhiAcHorizontalVanesSelect", this);
	}

	void set_mhi_ac(MhiAc *ac) {
		this->mhi_ac = ac;
	}

	void control(const std::string &value) override {
		auto options = this->traits.get_options();
		if (value.compare(options[0]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_1);
		} else if (value.compare(options[1]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_2);
		} else if (value.compare(options[2]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_3);
		} else if (value.compare(options[3]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_4);
		} else if (value.compare(options[4]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_5);
		} else if (value.compare(options[5]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_6);
		} else if (value.compare(options[6]) == 0) {
			this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_7);
        } else if (value.compare(options[7]) == 0) {
            this->mhi_ac->mhi_set_horizontal_vanes(vanesLR_swing);
        }
	}

	void mhi_set_horizontal_vanes(ACVanesLR vanes) override {
		auto options = this->traits.get_options();
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
		}
	}
};