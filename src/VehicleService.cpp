#define LOG_TAG "automotive.vehicle@2.0-service"

#include <android/log.h>
#include <android/binder_process.h>
#include <hidl/HidlTransportSupport.h>
#include <iostream>
#include <utils/Looper.h>

#include <vhal_v2_0/EmulatedVehicleConnector.h>
#include <vhal_v2_0/EmulatedVehicleHal.h>
#include <vhal_v2_0/VehicleHalManager.h>
#include <vhal_v2_0/WatchdogClient.h>

#include "inc/VssVehicleEmulator.h"


using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;

int main(int /* argc */, char* /* argv */ []) {
    auto store = std::make_unique<VehiclePropertyStore>();
    auto connector = std::make_unique<impl::EmulatedVehicleConnector>();
    auto hal = std::make_unique<impl::EmulatedVehicleHal>(store.get(), connector.get());
    auto emulator = std::make_unique<impl::VssVehicleEmulator>(hal.get());
    auto service = std::make_unique<VehicleHalManager>(hal.get());
    connector->setValuePool(hal->getValuePool());

    configureRpcThreadpool(4, false /* callerWillJoin */);

    ALOGI("Registering as service...");
    status_t status = service->registerAsService();

    if (status != OK) {
        ALOGE("Unable to register vehicle service (%d)", status);
        return 1;
    }

    // Setup a binder thread pool to be a car watchdog client.
    ABinderProcess_setThreadPoolMaxThreadCount(1);
    ABinderProcess_startThreadPool();
    sp<Looper> looper(Looper::prepare(0 /* opts */));
    std::shared_ptr<WatchdogClient> watchdogClient =
            ndk::SharedRefBase::make<WatchdogClient>(looper, service.get());
    // The current health check is done in the main thread, so it falls short of capturing the real
    // situation. Checking through HAL binder thread should be considered.
    if (!watchdogClient->initialize()) {
        ALOGE("Failed to initialize car watchdog client");
        return 1;
    }
    ALOGI("Ready");
    while (true) {
        looper->pollAll(-1 /* timeoutMillis */);
    }

    return 1;
}
