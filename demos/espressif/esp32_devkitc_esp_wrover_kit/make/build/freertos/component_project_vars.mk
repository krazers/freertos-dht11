# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /Users/azer/esp/amazon-freertos-master/demos/espressif/esp32_devkitc_esp_wrover_kit/common/application_code/espressif_code/freertos/include /Users/azer/esp/amazon-freertos-master/demos/espressif/esp32_devkitc_esp_wrover_kit/common/config_files /Users/azer/esp/amazon-freertos-master/lib/include /Users/azer/esp/amazon-freertos-master/lib/include/private /Users/azer/esp/amazon-freertos-master/lib/FreeRTOS/portable/ThirdParty/GCC/Xtensa_ESP32/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/freertos -lfreertos -Wl,--undefined=uxTopUsedPriority
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += freertos
component-freertos-build: 
