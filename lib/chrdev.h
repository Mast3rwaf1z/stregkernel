#include "variables.h"
#include "linux/fs.h"
#include "linux/kdev_t.h"
#include <linux/device.h>
#include <linux/device/class.h>

typedef struct {
    int major;
    struct class* chrdev_class;
    struct device* chrdev_device;
} chrdev_wrapper;

chrdev_wrapper* create_wrapper(chrdev_wrapper* wrapper, char* name, struct file_operations* fops) {
    wrapper->major = register_chrdev(0, name, fops);
    wrapper->chrdev_class = class_create(NAMESPACE);
    wrapper->chrdev_device = device_create(wrapper->chrdev_class, NULL, MKDEV(wrapper->major, 0), NULL, name);
    return wrapper;
}

void destroy_wrapper(chrdev_wrapper* wrapper, char* name) {
    device_destroy(wrapper->chrdev_class, MKDEV(wrapper->major, 0));
    class_unregister(wrapper->chrdev_class);
    class_destroy(wrapper->chrdev_class);
    unregister_chrdev(wrapper->major, name);
}
