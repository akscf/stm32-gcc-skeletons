TOOLCHAIN_PREFIX=arm-linux-gnueabi
CC=$(TOOLCHAIN_PREFIX)-gcc
AS=$(TOOLCHAIN_PREFIX)-as
LD=$(TOOLCHAIN_PREFIX)-ld
OC=$(TOOLCHAIN_PREFIX)-objcopy
OD=$(TOOLCHAIN_PREFIX)-objdump
SIZE=$(TOOLCHAIN_PREFIX)-size

# ------------------------------------------------------------------------------------------------------
STM32_LIBDIR=./libs/stm32f10x
USR_LIBDIR=./libs
INCLUDE_DIR=./include
SRC_DIR=./src
TARGET_DIR=./target
TARGET_NAME=firmware
TARGET_FQN=$(TARGET_DIR)/$(TARGET_NAME)

# ------------------------------------------------------------------------------------------------------
SPL=
#SPL+=misc
SPL+=stm32f10x_rcc
SPL+=stm32f10x_gpio
SPL+=stm32f10x_tim

# ------------------------------------------------------------------------------------------------------
CFLAGS=-c -mcpu=cortex-m3 -mthumb -mapcs-frame
#-Os
CFLAGS+=-std=c99 -msoft-float 
CFLAGS+=-mno-sched-prolog -fno-hosted -mtune=cortex-m3
CFLAGS+=-march=armv7-m -mfix-cortex-m3-ldrd -ffunction-sections -fdata-sections

CFLAGS+=-Wall -Werror
CFLAGS+=-I$(STM32_LIBDIR)/cmsis -I$(STM32_LIBDIR)/spl -I$(INCLUDE_DIR)
CFLAGS+=-DSTM32F10X_MD  -DUSE_STDPERIPH_DRIVER 

LDFLAGS=-static -mcpu=cortex-m3 -mthumb -mthumb-interwork
LDFLAGS+=-nostartfiles -nodefaultlibs -nostdlib
#LDFLAGS+=-Xlinker -Map=$(TARGET_DIR)/$(TARGET_NAME).map -Xlinker --gc-sections -s
LDFLAGS+=-Xlinker --gc-sections -s
LDFLAGS+=-T $(STM32_LIBDIR)/stm32f103c8t8.ld -o $(TARGET_FQN)

ASFLAGS=-mcpu=cortex-m3 -I$(STM32_LIBDIR)/cmsis -I$(STM32_LIBDIR)/spl -I$(INCLUDE_DIR) -gdwarf-2

CPFLAGS=-O binary 
ODFLAGS=-S
# ------------------------------------------------------------------------------------------------------
INDEPENDENT_HEADERS=

SRC=$(wildcard ${SRC_DIR}/*.c)
MAIN_OBJ=$(addprefix $(TARGET_DIR)/, $(SRC:$(SRC_DIR)/%.c=%.o))

STM32_LIBSRC=$(addprefix $(STM32_LIBDIR)/cmsis, $(addsuffix .c, $(SPL)))
STM32_LIBSRC+=$(STM32_LIBDIR)/cmsis/system_stm32f10x.c

OBJ=$(TARGET_DIR)/startup.o $(TARGET_DIR)/system_stm32f10x.o
OBJ+=$(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(SPL)))

ALL_OBJ=$(OBJ) $(MAIN_OBJ)
ALL_SRC=$(SRC) $(STM32_LIBSRC)

# ------------------------------------------------------------------------------------------------------

all: $(TARGET_NAME)

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TARGET_DIR)/%.o: $(STM32_LIBDIR)/spl/%.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/system_stm32f10x.o: $(STM32_LIBDIR)/cmsis/system_stm32f10x.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/startup.o: $(STM32_LIBDIR)/cmsis/startup_stm32f10x_md.s
	$(AS) $(ASFLAGS) $(STM32_LIBDIR)/cmsis/startup_stm32f10x_md.s -o $(TARGET_DIR)/startup.o

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(ALL_SRC) : %.c : %.h $(INDEPENDENT_HEADERS)
	@touch $@

%.h: ;

$(TARGET_NAME): $(TARGET_DIR) $(ALL_OBJ)
	$(CC) $(ALL_OBJ) $(LDFLAGS)
	$(OC) $(CPFLAGS) $(TARGET_FQN) $(TARGET_FQN).bin
#	$(OD) $(ODFLAGS) $(TARGET_FQN) > $(TARGET_FQN).list
	$(SIZE) -B $(TARGET_FQN)

install: $(TARGET_FQN)
	st-flash write $(TARGET_FQN).bin 0x8000000

clean:
	rm -f $(TARGET_DIR)/*
	rmdir $(TARGET_DIR)
