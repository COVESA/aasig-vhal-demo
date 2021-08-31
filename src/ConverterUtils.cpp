#include "inc/ConverterUtils.h"

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>


namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {
VehiclePropValue initializeProp(VehicleProperty id, int32_t area) {
    VehiclePropValue val = {
        .timestamp = elapsedRealtimeNano(),
        .areaId = area,
        .prop = toInt(id),
        .status = VehiclePropertyStatus::AVAILABLE
    };

    return val;
}

// TODO create utils like this for other types
VehiclePropValue convertBool(std::string value, VehicleProperty id, int32_t area) {
    VehiclePropValue prop = initializeProp(id, area);
    bool v = value == "true";
    prop.value.int32Values = std::vector<int32_t> { v };
    return prop;
}

// TODO create utils like this for other types
VehiclePropValue convertFloatLinear(std::string value, VehicleProperty id, int32_t area, float K, float m) {
    VehiclePropValue prop = initializeProp(id, area);
    float v = std::stof(value);
    prop.value.floatValues = std::vector<float> { v * K + m };
    return prop;
}

// TODO create utils like this for other types
float getVehiclePropertyFloatValue(int propId, VehicleHal* vhal) {
     VehiclePropValue request = VehiclePropValue {
      .areaId = toInt(VehicleArea::GLOBAL),
      .prop = propId,
    };

    StatusCode halStatus;
    auto valPtr = vhal->get(request, &halStatus);
    // TODO check status, check if floatValues is not empty
    float value = 0;
    if (valPtr != nullptr) {
        value = valPtr->value.floatValues[0];
    }

    return value;
}

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
