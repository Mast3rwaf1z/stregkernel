#include "lib/chrdev.h"
#include "lib/integrations/settings.h"
#include "lib/variables.h"

#include "lib/integrations.h"

chrdev_wrapper wrappers[] = {
    {
        .name = NAMESPACE "quickbuy",
        .operations.write = quickbuy
    },
    { 
        .name = NAMESPACE "flogo",
        .operations.read = flogo
    },
    {
        .name = NAMESPACE "balance",
        .operations.read = balance
    },
    {
        .name = NAMESPACE "history",
        .operations.read = history
    },
    {
        .name = NAMESPACE "products",
        .operations.read = products
    },
    {
        .name = SETTING "address",
        .operations = {
            .write = set_address,
            .read = get_address
        }
    },
    {
        .name = SETTING "domain",
        .operations = {
            .write = set_domain,
            .read = get_domain
        }
    },
    {
        .name = SETTING "username",
        .operations = {
            .write = set_username,
            .read = get_username
        }
    },
    {
        .name = SETTING "member_id",
        .operations = {
            .write = set_member_id,
            .read = get_member_id
        }
    },
    {
        .name = SETTING "port",
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
    pr_info(PRINT_FMT "stregkernel unloaded\n");
}


module_init(stregkernel_init);
module_exit(stregkernel_exit);
MODULE_LICENSE("GPL");
