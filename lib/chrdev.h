#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/device/class.h>

typedef struct {
    int major;
    char* name;
    struct class* chrdev_class;
    struct device* chrdev_device;
    struct file_operations operations;
} chrdev_wrapper;

chrdev_wrapper* create_wrapper(chrdev_wrapper* wrapper) {
    wrapper->major = register_chrdev(0, wrapper->name, &wrapper->operations);
    wrapper->chrdev_class = class_create(wrapper->name);
    wrapper->chrdev_device = device_create(wrapper->chrdev_class, NULL, MKDEV(wrapper->major, 0), NULL, wrapper->name);
    return wrapper;
}

void destroy_wrapper(chrdev_wrapper* wrapper) {
    device_destroy(wrapper->chrdev_class, MKDEV(wrapper->major, 0));
    class_unregister(wrapper->chrdev_class);
    class_destroy(wrapper->chrdev_class);
    unregister_chrdev(wrapper->major, wrapper->name);
}
