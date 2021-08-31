#include "inc/AndroidVssConverter.h"

#define LOG_TAG "AndroidVssConverter"

#include <log/log.h>
#include <vhal_v2_0/VehicleUtils.h>

#include "inc/ConverterUtils.h"


namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

VehiclePropValue AndroidVssConverter::convertProperty(std::string id, std::string value, VehicleHal* vhal) {
    if (conversionMap.empty()) {
        initConversionMap(vhal);
    }

    // TODO error handling like missing id
    return conversionMap[id](value);
}




/** BEGIN GENERATED SECTION **/
static VehiclePropValue convertFuelLevel(std::string value, VehicleProperty id, int32_t area, VehicleHal* vhal) {
    float fuelCapacity = getVehiclePropertyFloatValue(toInt(VehicleProperty::INFO_FUEL_CAPACITY), vhal);
    
    return convertFloatLinear(value, id, area, 0, fuelCapacity);
}

void AndroidVssConverter::initConversionMap(VehicleHal* vhal) {
    conversionMap.clear();

    conversionMap["Vehicle.ADAS.ABS.IsActive"] = std::bind(convertBool,
            std::placeholders::_1, VehicleProperty::ABS_ACTIVE, toInt(VehicleArea::GLOBAL));
    conversionMap["Vehicle.Powertrain.CombustionEngine.Engine.EOT"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::ENGINE_OIL_TEMP, toInt(VehicleArea::GLOBAL), 1.0f, 0.0f);
    conversionMap["Vehicle.Powertrain.FuelSystem.Level"] = std::bind(convertFuelLevel,
            std::placeholders::_1, VehicleProperty::FUEL_LEVEL, toInt(VehicleArea::GLOBAL), vhal);
    conversionMap["Vehicle.Chassis.Axle.Row1.Wheel.Left.Tire.Pressure"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::TIRE_PRESSURE, (int32_t) VehicleAreaWheel::LEFT_FRONT, 1.0f, 0.0f);
    conversionMap["Vehicle.Chassis.Axle.Row1.Wheel.Right.Tire.Pressure"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::TIRE_PRESSURE, (int32_t) VehicleAreaWheel::RIGHT_FRONT, 1.0f, 0.0f);
    conversionMap["Vehicle.Chassis.Axle.Row2.Wheel.Left.Tire.Pressure"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::TIRE_PRESSURE, (int32_t) VehicleAreaWheel::LEFT_REAR, 1.0f, 0.0f);
    conversionMap["Vehicle.Chassis.Axle.Row2.Wheel.Right.Tire.Pressure"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::TIRE_PRESSURE, (int32_t) VehicleAreaWheel::RIGHT_REAR, 1.0f, 0.0f);
    conversionMap["Vehicle.Speed"] = std::bind(convertFloatLinear,
            std::placeholders::_1, VehicleProperty::PERF_VEHICLE_SPEED, toInt(VehicleArea::GLOBAL), 1.0f / 3.6f, 0.0f);   
}
 /** END GENERATED SECTION **/


}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
