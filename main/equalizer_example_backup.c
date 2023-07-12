/* Play 'wav' file processed by an equalizer

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_common.h"
#include "fatfs_stream.h"
#include "i2s_stream.h"
#include "wav_decoder.h"
#include "mp3_decoder.h"
#include "wav_encoder.h"
#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "board.h"
#include "equalizer.h"

static const char *TAG = "EQUALIZER_EXAMPLE";

void app_main(void)
{
    audio_pipeline_handle_t pipeline;
    audio_element_handle_t fatfs_stream_reader, i2s_stream_writer, wav_decoder, equalizer;

    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set(TAG, ESP_LOG_INFO);

    ESP_LOGI(TAG, "[1.0] Mount sdcard");
    // Initialize peripherals management
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();

    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

    // Initialize SD Card peripheral
    audio_board_sdcard_init(set, SD_MODE_1_LINE);

    ESP_LOGI(TAG, "[2.0] Start codec chip");
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);

    /**
     * 
     * 
     * 
     * 
    */

    ESP_LOGI(TAG, "[3.0] Create audio pipeline");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(pipeline);

    /**
    #define DEFAULT_AUDIO_PIPELINE_CONFIG() {\ 
    .rb_size = DEFAULT_PIPELINE_RINGBUF_SIZE,\ // (8*1024) 
    } 


    audio_pipeline_init:
    El objeto audio_pipeline_handle_t se encarga de controlar el flujo de datos de audio y de conectar los elementos de audio con el ringbuffer. Se conectará y iniciará los elementos de audio en orden, siendo responsable de recuperar los datos del elemento anterior y pasarlos al elemento posterior. También se encargará de recibir eventos de cada elemento, procesarlos o pasarlos a una capa superior.
     * 
    */


    fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
    fatfs_cfg.type = AUDIO_STREAM_READER;
    fatfs_stream_reader = fatfs_stream_init(&fatfs_cfg); // es un elemento de la pipeline
    /**
    FATFS_Default:
    .type = AUDIO_STREAM_NONE,                   \
    .buf_sz = 4096,             \
    .out_rb_size = 8*1024, \
    .task_stack = 3072,       \
    .task_core = 0,         \
    .task_prio = 4,         \
    .ext_stack = false,                          \
    .write_header = true,                        \

    fatfs_stream_init:
    Crea un identificador (handle) para un Elemento de Audio para transmitir datos desde FatFs a otro Elemento o para obtener datos de otros elementos escritos en FatFs, según la configuración del tipo de transmisión, ya sea AUDIO_STREAM_READER o AUDIO_STREAM_WRITER.

    */

    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg); // Crea un identificador (handle) para un Elemento de Audio para transmitir datos desde I2S a otro Elemento o para obtener datos de otros elementos enviados a I2S, según la configuración del tipo de transmisión, ya sea AUDIO_STREAM_READER o AUDIO_STREAM_WRITER.

    /**
    * I2S_default: 
            .type = AUDIO_STREAM_WRITER,                                                \
    .i2s_config = {                                                             \
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),      \
        .sample_rate = 44100,                                                   \
        .bits_per_sample = 16,                           \
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           \
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,                      \
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,          \
        .dma_buf_count = 3,                                                     \
        .dma_buf_len = 300,                                                     \
        .use_apll = true,                                                       \
        .tx_desc_auto_clear = true,                                             \
        .fixed_mclk = 0                                                         \
    },                                                                          \
    .i2s_port = I2S_NUM_0,                                                      \
    .use_alc = false,     NIVEL AUTOMÁTICO DE GANANCIA                          \
    .volume = 0,                                                                \
    .out_rb_size = 8*1024,                                  \
    .task_stack = 3072+512,                                        \
    .task_core = 0,                                          \
    .task_prio = 23,                                          \
    .stack_in_ext = false,                                                      \
    .multi_out_num = 0,                                                         \
    .uninstall_drv = true,                                                      \
    .need_expand = false,                                                       \
    .expand_src_bits = 16,                               \
    .buffer_len = 3600,                                          \
    */


    wav_decoder_cfg_t wavdec_cfg = DEFAULT_WAV_DECODER_CONFIG();
    wav_decoder = wav_decoder_init(&wavdec_cfg); //Crea un identificador (handle) para un Elemento de Audio para decodificar datos WAV entrantes.

    /**
    default:
    .out_rb_size        = 8*1024,  \
    .task_stack         = 4*1024,       \
    .task_core          = 0,        \
    .task_prio          = 5,        \
    .stack_in_ext       = true,                         \

    */

    equalizer_cfg_t eq_cfg = DEFAULT_EQUALIZER_CONFIG();
    int set_gain[] = { -13,  -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13, -13};
    /**
    default:
        .samplerate     = 48000,                        \
        .channel        = 1,                            \
        .out_rb_size    = 8*1024,    \
        .task_stack     = 4*1024,         \
        .task_core      = 0,          \
        .task_prio      = 5,          \
        .stack_in_ext   = true,      
    */

    eq_cfg.set_gain =
        set_gain; // El tamaño del arreglo de ganancia debe ser el producto de NUMBER_BAND y el número de canales de los datos de flujo de audio. El valor mínimo de ganancia es de -13 dB.
    equalizer = equalizer_init(&eq_cfg); // es igual a "el"

    /**
    DEFAULT_AUDIO_ELEMENT_CONFIG:                
    .buffer_len         = 4*1024,\
    .task_stack         = 2*1024,   \
    .task_prio          = 5,    \
    .task_core          = 0,    \
    .multi_in_rb_num    = 0,                            \
    .multi_out_rb_num   = 0,                            \

    cfg.destroy = equalizer_destroy;
    cfg.process = equalizer_process;
    cfg.open = equalizer_open;
    cfg.close = equalizer_close;
    cfg.buffer_len = 0;
    cfg.tag = "equalizer";
    cfg.task_stack = config->task_stack;
    cfg.task_prio = config->task_prio;
    cfg.task_core = config->task_core;
    cfg.out_rb_size = config->out_rb_size;
    cfg.stack_in_ext = config->stack_in_ext;
}
    */

    ESP_LOGI(TAG, "[3.1] Register all elements to audio pipeline");
    audio_pipeline_register(pipeline, fatfs_stream_reader, "file_read");
    audio_pipeline_register(pipeline, wav_decoder, "wavdec");
    audio_pipeline_register(pipeline, equalizer, "equalizer");
    audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

    ESP_LOGI(TAG, "[3.2] Link it together [sdcard]-->fatfs_stream-->wav_decoder-->equalizer-->i2s_stream");
    audio_pipeline_link(pipeline, (const char *[]) {
        "file_read", "wavdec", "equalizer", "i2s"
    }, 4);

    ESP_LOGI(TAG, "[3.3] Set up  uri (file as fatfs_stream, wav as wav decoder, and default output is i2s)");
    audio_element_set_uri(fatfs_stream_reader, "/sdcard/Frunas.wav");

    ESP_LOGI(TAG, "[4.0] Set up  event listener");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

    ESP_LOGI(TAG, "[4.1] Listening event from all elements of pipeline");
    audio_pipeline_set_listener(pipeline, evt);

    ESP_LOGI(TAG, "[4.2] Listening event from peripherals");
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    ESP_LOGI(TAG, "[5.0] Start audio_pipeline");
    audio_pipeline_run(pipeline);

    ESP_LOGI(TAG, "[6.0] Listen for all pipeline events");
    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
            continue;
        }

        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *)wav_decoder
            && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
            audio_element_info_t music_info = {0};
            audio_element_getinfo(wav_decoder, &music_info);

            ESP_LOGI(TAG, "[ * ] Receive music info from wav decoder, sample_rates=%d, bits=%d, ch=%d",
                     music_info.sample_rates, music_info.bits, music_info.channels);
            if (equalizer_set_info(equalizer, music_info.sample_rates, music_info.channels) != ESP_OK) {
                break;
            }
            i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
            continue;
        }
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.cmd == AEL_MSG_CMD_REPORT_STATUS
            && (((int)msg.data == AEL_STATUS_STATE_STOPPED) || ((int)msg.data == AEL_STATUS_STATE_FINISHED))) {
            ESP_LOGW(TAG, "[ * ] Stop event received");
            break;
        }
    }

    ESP_LOGI(TAG, "[7.0] Stop audio_pipeline and release all resources");
    audio_pipeline_stop(pipeline);
    audio_pipeline_wait_for_stop(pipeline);
    audio_pipeline_terminate(pipeline);
    audio_pipeline_unregister(pipeline, fatfs_stream_reader);
    audio_pipeline_unregister(pipeline, wav_decoder);
    audio_pipeline_unregister(pipeline, equalizer);
    audio_pipeline_unregister(pipeline, i2s_stream_writer);

    /* Terminate the pipeline before removing the listener */
    audio_pipeline_remove_listener(pipeline);

    /* Stop all peripheral before removing the listener */
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(pipeline);
    audio_element_deinit(fatfs_stream_reader);
    audio_element_deinit(wav_decoder);
    audio_element_deinit(equalizer);
    audio_element_deinit(i2s_stream_writer);
    esp_periph_set_destroy(set);
}
