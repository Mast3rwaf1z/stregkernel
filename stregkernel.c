#include "lib/chrdev.h"
#include "lib/variables.h"

#include "lib/integrations.h"

chrdev_wrapper wrappers[] = {
    {
        .name = NAMESPACE "/quickbuy",
        .operations = {
            .write = quickbuy_w
        }
    },
    { 
        .name = NAMESPACE "/flogo",
        .operations = {
            .read = flogo_r
        }
    },
    {
        .name = NAMESPACE "/settings/address",
        .operations = {
            .write = set_address
        }
    },
    {
        .name = NAMESPACE "/settings/port",
        .operations = {
            .write = set_port
        }
    }
};

static int stregkernel_init(void) {
    for(int i = 0; i < sizeof(wrappers)/sizeof(chrdev_wrapper); i++) {
        create_wrapper(&wrappers[i]);
        pr_info(PRINT_FMT "%s loaded with major %d\n", wrappers[i].name, wrappers[i].major);
    }
    return 0;
}

static void stregkernel_exit(void) {
    for (int i = 0; i < sizeof(wrappers)/sizeof(chrdev_wrapper); i++) {
        destroy_wrapper(&wrappers[i]);
    }
    pr_info("stregkernel unloaded\n");
}


module_init(stregkernel_init);
module_exit(stregkernel_exit);
MODULE_LICENSE("GPL");
