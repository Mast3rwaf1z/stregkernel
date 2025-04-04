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

chrdev_wrapper* create_wrapper(chrdev_wrapper* wrapper);

void destroy_wrapper(chrdev_wrapper* wrapper);
