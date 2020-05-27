SOURCE_FILES=\
    oss_test_util.h\
    oss_config.h\
    oss_test_util.c\
    oss_config.c\
    test_oss_object.c\

INCLUDES=\
	-I.\
	-I./oss_c_sdk_include\
    -I/usr/local/include\
	-I/usr/local/libcurl/include\
    -I/usr/local/include/libxml2\
    -I/usr/local/apr/include/apr-1\
    -I/usr/local/apr-util/include/apr-1

CFLAGS=\
    -D_LARGEFILE64_SOURCE\
    -fPIC\
	-g

LIBS_PATH=\
	-L. -L../\
    -L/usr/local/lib\
	-L/usr/local/libcurl/lib\
    -L/usr/local/apr/lib\
    -L/usr/local/apr-util/lib

LIBS=\
	-lipc\
	-loss_c_sdk\
    -lxml2\
    -laprutil-1\
    -lapr-1\
    -lcurl\
    -lrt\
    -lm\
	-lpthread
	
main : main.o oss_sdk.o oss_test_util.o oss_config.o ConfigFile.o
	g++ main.o oss_sdk.o oss_test_util.o oss_config.o ConfigFile.o -o main $(CFLAGS) $(INCLUDES) $(LIBS_PATH) $(LIBS)

oss_config.o : oss_config.c oss_config.h
	gcc -c oss_config.c $(CFLAGS) $(INCLUDES)

oss_test_util.o : oss_test_util.c oss_test_util.h
	gcc -c oss_test_util.c $(CFLAGS) $(INCLUDES)

oss_sdk.o : oss_sdk.c oss_sdk.h oss_test_util.h oss_config.h
	gcc -c oss_sdk.c $(CFLAGS) $(INCLUDES)

main.o : main.cpp oss_sdk.h wp_sdk.h
	gcc -c main.cpp $(CFLAGS) $(INCLUDES)

ConfigFile.o : ConfigFile.cpp ConfigFile.h
	gcc -c ConfigFile.cpp $(CFLAGS) $(INCLUDES)

.PHONY : clean
clean:
	rm -f main
	rm -f *.o
	


