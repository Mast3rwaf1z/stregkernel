#include "lib/chrdev.h"
#include "lib/integrations/settings.h"
#include "lib/variables.h"

#include "lib/integrations.h"

chrdev_wrapper wrappers[] = {
    {
        .name = NAMESPACE "/quickbuy",
        .operations.write = quickbuy
    },
    { 
        .name = NAMESPACE "/flogo",
        .operations.read = flogo
    },
    {
        .name = NAMESPACE "/balance",
        .operations.read = balance
    },
    {
        .name = NAMESPACE "/history",
        .operations.read = history
    },
    {
        .name = NAMESPACE "/settings/address",
        .operations = {
            .write = set_address,
            .read = get_address
        }
    },
    {
        .name = NAMESPACE "/settings/domain",
        .operations = {
            .write = set_domain,
            .read = get_domain
        }
    },
    {
        .name = NAMESPACE "/settings/username",
        .operations = {
            .write = set_username,
            .read = get_username
        }
    },
    {
        .name = NAMESPACE "/settings/user_id",
        .operations = {
            .write = set_user_id,
            .read = get_user_id
        }
    },
    {
        .name = NAMESPACE "/settings/port",
        .operations = {
            .write = set_port,
            .read = get_port
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
