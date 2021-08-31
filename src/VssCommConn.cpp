#include "inc/VssCommConn.h"

#define LOG_TAG "VssCommConn"

#include <thread>
#include <log/log.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

void VssCommConn::start() {
    mReadThread = std::make_unique<std::thread>(std::bind(&VssCommConn::readThread, this));
}

void VssCommConn::stop() {
    if (mReadThread->joinable()) {
        mReadThread->join();
    }
}

void VssCommConn::readThread() {
    std::string buffer;
    while (isOpen()) {
        buffer = read();
        if (buffer.size() == 0) {
            ALOGI("%s: Read returned empty message, exiting read loop.", __func__);
            break;
        }

        mMessageProcessor->doSetProperty(buffer);
    }
}

}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
