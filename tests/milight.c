#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "../src/milight.h"

void milight_reset_counter(void);

milight_command actual_cmd, expected_cmd;
uint8_t actual_packet[10], expected_packet[10];

void setup_write(void)
{
    memset(&actual_cmd, 0, sizeof(actual_cmd));
    memset(actual_packet, 0, sizeof(actual_packet));
    memset(expected_packet, 0, sizeof(expected_packet));

    actual_cmd.id = 0xFFF10203;
}

// Check basic packet structure and ignore CRC
START_TEST(packet_structure)
{
    actual_cmd.color = 0x04;
    actual_cmd.brightness = 0x05;
    actual_cmd.code = MILIGHT_COMMAND_CODE_CH2_OFF,
    actual_cmd.sequence = 0x07;

    memcpy(expected_packet, (uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x63, 0x57}, 10);
    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        memcmp(expected_packet, actual_packet, 7) == 0, 
        "\n\tmilight_write_packet() # Checking packet"
        "\n\texpected_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected_packet[0], expected_packet[1], expected_packet[2], expected_packet[3], expected_packet[4], expected_packet[5], expected_packet[6], expected_packet[7], 
        actual_packet[0], actual_packet[1], actual_packet[2], actual_packet[3], actual_packet[4], actual_packet[5], actual_packet[6], actual_packet[7]);
}
END_TEST

// Test to see if counter increments by one (still ignoring CRC)
START_TEST(packet_counter)
{
    actual_cmd.color = 0x04;
    actual_cmd.brightness = 0x05;
    actual_cmd.code = MILIGHT_COMMAND_CODE_CH2_OFF,

    memcpy(expected_packet, (uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0xCE, 0x00}, 10);

    uint8_t counter_expected_packet = 0x00;

    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        counter_expected_packet == actual_packet[7], 
        "\n\tmilight_write_packet() # Checking counter"
        "\n\texpected_packet: 0x%02X"
        "\n\t  actual_packet: 0x%02X", 
        counter_expected_packet, actual_packet[7]);

    counter_expected_packet = 0x01;

    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        counter_expected_packet == actual_packet[7], 
        "\n\tmilight_write_packet() # Checking counter"
        "\n\texpected_packet: 0x%02X"
        "\n\t  actual_packet: 0x%02X", 
        counter_expected_packet, actual_packet[7]);
}
END_TEST

// Check CRC explicitly
START_TEST(packet_crc16)
{
    actual_cmd.color = 0x04;
    actual_cmd.brightness = 0x05;
    actual_cmd.code = MILIGHT_COMMAND_CODE_CH2_OFF,

    memcpy(expected_packet, (uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0xDC, 0x23}, 10);

    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        memcmp(&expected_packet[8], &actual_packet[8], 2) == 0, 
        "\n\tmilight_write_packet() # Checking CRC"
        "\n\texpected_packet: 0x%02X 0x%02X"
        "\n\t  actual_packet: 0x%02X 0x%02X", 
        expected_packet[8], expected_packet[9], 
        actual_packet[8], actual_packet[9]);
}
END_TEST

// Set Discomode and command
START_TEST(disco_modes)
{
    actual_cmd.code = MILIGHT_COMMAND_CODE_DISCO_MODE;
    actual_cmd.disco = MILIGHT_DISCO_MODE_GREEN_FADE;

    memcpy(expected_packet, (uint8_t[]){0x07, 0x16, 0x02, 0x03, 0x00, 0x00, 0x0C, 0x00, 0x30, 0xCD}, 10);
    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        memcmp(expected_packet, actual_packet, 10) == 0, 
        "\n\tmilight_write_packet() # Checking disco mode"
        "\n\texpected_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected_packet[0], expected_packet[1], expected_packet[2], expected_packet[3], expected_packet[4], expected_packet[5], expected_packet[6], expected_packet[7], expected_packet[8], expected_packet[9], 
        actual_packet[0], actual_packet[1], actual_packet[2], actual_packet[3], actual_packet[4], actual_packet[5], actual_packet[6], actual_packet[7], actual_packet[8], actual_packet[9]);
}
END_TEST

// Long Press ON
START_TEST(alt_mode)
{
    actual_cmd.code = MILIGHT_COMMAND_CODE_ON | MILIGHT_COMMAND_ALT;

    memcpy(expected_packet, (uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x00, 0x00, 0x11, 0x00, 0x14, 0xB0}, 10);
    milight_write_packet(&actual_cmd, actual_packet);
    ck_assert_msg(
        memcmp(expected_packet, actual_packet, 10) == 0, 
        "\n\tmilight_write_packet() # Checking disco mode"
        "\n\texpected_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual_packet: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected_packet[0], expected_packet[1], expected_packet[2], expected_packet[3], expected_packet[4], expected_packet[5], expected_packet[6], expected_packet[7], expected_packet[8], expected_packet[9], 
        actual_packet[0], actual_packet[1], actual_packet[2], actual_packet[3], actual_packet[4], actual_packet[5], actual_packet[6], actual_packet[7], actual_packet[8], actual_packet[9]);
}
END_TEST

