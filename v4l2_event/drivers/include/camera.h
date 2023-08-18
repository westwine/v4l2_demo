#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <linux/videodev2.h>
#include <media/v4l2-device.h>

//cam structure include v4l2_device
typedef struct _cam {
    struct v4l2_device v4l2_dev;
    struct video_device vdev; // video_device contain v4l2_fh, no need to initialize
} cam;






#endif