#ifndef android_hardware_automotive_vehicle_V2_0_impl_vss_AndroidVssConverter_H_
#define android_hardware_automotive_vehicle_V2_0_impl_vss_AndroidVssConverter_H_

#include <functional>
#include <map>

#include <vhal_v2_0/VehicleHal.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

class AndroidVssConverter {
    public:
    VehiclePropValue convertProperty(std::string id, std::string value, VehicleHal* vhal);

    private:
    std::map<std::string, std::function<VehiclePropValue (std::string)>> conversionMap;

    void initConversionMap(VehicleHal* vhal);
};

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android


#endif // android_hardware_automotive_vehicle_V2_0_impl_vss_AndroidVssConverter_H_