void setup_read(void)
{
    memset(&expected_cmd, 0, sizeof(actual_cmd));
    memset(&actual_cmd, 0, sizeof(actual_cmd));
    memset(actual_packet, 0, sizeof(actual_packet));
    memset(expected_packet, 0, sizeof(expected_packet));
}

// Read in a packet
START_TEST(read_packet_protections)
{
    int expected, actual;
    // Test for MILIGHT_RESULT_INVALID_LENGTH
    expected = MILIGHT_RESULT_INVALID_LENGTH;
    actual = milight_read_packet(&actual_cmd, (const uint8_t[]){0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    ck_assert_msg(
        actual == expected,
        "\n\texpected: [%d] = \"%s\""
        "\n\t  actual: [%d] = \"%s\"", 
        expected, milight_get_result_msg(expected),
        actual, milight_get_result_msg(actual));

    // Test for MILIGHT_RESULT_CRC_FAILED
    expected = MILIGHT_RESULT_CRC_FAILED;
    actual = milight_read_packet(&actual_cmd, (const uint8_t[]){0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    ck_assert_msg(
        actual == expected,
        "\n\texpected: [%d] = \"%s\""
        "\n\t  actual: [%d] = \"%s\"", 
        expected, milight_get_result_msg(expected),
        actual, milight_get_result_msg(actual));
}
END_TEST

// Read in a packet
START_TEST(read_packet)
{
    expected_cmd = (milight_command) {
        .id = 0x00010203,
        .color = 0x04,
        .brightness = 0x05,
        .code = MILIGHT_COMMAND_CODE_CH2_OFF,
        .sequence = 0x07
    };

    // No need to catch result, already tested for in read_packet_protections
    milight_read_packet(&actual_cmd, (const uint8_t[]){0x07, 0x10, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x63, 0x57});

    ck_assert_msg(
        memcmp(&expected_cmd, &actual_cmd, sizeof(milight_command)) == 0, 
        "\n\texpected_cmd{ .id = 0x%08X, .color = 0x%02X, .brightness = 0x%02X, .code = 0x%02X, .disco = 0x%02X, .sequence = 0x%02X }"
        "\n\t  actual_cmd{ .id = 0x%08X, .color = 0x%02X, .brightness = 0x%02X, .code = 0x%02X, .disco = 0x%02X, .sequence = 0x%02X }", 
        expected_cmd.id,expected_cmd.color,expected_cmd.brightness,expected_cmd.code,expected_cmd.disco,expected_cmd.sequence, 
        actual_cmd.id,actual_cmd.color,actual_cmd.brightness,actual_cmd.code,actual_cmd.disco,actual_cmd.sequence);
}
END_TEST

// Read in a packet
START_TEST(read_packet_disco)
{
    expected_cmd = (milight_command) {
        .id = 0x00010203,
        .color = 0x04,
        .brightness = 0x05,
        .code = MILIGHT_COMMAND_CODE_DISCO_MODE,
        .disco = MILIGHT_DISCO_MODE_GREEN_FADE,
        .sequence = 0x07
    };

    // No need to catch result, already tested for in read_packet_protections
    milight_read_packet(&actual_cmd, (const uint8_t[]){0x07, 0x16, 0x02, 0x03, 0x04, 0x05, 0x0C, 0x07, 0xDE, 0xF2});

    ck_assert_msg(
        memcmp(&expected_cmd, &actual_cmd, sizeof(milight_command)) == 0, 
        "\n\texpected_cmd{ .id = 0x%08X, .color = 0x%02X, .brightness = 0x%02X, .code = 0x%02X, .disco = 0x%02X, .sequence = 0x%02X }"
        "\n\t  actual_cmd{ .id = 0x%08X, .color = 0x%02X, .brightness = 0x%02X, .code = 0x%02X, .disco = 0x%02X, .sequence = 0x%02X }", 
        expected_cmd.id,expected_cmd.color,expected_cmd.brightness,expected_cmd.code,expected_cmd.disco,expected_cmd.sequence, 
        actual_cmd.id,actual_cmd.color,actual_cmd.brightness,actual_cmd.code,actual_cmd.disco,actual_cmd.sequence);
}
END_TEST

Suite* crc_suite(void){
    Suite *s;
    TCase *tc_write, *tc_read;

    s = suite_create("Milight");

    tc_write = tcase_create("Write Packet Structure");
    tcase_add_checked_fixture(tc_write, setup_write, NULL);

    tcase_add_test(tc_write, packet_structure);
    tcase_add_test(tc_write, packet_counter);
    tcase_add_test(tc_write, packet_crc16);
    tcase_add_test(tc_write, disco_modes);
    tcase_add_test(tc_write, alt_mode);

    tc_read = tcase_create("Read Packet Structure");
    tcase_add_checked_fixture(tc_write, setup_write, NULL);

    tcase_add_test(tc_write, read_packet_protections);
    tcase_add_test(tc_write, read_packet);
    tcase_add_test(tc_write, read_packet_disco);

    suite_add_tcase(s, tc_write);
    suite_add_tcase(s, tc_read);

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