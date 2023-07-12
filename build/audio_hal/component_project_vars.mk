# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /home/espectro/esp/esp-adf/components/audio_hal/include /home/espectro/esp/esp-adf/components/audio_hal/driver/include /home/espectro/esp/esp-adf/components/audio_hal/driver/es8388 /home/espectro/esp/esp-adf/components/audio_hal/driver/es8374 /home/espectro/esp/esp-adf/components/audio_hal/driver/es8311 /home/espectro/esp/esp-adf/components/audio_hal/driver/es7243 /home/espectro/esp/esp-adf/components/audio_hal/driver/zl38063 /home/espectro/esp/esp-adf/components/audio_hal/driver/zl38063/api_lib /home/espectro/esp/esp-adf/components/audio_hal/driver/zl38063/example_apps /home/espectro/esp/esp-adf/components/audio_hal/driver/zl38063/firmware /home/espectro/esp/esp-adf/components/audio_hal/driver/tas5805m /home/espectro/esp/esp-adf/components/audio_hal/driver/es7148 /home/espectro/esp/esp-adf/components/audio_hal/driver/es7210 /home/espectro/esp/esp-adf/components/audio_hal/driver/es8156 /home/espectro/esp/esp-adf/components/audio_hal/driver/es7243e
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/audio_hal -laudio_hal -L/home/espectro/esp/esp-adf/components/audio_hal/driver/zl38063/firmware -lfirmware
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += audio_hal
COMPONENT_LDFRAGMENTS += 
component-audio_hal-build: 
