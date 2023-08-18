#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>

#include "CamV4L2.h"
#include "../drivers/include/camera.h"

#define VIDEO_HEIGHT 480
#define VIDEO_WIDTH 640
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV
#define BUFFER_COUNT 1

static int xioctl(int fd, int request, void *arg)
{
    int r;

    do r = ioctl(fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}


CamV4L2::CamV4L2(std::string device){
    if(open_camera(device))
        return;

}

int CamV4L2::print_caps()
{
    struct v4l2_capability caps = {};
    int ret = xioctl(fd_, VIDIOC_QUERYCAP, &caps);
    if (ret == -1) {
        perror("Querying Capabilities");
        return 1;
    }
    // driver info
    printf( "Driver Caps:\n"
            "  Driver: \"%s\"\n"
            "  Card: \"%s\"\n"
            "  Bus: \"%s\"\n"
            "  Version: %d.%d\n"
            "  Capabilities: %08x\n",
            caps.driver,
            caps.card,
            caps.bus_info,
            (caps.version>>16)&&0xff,
            (caps.version>>24)&&0xff,
            caps.capabilities);

    
    return 0;
}

int CamV4L2::open_camera(std::string device){
    fd_ = open(device.c_str(), O_RDWR);
    if (fd_ == -1) {
        perror("Opening video device");
        //return 1;
    }
    printf("Sucessfuly open device %s.\n", device.c_str());
    return 0;

}

int CamV4L2::get_thermal() {
    degree_t arg;
    int ret = xioctl(_fd, CAMERA_GET_THERMAL, &arg);
    if (ret == -1) {
        perror("getting thermal data");
        return 1;
    }
    printf("get camera thermal data : %d"%arg.data);
    return 0;

}

void CamV4L2::close_camera(){
    close(fd_);
}

void CamV4L2::run(){
    if(print_caps())
        return;

    close_camera();

}

