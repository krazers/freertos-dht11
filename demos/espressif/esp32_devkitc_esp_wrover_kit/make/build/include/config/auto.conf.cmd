deps_config := \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/app_trace/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/esp32/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/fatfs/Kconfig \
	/Users/azer/esp/amazon-freertos-master/demos/espressif/esp32_devkitc_esp_wrover_kit/make/../../../../demos/espressif/esp32_devkitc_esp_wrover_kit/common/application_code/espressif_code/freertos/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/heap/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/log/Kconfig \
	/Users/azer/esp/amazon-freertos-master/demos/espressif/esp32_devkitc_esp_wrover_kit/make/../../../../demos/espressif/esp32_devkitc_esp_wrover_kit/common/application_code/espressif_code/mbedtls/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/openssl/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/pthread/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/spi_flash/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/spiffs/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/wear_levelling/Kconfig \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/azer/esp/amazon-freertos-master/lib/third_party/mcu_vendor/espressif/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
