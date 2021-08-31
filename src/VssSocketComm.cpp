#include "inc/VssSocketComm.h"

#define LOG_TAG "VssSocketComm"

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <android/log.h>
#include <arpa/inet.h>
#include <log/log.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Socket to use when communicating with Host PC
static constexpr int DEBUG_SOCKET = 44452;

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

VssSocketComm::VssSocketComm(VssMessageProcessor* messageProcessor)
    : mListenFd(-1), mMessageProcessor(messageProcessor) {}

VssSocketComm::~VssSocketComm() {
}

void VssSocketComm::start() {
    if (!listen()) {
        return;
    }

    mListenThread = std::make_unique<std::thread>(std::bind(&VssSocketComm::listenThread, this));
}

void VssSocketComm::stop() {
    if (mListenFd > 0) {
        ::close(mListenFd);
        if (mListenThread->joinable()) {
            mListenThread->join();
        }
        mListenFd = -1;
    }
}

bool VssSocketComm::listen() {
    int retVal;
    struct sockaddr_in servAddr;

    mListenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mListenFd < 0) {
        ALOGE("%s: socket() failed, mSockFd=%d, errno=%d", __FUNCTION__, mListenFd, errno);
        mListenFd = -1;
        return false;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(DEBUG_SOCKET);

    retVal = bind(mListenFd, reinterpret_cast<struct sockaddr*>(&servAddr), sizeof(servAddr));
    if(retVal < 0) {
        ALOGE("%s: Error on binding: retVal=%d, errno=%d", __FUNCTION__, retVal, errno);
        close(mListenFd);
        mListenFd = -1;
        return false;
    }

    ALOGI("%s: Listening for connections on port %d", __FUNCTION__, DEBUG_SOCKET);
    if (::listen(mListenFd, 1) == -1) {
        ALOGE("%s: Error on listening: errno: %d: %s", __FUNCTION__, errno, strerror(errno));
        return false;
    }
    return true;
}

VssSocketConn* VssSocketComm::accept() {
    sockaddr_in cliAddr;
    socklen_t cliLen = sizeof(cliAddr);
    int sfd = ::accept(mListenFd, reinterpret_cast<struct sockaddr*>(&cliAddr), &cliLen);

    if (sfd > 0) {
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliAddr.sin_addr, addr, INET_ADDRSTRLEN);

        ALOGD("%s: Incoming connection received from %s:%d", __FUNCTION__, addr, cliAddr.sin_port);
        return new VssSocketConn(mMessageProcessor, sfd);
    }

    return nullptr;
}

void VssSocketComm::listenThread() {
    while (true) {
        VssSocketConn* conn = accept();
        if (conn == nullptr) {
            return;
        }

        conn->start();
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mOpenConnections.push_back(std::unique_ptr<VssSocketConn>(conn));
        }
    }
}

/**
 * Called occasionally to clean up connections that have been closed.
 */
void VssSocketComm::removeClosedConnections() {
    std::lock_guard<std::mutex> lock(mMutex);
    std::remove_if(mOpenConnections.begin(), mOpenConnections.end(),
                   [](std::unique_ptr<VssSocketConn> const& c) { return !c->isOpen(); });
}

VssSocketConn::VssSocketConn(VssMessageProcessor* messageProcessor, int sfd)
    : VssCommConn(messageProcessor), mSockFd(sfd) {}

std::string VssSocketConn::read() {
    std::string buffer;
    char c;
    while (true) {
        int numRead = ::read(mSockFd, &c, 1);
        if (numRead <= 0 || c == '\n') {
            break;
        } else {
            buffer += c;
        }
    }

    ALOGD("Buffer size that was read: %d", (int) buffer.size());
    ALOGD("Buffer that was read: %s", buffer.c_str());
    return buffer;
}

void VssSocketConn::stop() {
    if (mSockFd > 0) {
        close(mSockFd);
        mSockFd = -1;
    }
}

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
