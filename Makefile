TARGET    = main
MCPU      = cortex-m4
STARTUP   = startup_stm32f401rctx
LOADER    = STM32F401RCTX_FLASH.ld
OBJCOPY   = arm-none-eabi-objcopy
CC        = arm-none-eabi-gcc

INCLUDE1  = Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCLUDE2  = Drivers/CMSIS/Include
ST_INCL   = Core/Startup
INC_H     = inc

SRC       := $(wildcard *.c)
OBJ       := $(patsubst %.c,build/%.o,$(SRC))

SYS_SRC   = Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
SYS_OBJ   = build/system_stm32f4xx.o

CFLAGS = -mcpu=$(MCPU) -g3 --specs=nano.specs -mthumb -mfloat-abi=soft -Wall \
 -I$(INC_H) -I$(INCLUDE1) -I$(INCLUDE2) -DSTM32F401xC -std=gnu11

all: build/$(TARGET).hex

build:
	mkdir -p build

build/%.o: %.c | build
	$(CC) $(CFLAGS) -c -O0 -ffunction-sections -fdata-sections -fstack-usage -o $@ $<

# system_stm32f4xx.c 
$(SYS_OBJ): $(SYS_SRC)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(STARTUP).o: $(ST_INCL)/$(STARTUP).s | build
	$(CC) $(CFLAGS) -c -x assembler-with-cpp -o $@ $<

build/$(TARGET).elf: $(OBJ) $(SYS_OBJ) $(STARTUP).o $(LOADER)
	$(CC) -o $@ $(OBJ) $(SYS_OBJ) $(STARTUP).o -mcpu=$(MCPU) \
	--specs=nosys.specs -T"$(LOADER)" -Wl,-Map=build/$(TARGET).map \
	-Wl,--gc-sections -static -mfloat-abi=soft -mthumb \
	-Wl,--start-group -lc -lm -Wl,--end-group
	arm-none-eabi-size $@

build/$(TARGET).hex: build/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
