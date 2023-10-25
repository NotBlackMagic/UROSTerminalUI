#include "micro_ros_transport.h"

//https://micro.ros.org/docs/tutorials/advanced/create_custom_transports/

//int clock_gettime(clockid_t unused, struct timespec *tp);
//bool serial_transport_open(struct uxrCustomTransport * transport);
//bool serial_transport_close(struct uxrCustomTransport * transport);
//size_t serial_transport_write(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, uint8_t *err);
//size_t serial_transport_read(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, int timeout, uint8_t *err);

static int sem_initialized = 0;
static struct rt_semaphore rx_sem;
static rt_device_t micro_ros_serial;

#ifndef MICRO_ROS_SERIAL_NAME
    #define MICRO_ROS_SERIAL_NAME "uart4"
#endif

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

bool serial_transport_open(struct uxrCustomTransport* transport) {
    micro_ros_serial = rt_device_find(MICRO_ROS_SERIAL_NAME);
    if (!micro_ros_serial) {
//        LOG_E("Failed to open device %s", MICRO_ROS_SERIAL_NAME);
        return 0;
    }
    if(sem_initialized == 0) {
        rt_sem_init(&rx_sem, "micro_ros_rx_sem", 0, RT_IPC_FLAG_FIFO);
        sem_initialized = 1;
    }
    rt_device_open(micro_ros_serial, RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(micro_ros_serial, uart_input);
    return 1;
}

bool serial_transport_close(struct uxrCustomTransport* transport) {
    rt_device_close(micro_ros_serial);
    rt_sem_detach(&rx_sem);
    sem_initialized = 0;
    return 1;
}

size_t serial_transport_write(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, uint8_t* err) {
    //Very important to wait until written AT LEAST 1 byte, else is considered a failure and will not work...
    uint8_t written = rt_device_write(micro_ros_serial, 0, buf, len);
    while(written == 0) {
        written = rt_device_write(micro_ros_serial, 0, buf, len);
    }
    return written;
}

size_t serial_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err) {
//    int readCount = 0;
//    int tick = rt_tick_get();
//    while((tick + timeout) > rt_tick_get()) {
//        readCount += rt_device_read(micro_ros_serial, 0, &buf[readCount], (len - readCount));
//        if(readCount == len) {
//            break;
//        }
//        else if(readCount > len) {
//            //ERROR: This can't be allowed to happen!
//            return 0;
//        }
//    }
//    return readCount;
    int tick = rt_tick_get();
    for (int i = 0; i < len; i++) {
        if(sem_initialized == 0) {
            rt_sem_init(&rx_sem, "micro_ros_rx_sem", 0, RT_IPC_FLAG_FIFO);
            sem_initialized = 1;
        }
        while (rt_device_read(micro_ros_serial, -1, &buf[i], 1) != 1) {
            rt_sem_take(&rx_sem, timeout / 4);
            if( (rt_tick_get() - tick) > timeout) {
                //EROOR: Read timeout
                return 0;
            }
        }
    }
    return len;
}
