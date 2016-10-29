SOURCE_DS = \
        src/deque.c \
        src/dictionary.c \
        src/exception.c
 	
SOURCE_LIB_SORTTEXT = \
        src/libtext_sort.c

SOURCE_CMD_DRIVER = \
        src/cmdmain.c \
        src/utils.c

SOURCE_UI_DRIVER = \
        src/uimain.c \
        src/utils.c 
        
OBJECT_DS = \
        deque.o \
        dictionary.o \
        exception.o
 	
OBJECT_LIB_SORTTEXT = \
        libtext_sort.o
        
all: cmd_static

cmd_static: libstatic
	gcc -o l4 $(SOURCE_CMD_DRIVER) libsorttext.a libds.a

cmd_shared: libshared
	gcc -o l4 $(SOURCE_CMD_DRIVER) libsorttext.so libds.so

ui_static: libstatic
	gcc -o l4 $(SOURCE_UI_DRIVER) libsorttext.a libds.a

ui_shared: libshared
	gcc -o l4 $(SOURCE_UI_DRIVER) libsorttext.so libds.so

libstatic:
	gcc -c $(SOURCE_DS) $(SOURCE_LIB_SORTTEXT) $(SOURCE_LIB_SORTTEXT)
	ar -rs libds.a $(OBJECT_DS)
	ar -rs libsorttext.a $(OBJECT_LIB_SORTTEXT)
	
libshared:
	gcc -fpic -shared -o libds.so $(SOURCE_DS)
	gcc -fpic -shared -o libsorttext.so $(SOURCE_LIB_SORTTEXT)
	
clean:
	rm -f *.o *.a *.so l4
