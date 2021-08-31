#ifndef android_hardware_automotive_vehicle_V2_0_impl_VssMessageProcessor_H_
#define android_hardware_automotive_vehicle_V2_0_impl_VssMessageProcessor_H_

#include <string>
#include <vhal_v2_0/VehicleHal.h>
#include <vhal_v2_0/VehicleEmulator.h>

#include <inc/AndroidVssConverter.h>
#include "inc/VssSocketComm.h"
#include "inc/VssCommConn.h"


namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {


class VssVehicleEmulator: public VehicleEmulator, public VssMessageProcessor {
   public:
    VssVehicleEmulator(EmulatedVehicleHalIface* hal);
    virtual ~VssVehicleEmulator();

    void doSetProperty(std::string vssString) override;
   private:
    EmulatedVehicleHalIface* mHal;
    std::unique_ptr<AndroidVssConverter> mVssConverter;
    std::unique_ptr<VssSocketComm> mVssSocketComm;
};

}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif  // android_hardware_automotive_vehicle_V2_0_impl_VssMessageProcessor_H_
