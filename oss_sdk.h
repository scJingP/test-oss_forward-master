#include <stdio.h>
#include <stdlib.h>
#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include "oss_config.h"
#include "oss_test_util.h"

void oss_sdk_append_object_from_buffer(const char *object_name, unsigned char *str, int str_len);
void oss_sdk_init();
void oss_sdk_destroy();
void test();



