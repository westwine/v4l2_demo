#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <media/v4l2-subdev.h>
#include <media/v4l2-device.h>
#include <linux/videodev2.h>


extern int sensor_register(struct v4l2_subdev *sensor, struct v4l2_device *dev);




typedef struct _degree_t {
    int data; //thermal value in int
} degree_t;

struct sensor_func {
    void (*get_thermal)(struct v4l2_subdev *, degree_t *);
};


#define SENSOR_GET_THERMAL _IOR('A', BASE_VIDIOC_PRIVATE + 1, degree_t)




#endif