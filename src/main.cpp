#include <Arduino.h>
#include <stdio.h>
extern "C" {
#include "stm32f3xx_hal.h"
}

// 定数定義
const uint16_t DAC_MAX_VALUE      = 4095;
const double   SENSOR_VDD         = 3.3;
const double   NO_CURRENT_OUTPUT  = 1.65;
const double   SENSOR_MAX_CURRENT = 12.9;

// ハンドル定義
DAC_HandleTypeDef hdac1;

// DACの初期化
static void dac1_init() {
    __HAL_RCC_DAC1_CLK_ENABLE();

    hdac1.Instance = DAC1;
    if (HAL_DAC_Init(&hdac1) != HAL_OK) {
        Error_Handler();
    }

    DAC_ChannelConfTypeDef sConfig = {0};
    sConfig.DAC_Trigger            = DAC_TRIGGER_NONE;
    if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }

    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

// 電圧からDACの値（0〜4095）への変換
uint16_t voltage_to_dac_value(double voltage) {
    return (voltage / 3.3) * DAC_MAX_VALUE;
}

// 電流値から対応する電圧値への変換
double current_to_voltage(double current) {
    return (((SENSOR_VDD - 0.1) - NO_CURRENT_OUTPUT) / SENSOR_MAX_CURRENT) * current + NO_CURRENT_OUTPUT;
}

// 電流値からDACを設定する関数
void set_current_limit(double current) {
    uint16_t value = voltage_to_dac_value(current_to_voltage(current));
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

void setup() {
    // DACの初期化を実行
    dac1_init();

    // 内蔵LEDを有効化して点灯（動作確認用）
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // 例として、起動時に電流リミットを5.0Aに設定
    set_current_limit(2.0);
}

void loop() {
    // 必要に応じてメインループ内で電流値を変更可能
    // 例: 1秒ごとに電流リミットを変更するなど
    // set_current_limit(5.0);
    // delay(1000);
    // set_current_limit(8.0);
    // delay(1000);
}