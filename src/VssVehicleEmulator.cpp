#include "inc/VssVehicleEmulator.h"

#include "inc/AndroidVssConverter.h"
#include <vector>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

VssVehicleEmulator::VssVehicleEmulator(EmulatedVehicleHalIface* hal) : VehicleEmulator(hal), mHal{hal} {
    ALOGI("Starting VssSocketComm");
    mVssSocketComm = std::make_unique<VssSocketComm>(this);
    mVssSocketComm->start();
    mVssConverter = std::make_unique<AndroidVssConverter>();
}

VssVehicleEmulator::~VssVehicleEmulator() {
    mVssSocketComm->stop();
}

static std::vector<std::string> split(std::string str, std::string delimiter) {
    std::vector<std::string> vecStr {};
    size_t last = 0;
    size_t next = 0;
    while ((next = str.find(delimiter, last)) != std::string::npos) {
        vecStr.push_back(str.substr(last, next - last));
        last = next + 1;
    }
    vecStr.push_back(str.substr(last));

    return vecStr;
}

void VssVehicleEmulator::doSetProperty(std::string vssString) {
    std::vector<std::string> vecStr = split(vssString, ",");
    std::string id = vecStr[0];
    std::string value = vecStr[1];
    ALOGI("Property id: %s, value: %s", id.c_str(), value.c_str());

    VehiclePropValue val = mVssConverter->convertProperty(id, value, mHal);
    mHal->setPropertyFromVehicle(val);
}

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // 