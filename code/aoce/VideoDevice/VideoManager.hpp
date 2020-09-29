#pragma once
#include "../Aoce.hpp"
#include "VideoDevice.hpp"
namespace aoce {

typedef std::shared_ptr<VideoDevice> VideoDevicePtr;

class ACOE_EXPORT VideoManager {
   protected:
    /* data */
    std::vector<VideoDevicePtr> videoList;

   public:
    VideoManager(/* args */);
    virtual ~VideoManager();

   protected:
    virtual void getDevices(){};

   public:
    const std::vector<VideoDevicePtr>& getDeviceList();
};
}  // namespace aoce