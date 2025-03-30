#pragma once

#include <linux/compiler_types.h>
#include <net/net_namespace.h>

#define DEVICE_NAME "stregsystem"
#define QUICKBUY_NAME "quickbuy"
#define BALANCE_NAME "balance"
#define CLASS_NAME "streg"
#define MAX_BUFFER 1024

static int quickbuy_major_number;
static int balance_major_number;
static struct class* streg_class = NULL;
static struct device* quickbuy_device = NULL;
static struct device* balance_device = NULL;
static char multibuy_query[MAX_BUFFER] = {0};
