#include "oss_sdk.h"

void oss_sdk_append_object_from_buffer(const char *object_name, unsigned char *str, int str_len)
{
    aos_pool_t *p;
    aos_string_t bucket;
    aos_string_t object;
    aos_status_t *s;
    int is_oss_domain = 1;
    int64_t position = 0;
    aos_table_t *headers1;
    aos_table_t *headers2;
    aos_table_t *resp_headers;
    oss_request_options_t *options;
    aos_list_t buffer;
    aos_buf_t *content;
    char *next_append_position;
	
    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_test_request_options(options, is_oss_domain);
    headers1 = aos_table_make(p, 0);
    resp_headers = aos_table_make(p, 5);
    aos_str_set(&bucket, TEST_BUCKET_NAME);
	
    aos_str_set(&object, object_name);
	
    s = oss_head_object(options, &bucket, &object, headers1, &resp_headers);
	printf("1------ %d\n", s->code);
    if(s->code == 200) {
        next_append_position = (char*)(apr_table_get(resp_headers, "x-oss-next-append-position"));
        position = atoi(next_append_position); 
    }
	
    //append object
    headers2 = aos_table_make(p, 0);
    aos_list_init(&buffer);
    content = aos_buf_pack(p, str, str_len);
    aos_list_add_tail(&content->node, &buffer);
    s = oss_append_object_from_buffer(options, &bucket, &object, position, &buffer, headers2, &resp_headers);
	printf("2------ %d\n", s->code);
    assert(200 == s->code);
    printf("test_append_object_from_buffer ok\n");

    aos_pool_destroy(p);
}

void oss_sdk_init()
{
	if (aos_http_io_initialize("oss_test", 0) != AOSE_OK) {        
		exit(1);    
	}
}

void oss_sdk_destroy()
{
	aos_http_io_deinitialize();
}

void test()
{
	printf("hahh\n");
}







