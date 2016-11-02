#ifndef MILIGHT_MILIGHT_H_
#define MILIGHT_MILIGHT_H_

#include <stdint.h>

enum milight_command_code{
    MILIGHT_COMMAND_CODE_NONE         = 0x00,
    MILIGHT_COMMAND_CODE_ON           = 0x01,
    MILIGHT_COMMAND_CODE_OFF          = 0x02,
    MILIGHT_COMMAND_CODE_CH1_ON       = 0x03,
    MILIGHT_COMMAND_CODE_CH1_OFF      = 0x04,
    MILIGHT_COMMAND_CODE_CH2_ON       = 0x05,
    MILIGHT_COMMAND_CODE_CH2_OFF      = 0x06,
    MILIGHT_COMMAND_CODE_CH3_ON       = 0x07,
    MILIGHT_COMMAND_CODE_CH3_OFF      = 0x08,
    MILIGHT_COMMAND_CODE_CH4_ON       = 0x09,
    MILIGHT_COMMAND_CODE_CH4_OFF      = 0x0A,
    MILIGHT_COMMAND_CODE_DISCO_FASTER = 0x0B,
    MILIGHT_COMMAND_CODE_DISCO_MODE   = 0x0C,
    MILIGHT_COMMAND_CODE_DISCO_SLOWER = 0x0D,
    MILIGHT_COMMAND_CODE_BRIGHTNESS   = 0x0E,
    MILIGHT_COMMAND_CODE_COLOR        = 0x0F,
    
    // Special flag for "Long press"
    MILIGHT_COMMAND_ALT               = 0x10
};

enum milight_disco_mode{
    MILIGHT_DISCO_MODE_COLOR_FADE = 0x00,
    MILIGHT_DISCO_MODE_WHITE_FADE = 0x01,
    MILIGHT_DISCO_MODE_ALL_FADE   = 0x02,
    MILIGHT_DISCO_MODE_ALL_JUMP   = 0x03,
    MILIGHT_DISCO_MODE_RANDOM     = 0x04,
    MILIGHT_DISCO_MODE_RED_FADE   = 0x05,
    MILIGHT_DISCO_MODE_GREEN_FADE = 0x06,
    MILIGHT_DISCO_MODE_BLUE_FADE  = 0x07,
    MILIGHT_DISCO_MODE_ALL        = 0x08,
};

typedef struct{
    uint32_t id;
    uint8_t color;
    uint8_t brightness;
    enum milight_command_code code;
    enum milight_disco_mode disco;
}milight_command;

void write_packet(milight_command *command, uint8_t *packet);

#endif /* MILIGHT_MILIGHT_H_ */ 