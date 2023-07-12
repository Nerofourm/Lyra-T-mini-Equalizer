# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += /home/espectro/esp/esp-adf/components/esp-adf-libs/esp_audio/include /home/espectro/esp/esp-adf/components/esp-adf-libs/esp_codec/include/codec /home/espectro/esp/esp-adf/components/esp-adf-libs/esp_codec/include/processing /home/espectro/esp/esp-adf/components/esp-adf-libs/recorder_engine/include /home/espectro/esp/esp-adf/components/esp-adf-libs/media_lib_sal/include /home/espectro/esp/esp-adf/components/esp-adf-libs/media_lib_sal/include/port /home/espectro/esp/esp-adf/components/esp-adf-libs/audio_misc/include /home/espectro/esp/esp-adf/components/esp-adf-libs/esp_muxer/include /home/espectro/esp/esp-adf/components/esp-adf-libs/esp_media_protocols/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/esp-adf-libs -lesp-adf-libs -L/home/espectro/esp/esp-adf/components/esp-adf-libs/esp_audio/lib/esp32 -L/home/espectro/esp/esp-adf/components/esp-adf-libs/esp_codec/lib/esp32 -L/home/espectro/esp/esp-adf/components/esp-adf-libs/recorder_engine/lib/esp32 -L/home/espectro/esp/esp-adf/components/esp-adf-libs/esp_media_protocols/lib/esp32 -L/home/espectro/esp/esp-adf/components/esp-adf-libs/esp_muxer/lib/esp32 -lesp_processing -lesp_audio -lesp_codec -lesp_muxer -lesp_media_protocols -lrecorder_engine 
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += esp-adf-libs
COMPONENT_LDFRAGMENTS += 
component-esp-adf-libs-build: 
