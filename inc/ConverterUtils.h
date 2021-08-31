#ifndef android_hardware_automotive_vehicle_V2_0_impl_vss_ConverterUtils_H_
#define android_hardware_automotive_vehicle_V2_0_impl_vss_ConverterUtils_H_

#include <vhal_v2_0/VehicleHal.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

VehiclePropValue initializeProp(VehicleProperty id, int32_t area);

// TODO create utils like this for other types
VehiclePropValue convertBool(std::string value, VehicleProperty id, int32_t area);

// TODO create utils like this for other types
VehiclePropValue convertFloatLinear(std::string value, VehicleProperty id, int32_t area, float K, float m);
// TODO create utils like this for other types
float getVehiclePropertyFloatValue(int propId, VehicleHal* vhal);

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif // android_hardware_automotive_vehicle_V2_0_impl_vss_ConverterUtils_H_