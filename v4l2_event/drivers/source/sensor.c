#include <linux/videodev2.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-event.h>

#include "../include/sensor.h"

static int sensor_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh) {
    return 0;
}

static int sensor_close(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh) {
    return 0;
}

static struct v4l2_subdev_internal_ops sensor_internal_ops = {
	.open = sensor_open,
	.close = sensor_close,
};

void sensor_get_thermal(struct v4l2_subdev *sensor, degree_t *deg) {
    deg->data = 27;
}

static struct sensor_func *func_tbl = {
    .get_thermal = sensor_get_thermal,
};

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg) {
    switch (cmd) {
        case SENSOR_GET_THERMAL:
            func_tbl->get_thermal(sd, arg);
            break;
        default:
            printk("no operation conduct...");
    }

    return 0;
}

struct v4l2_subdev_core_ops sensor_core_ops = {
    .ioctl = sensor_ioctl,
};

static struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
};

int sensor_register(struct v4l2_subdev *sensor, struct v4l2_device *dev) {
    v4l2_subdev_init(sensor, &sensor_ops);
    sensor->internal_ops = &sensor_internal_ops;
    v4l2_device_register_subdev(dev, sensor);
    //func init
    //sensor->func = &func_tbl;
    return 0;
}
