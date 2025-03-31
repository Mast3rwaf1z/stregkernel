#pragma once

#include <linux/compiler_types.h>
#include <net/net_namespace.h>

#define NAMESPACE "fklub"
#define DEVICE_NAME "stregkernel"
#define QUICKBUY_NAME "quickbuy"
#define BALANCE_NAME "balance"
#define CLASS_NAME "streg"
#define MAX_BUFFER 1024

static char multibuy_query[MAX_BUFFER] = {0};
