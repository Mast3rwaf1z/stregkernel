#pragma once

#include <linux/compiler_types.h>
#include <net/net_namespace.h>

#define DEVICE_NAME "stregsystem"
#define CLASS_NAME "streg"
#define MAX_BUFFER 1024

static int major_number;
static struct class* streg_class = NULL;
static struct device* streg_device = NULL;
static char multibuy_query[MAX_BUFFER] = {0};
