#ifndef android_hardware_automotive_vehicle_V2_0_impl_VssCommConn_H_
#define android_hardware_automotive_vehicle_V2_0_impl_VssCommConn_H_

#include <string>
#include <thread>
#include <vector>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {


class VssMessageProcessor {
   public:
    virtual ~VssMessageProcessor() = default;

    virtual void doSetProperty(std::string vssString);
};

class VssCommConn {
   public:
    VssCommConn(VssMessageProcessor* messageProcessor) : mMessageProcessor(messageProcessor) {}

    virtual ~VssCommConn() {}

    /**
     * Start the read thread reading messages from this connection.
     */
    virtual void start();

    /**
     * Closes a connection if it is open.
     */
    virtual void stop();

    /**
     * Returns true if the connection is open and available to send/receive.
     */
    virtual bool isOpen() = 0;

    virtual std::string read() = 0;


   protected:
    std::unique_ptr<std::thread> mReadThread;
    VssMessageProcessor* mMessageProcessor;

    /**
     * A thread that reads messages in a loop, and responds. You can stop this thread by calling
     * stop().
     */
    void readThread();
};

}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif  // android_hardware_automotive_vehicle_V2_0_impl_VssCommConn_H_
