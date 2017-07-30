MAKE_CONFIG=../../config.mk
include $(MAKE_CONFIG)

TARGET=hello
OBJS=$(patsubst %.c, %.c.o, $(wildcard *.c)) \
	 $(patsubst %.s, %.s.o, $(wildcard *.s))

$(TARGET) : $(OBJS)
	@echo \ - LD $@
	@$(K_LD) $(K_LDFLAGS) -o $@ $^

%.c.o : %.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -Iinclude -o $@ -c $<

%.s.o : %.s
	@echo \ - AS $@
	@$(K_AS) $(K_ASFLAGS) -o $@ $<

## CLEAN

CLEAN_OBJS=$(OBJS:%=clean-%) $(TARGET:%=clean-%)

.PHONY : clean $(CLEAN_OBJS)

clean : $(CLEAN_OBJS)

$(CLEAN_OBJS) :
	@echo \ - RM $(@:clean-%=%)
	@$(RM) $(@:clean-%=%)
