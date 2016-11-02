#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "../src/milight.h"

void milight_reset_counter(void);

START_TEST(packet)
{
    milight_command cmd = {
        .id = 0xFFF10203,
        .color = 0x04,
        .brightness = 0x05,
        .code = MILIGHT_COMMAND_CODE_CH2_OFF,
        .disco = MILIGHT_DISCO_MODE_GREEN_FADE
    };

    uint8_t actual[10], expected[10] = {0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0xCE, 0x00};

    // Check basic packet structure and ignore CRC
    milight_reset_counter();
    write_packet(&cmd, actual);
    ck_assert_msg(
        memcmp(expected, actual, 8) == 0, 
        "\n\twrite_packet() # Checking packet"
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7]);

    // Test to see if counter increments by one (still ignoring CRC)
    expected[7] = 0x01;
    write_packet(&cmd, actual);
    ck_assert_msg(
        expected[7] == actual[7], 
        "\n\twrite_packet() # Checking counter"
        "\n\texpected: 0x%02X"
        "\n\t  actual: 0x%02X", 
        expected[7], actual[7]);

    // Check CRC explicitly
    expected[7] = 0x02;
    write_packet(&cmd, actual);
    ck_assert_msg(
        memcmp(&expected[8], &actual[8], 2) == 0, 
        "\n\twrite_packet() # Checking CRC"
        "\n\texpected: 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X", 
        expected[8], expected[9], 
        actual[8], actual[9]);

    // Set Discomode and command
    milight_reset_counter();
    cmd.code = MILIGHT_COMMAND_CODE_DISCO_MODE;
    memcpy(expected, (uint8_t[]){0x07, 0x16, 0x02, 0x03, 0x04, 0x05, 0x0C, 0x00, 0x61, 0x86}, 10);
    write_packet(&cmd, actual);
    ck_assert_msg(
        memcmp(expected, actual, 10) == 0, 
        "\n\twrite_packet() # Checking disco mode"
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], expected[8], expected[9], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9]);

    // Long Press ON
    milight_reset_counter();
    cmd.code = MILIGHT_COMMAND_CODE_ON | MILIGHT_COMMAND_ALT;
    memcpy(expected, (uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x11, 0x00, 0x45, 0xFB}, 10);
    write_packet(&cmd, actual);
    ck_assert_msg(
        memcmp(expected, actual, 10) == 0, 
        "\n\twrite_packet() # Checking disco mode"
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], expected[8], expected[9], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9]);
}
END_TEST

Suite* crc_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Milight");

    tc_core = tcase_create("Packet");

    tcase_add_test(tc_core, packet);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(int argc, char *argv[]){
    int failed = 0;
    Suite *s;
    SRunner *sr;

    s = crc_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}