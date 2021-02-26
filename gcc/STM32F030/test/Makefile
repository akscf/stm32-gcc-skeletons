TOOLCHAIN_PREFIX=arm-linux-gnueabi
CC=$(TOOLCHAIN_PREFIX)-gcc
AS=$(TOOLCHAIN_PREFIX)-as
LD=$(TOOLCHAIN_PREFIX)-ld
OC=$(TOOLCHAIN_PREFIX)-objcopy
OD=$(TOOLCHAIN_PREFIX)-objdump
SIZE=$(TOOLCHAIN_PREFIX)-size

# ------------------------------------------------------------------------------------------------------
STM32_LIBDIR=./libs/stm32f0xx
USR_LIBDIR=./libs
INCLUDE_DIR=./include
SRC_DIR=./src
TARGET_DIR=./target
TARGET_NAME=firmware
TARGET_FQN=$(TARGET_DIR)/$(TARGET_NAME)

# ------------------------------------------------------------------------------------------------------
SPL=
#SPL+=misc
SPL+=stm32f0xx_rcc
SPL+=stm32f0xx_gpio
SPL+=stm32f0xx_tim

# ------------------------------------------------------------------------------------------------------
CFLAGS=-c -mcpu=cortex-m0 -mthumb -mapcs-frame -Os
#CFLAGS+=-D__thumb2__=1
#CFLAGS+=-gdwarf-2
CFLAGS+=-std=c99 -msoft-float 
CFLAGS+=-mno-sched-prolog -fno-hosted -mtune=cortex-m0
CFLAGS+=-march=armv6-m -ffunction-sections -fdata-sections
CFLAGS+=-Wall -Werror
CFLAGS+=-I$(STM32_LIBDIR)/cmsis -I$(STM32_LIBDIR)/spl -I$(INCLUDE_DIR)
CFLAGS+=-DUSE_STDPERIPH_DRIVER -DSTM32F0XX_MD

LDFLAGS=-static -mcpu=cortex-m0 -mthumb -mthumb-interwork
LDFLAGS+=-nostartfiles -nodefaultlibs -nostdlib
LDFLAGS+=-Xlinker --gc-sections -s
LDFLAGS+=-T $(STM32_LIBDIR)/stm32f030.ld -o $(TARGET_FQN)

ASFLAGS=-mcpu=cortex-m0 -I$(STM32_LIBDIR)/cmsis -I$(STM32_LIBDIR)/spl -I$(INCLUDE_DIR) -gdwarf-2

CPFLAGS=-O binary 
ODFLAGS=-S
# ------------------------------------------------------------------------------------------------------
INDEPENDENT_HEADERS=

SRC=$(wildcard ${SRC_DIR}/*.c)
MAIN_OBJ=$(addprefix $(TARGET_DIR)/, $(SRC:$(SRC_DIR)/%.c=%.o))

STM32_LIBSRC=$(addprefix $(STM32_LIBDIR)/cmsis, $(addsuffix .c, $(SPL)))
STM32_LIBSRC+=$(STM32_LIBDIR)/cmsis/system_stm32f0xx.c

OBJ=$(TARGET_DIR)/startup.o $(TARGET_DIR)/system_stm32f0xx.o
OBJ+=$(addprefix $(TARGET_DIR)/, $(addsuffix .o, $(SPL)))

ALL_OBJ=$(OBJ) $(MAIN_OBJ)
ALL_SRC=$(SRC) $(STM32_LIBSRC)

# ------------------------------------------------------------------------------------------------------

all: $(TARGET_NAME)

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TARGET_DIR)/%.o: $(STM32_LIBDIR)/spl/%.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/system_stm32f0xx.o: $(STM32_LIBDIR)/cmsis/system_stm32f0xx.c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET_DIR)/startup.o: $(STM32_LIBDIR)/cmsis/startup_stm32f0xx.s
	$(AS) $(ASFLAGS) $(STM32_LIBDIR)/cmsis/startup_stm32f0xx.s -o $(TARGET_DIR)/startup.o

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
	st-flash write $(TARGET_FQN).bin 0x08000000


clean:
	rm -f $(TARGET_DIR)/*.o $(TARGET_DIR)/*.bin $(TARGET_DIR)/*.list $(TARGET_FQN)
	rmdir $(TARGET_DIR)
