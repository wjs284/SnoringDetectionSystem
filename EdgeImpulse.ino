/*
 * Edge Impulse -----------------------------------------------------------------------------------------------------------------------------------
 */
#include "config.h"

#define SCK 14
#define WS 15
#define SD 32
#define L_R_Channel 0 // 接地
static void audio_inference_callback(uint32_t n_bytes)
{
    for (int i = 0; i < n_bytes >> 1; i++)
    {
        inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

        if (inference.buf_count >= inference.n_samples)
        {
            inference.buf_select ^= 1;
            inference.buf_count = 0;
            inference.buf_ready = 1;
        }
    }
}

static void capture_samples(void *arg)
{

    const int32_t i2s_bytes_to_read = (uint32_t)arg;
    size_t bytes_read = i2s_bytes_to_read;

    while (record_status)
    {

        /* read data at once from i2s */
        i2s_read((i2s_port_t)1, (void *)sampleBuffer, i2s_bytes_to_read, &bytes_read, 100);

        if (bytes_read <= 0)
        {
            ei_printf("Error in I2S read : %d", bytes_read);
        }
        else
        {
            if (bytes_read < i2s_bytes_to_read)
            {
                ei_printf("Partial I2S read");
            }

            // scale the data (otherwise the sound is too quiet)
            for (int x = 0; x < i2s_bytes_to_read / 2; x++)
            {
                sampleBuffer[x] = (int16_t)(sampleBuffer[x]) * 8;
            }

            if (record_status)
            {
                audio_inference_callback(i2s_bytes_to_read);
            }
            else
            {
                break;
            }
        }
    }
    vTaskDelete(NULL);
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 *
 * @param[in]  n_samples  The n samples
 *
 * @return     { description_of_the_return_value }
 */
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffers[0] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL)
    {
        return false;
    }

    inference.buffers[1] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[1] == NULL)
    {
        ei_free(inference.buffers[0]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    if (i2s_init(EI_CLASSIFIER_FREQUENCY))
    {
        ei_printf("Failed to start I2S!");
    }

    ei_sleep(100);

    record_status = true;

    xTaskCreate(capture_samples, "CaptureSamples", 1024 * 32, (void *)sample_buffer_size, 10, NULL);

    return true;
}

/**
 * @brief      Wait on new data
 *
 * @return     True when finished
 */
static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1)
    {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0)
    {
        delay(1);
    }

    inference.buf_ready = 0;
    return true;
}

/**
 * 获取原始音频信号数据
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

    return 0;
}

/**
 * 初始化i2s外设
 */
static int i2s_init(uint32_t sampling_rate)
{
    // Start listening for audio: MONO @ 8/16KHz
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
        .sample_rate = sampling_rate,
        .bits_per_sample = (i2s_bits_per_sample_t)16,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 512,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = -1,
    };
    i2s_pin_config_t pin_config = {
        // L/R_Channel GND
        .bck_io_num = SCK,  // IIS_SCLK  SCK
        .ws_io_num = WS,    // IIS_LCLK  WS
        .data_out_num = -1, // IIS_DSIN  NONE
        .data_in_num = SD,  // IIS_DOUT  SD
    };
    esp_err_t ret = 0;

    ret = i2s_driver_install((i2s_port_t)1, &i2s_config, 0, NULL);
    if (ret != ESP_OK)
    {
        ei_printf("Error in i2s_driver_install");
    }

    ret = i2s_set_pin((i2s_port_t)1, &pin_config);
    if (ret != ESP_OK)
    {
        ei_printf("Error in i2s_set_pin");
    }

    ret = i2s_zero_dma_buffer((i2s_port_t)1);
    if (ret != ESP_OK)
    {
        ei_printf("Error in initializing dma buffer with 0");
    }

    return int(ret);
}
