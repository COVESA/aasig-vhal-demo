#ifndef android_hardware_automotive_vehicle_V2_0_impl_VssSocketComm_H_
#define android_hardware_automotive_vehicle_V2_0_impl_VssSocketComm_H_

#include <mutex>
#include <thread>
#include <vector>

#include "inc/VssCommConn.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

class VssSocketConn;


class VssSocketComm {
   public:
    VssSocketComm(VssMessageProcessor *messageProcessor);
    virtual ~VssSocketComm();

    void start();
    void stop();

   private:
    int mListenFd;
    std::unique_ptr<std::thread> mListenThread;
    std::vector<std::unique_ptr<VssSocketConn>> mOpenConnections;
    VssMessageProcessor *mMessageProcessor;
    std::mutex mMutex;

    bool listen();

    VssSocketConn* accept();

    void listenThread();

    void removeClosedConnections();
};

/**
 * VssSocketConn represents a single connection to a client.
 */
class VssSocketConn : public VssCommConn {
   public:
    VssSocketConn(VssMessageProcessor* messageProcessor, int sfd);
    virtual ~VssSocketConn() = default;

    std::string read() override; // TODO rename to readLine

    /**
     * Closes a connection if it is open.
     */
    void stop() override;

    inline bool isOpen() override { return mSockFd > 0; }

   private:
    int mSockFd;
};

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android


#endif  // android_hardware_automotive_vehicle_V2_0_impl_VssSocketComm_H_
