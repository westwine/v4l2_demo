#include <linux/module.h>
#include <linux/videodev2.h>
#include <media/videobuf2-vmalloc.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-event.h>
#include <media/v4l2-common.h>

#include "../include/camera.h"
#include "../include/sensor.h"


//static camdev vcam
static cam camdev;
static struct v4l2_subdev sensor;

/****************************************************************************
*
* event functions
*
****************************************************************************/

static int event_add(struct v4l2_subscribed_event *sev, unsigned int elems) {
    printk("event added");
    return 0;
}
static void event_del(struct v4l2_subscribed_event *sev) {
    printk("event deleted");
}
static void event_replace(struct v4l2_event *old, const struct v4l2_event *new) {
    printk("event replaced");
}
static void event_merge(const struct v4l2_event *old, struct v4l2_event *new) {
    printk("event merged");
}

// static const struct v4l2_subscribed_event_ops *event_ops = {
//     .add = event_add,
//     .del = event_del,
//     .replace = event_replace,
//     .merge = event_merge,
// };


/****************************************************************************
*
* driver functions
*
****************************************************************************/



static int cam_querycap(struct file *file, void  *priv, struct v4l2_capability *vcap) {
    cam *cam = video_drvdata(file);
    /*
    fill
    struct v4l2_capability {
        __u8    driver[16];
        __u8    card[32];
        __u8    bus_info[32];
        __u32   version;
        __u32    capabilities;
        __u32    device_caps;
        __u32    reserved[3];
    };

    */
    strlcpy(vcap->driver, cam->vdev.name, sizeof(vcap->driver));
    strlcpy(vcap->card, "test Card", sizeof(vcap->card));
    strlcpy(vcap->bus_info, "test BUS", sizeof(vcap->bus_info));
    vcap->capabilities = V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_READWRITE; // report capabilities
    
    printk(KERN_INFO "[xxz_cam]%s, %d, \n", __func__, __LINE__);
    
    return 0;
}

static int cam_subscribe(struct v4l2_fh *fh, const struct v4l2_event_subscription *sub) {
    v4l2_event_subscribe(fh, sub, 0, NULL);
    return 0;
}

static int cam_unsubscribe(struct v4l2_fh *fh, const struct v4l2_event_subscription *sub) {
    v4l2_event_unsubscribe(fh, sub);
    return 0;
}


static const struct v4l2_file_operations cam_fops = {
    .owner          = THIS_MODULE,
    .open           = v4l2_fh_open,   // open /dev/video0 will enter here
    .release        = v4l2_fh_release,   // close fd will enter here    
    .unlocked_ioctl = video_ioctl2,  // need for ioctl

};

static const struct v4l2_ioctl_ops cam_ioctl_ops = {
    .vidioc_querycap = cam_querycap,  // ioctl VIDIOC_QUERYCAP will enter here
    .vidioc_subscribe_event = cam_subscribe,
    .vidioc_unsubscribe_event = cam_unsubscribe,

};



/****************************************************************************
*
* Video4Linux Module functions
*
****************************************************************************/


static int cam_init(void) {
    cam *cam;
    struct v4l2_device *v4l2_dev;
    int ret;
    
    cam = &camdev;
    v4l2_dev = &cam->v4l2_dev;
    //init v4l2 name, version
    strlcpy(v4l2_dev->name, "camv", sizeof(v4l2_dev->name));
    v4l2_info(v4l2_dev, "camera driver %s\n", "0.0.1"); // output V4l2 info
    ret = v4l2_device_register(NULL, v4l2_dev);
    if (ret < 0) {
        printk(KERN_INFO "Could not register v4l2_device\n");
        return ret;
    }
    
    //setup video device
    strlcpy(cam->vdev.name, "camera Driver", sizeof(cam->vdev.name));
    cam->vdev.v4l2_dev = v4l2_dev;   // set V4l2_device address to video_device
    cam->vdev.fops = &cam_fops;   //v4l2_file_operations
    cam->vdev.ioctl_ops = &cam_ioctl_ops;   //v4l2_ioctl_ops
    cam->vdev.release = video_device_release_empty;
    set_bit(V4L2_FL_USES_V4L2_FH, &cam->vdev.flags); //V4L2_FL_USE_FH_PRIO
    video_set_drvdata(&cam->vdev, cam);

    if (video_register_device(&cam->vdev, VFL_TYPE_GRABBER, -1) != 0) {
        /* return -1, -ENFILE(full) or others */
        printk(KERN_INFO "[xxz_cam]%s, %d, module inserted\n", __func__, __LINE__);
        ret = -ENODEV;
        goto out_dev;
    }    
    printk(KERN_INFO "[xxz_cam]%s, %d, module inserted\n", __func__, __LINE__);

    //subdev register
    sensor_register(&sensor, v4l2_dev);

    return 0;

out_dev:
    v4l2_device_unregister(&cam->v4l2_dev);
    video_unregister_device(&cam->vdev);  
    return ret;

}

static void cam_exit(void) {
    cam *cam;
    cam = &camdev;
    printk(KERN_INFO "[xxz_cam]%s, %d, module inserted\n", __func__, __LINE__);
    video_unregister_device(&cam->vdev);
    v4l2_device_unregister(&cam->v4l2_dev);
}

module_init(cam_init);
module_exit(cam_exit);

MODULE_DESCRIPTION("xxz camera");
MODULE_AUTHOR("XXZ");
MODULE_LICENSE("GPL");