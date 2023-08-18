[TOC]

# v4l2 driver implementation and event mechanism opitimization

## TODO

1. implement a minimal v4l2 driver system
2. log event queue when queue is full
3. design a priority queue to store event messages in order
4. flow control based on free buffers in queue

## overal framework

v4l2 driver : setup of device instances and connect to the sub-devices

[data struct of v4l2](https://www.processon.com/view/link/6175161563768912b56334e0)

The framework closely resembles the driver structure: it has a v4l2_device struct for the device instance data, a v4l2_subdev struct to refer to sub-device instances, the video_device struct stores V4L2 device node data and the v4l2_fh struct keeps track of filehandle instances.

driver : prepare data and return state

user : open device and start requesting data

### driver structure

1. A struct for each device instance containing the device state.
2. A way of initializing and commanding sub-devices (if any).
3. Creating V4L2 device nodes (/dev/videoX, /dev/vbiX and /dev/radioX) and keeping track of device-node specific data.
4. Filehandle-specific structs containing per-filehandle data;
5. video buffer handling.



## steps

/lib/modules/4.15.0-156-generic/kernel/drivers/media/v4l2-core

加载videodev。。 sudo modprobe videodev

之后加载驱动，注册video设备





## new TODO

1. event_ops 添加修改

   cam_cfgdev.c  line 523

   打印队列溢出时的队列信息；把队列溢出作为关键事件插入队列，优先进行通知

2. 更改event机制中队列的优先级，实现优先级队列

3. 对队列进行拥塞控制

