TARGET = main
ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
STARTUP = startup_stm32f401rctx
LOADER = STM32F401RCTX_FLASH.ld
OBJCOPY = arm-none-eabi-objcopy
CC = arm-none-eabi-gcc

OUTPUT = Build
SOURCE_DIRS = Core Periph App Drivers
INCLUDES = $(SOURCE_DIRS) Drivers/CMSIS/Include
SRC := $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.c))
OBJ := $(addprefix $(OUTPUT)/,$(notdir $(SRC:.c=.o)))

STARTUP_OBJ = $(OUTPUT)/$(STARTUP).o

CFLAGS = $(ARCH_FLAGS) -g3 --specs=nano.specs -Wall \
 $(addprefix -I,$(INCLUDES)) -DSTM32F401xC -std=gnu11

.PHONY: all
all: $(OUTPUT)/$(TARGET).hex

$(foreach src,$(SRC),$(eval $(OUTPUT)/$(notdir $(src:.c=.o)): $(src)))

$(OBJ): Makefile
	$(CC) $(CFLAGS) -MMD -MP -c -O0 -ffunction-sections -fdata-sections -fstack-usage -o $@ $(filter %.c,$^)

$(STARTUP_OBJ): $(STARTUP).s Makefile
	$(CC) $(CFLAGS) -c -x assembler-with-cpp -o $@ $<

$(OUTPUT)/$(TARGET).elf: $(OBJ) $(STARTUP_OBJ) $(LOADER) Makefile
	$(CC) -o $@ $(OBJ) $(STARTUP_OBJ) $(ARCH_FLAGS) \
	--specs=nosys.specs -T"$(LOADER)" -Wl,-Map=$(OUTPUT)/$(TARGET).map \
	-Wl,--gc-sections -static \
	-Wl,--start-group -lc -lm -Wl,--end-group

	@/bin/echo -e "\033[1;36m"
	arm-none-eabi-size $@
	@/bin/echo -e "\033[0m"

$(OUTPUT)/$(TARGET).hex: $(OUTPUT)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

-include $(OBJ:.o=.d)
