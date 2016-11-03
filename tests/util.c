#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "../src/util.h"

void milight_reset_counter(void);

uint8_t actual[11], expected[11];

void setup(void)
{
    memset(actual, 0, sizeof(actual));
    memset(expected, 0, sizeof(expected));
}

// Check basic packet structure and ignore CRC
START_TEST(test_shift_nibbles)
{
    memcpy(expected, (uint8_t[]){0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00}, 9);
    
    shift_nibbles((uint8_t[]){0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}, actual, 8);
    ck_assert_msg(
        memcmp(expected, actual, 9) == 0, 
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], expected[8], expected[9], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9]);

    memcpy(expected, (uint8_t[]){0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x00}, 10);
    
    shift_nibbles((uint8_t[]){0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00}, actual, 9);
    ck_assert_msg(
        memcmp(expected, actual, 10) == 0, 
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], expected[8], expected[9], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9]);
}
END_TEST

// Check basic packet structure and ignore CRC
START_TEST(test_shift_swap_nibbles)
{
    memcpy(expected, (uint8_t[]){0x70, 0x00, 0x2b, 0xaf, 0x6e, 0x07, 0x11, 0xa0, 0x28, 0x52, 0x0c}, 11);
    
    shift_swap_nibbles((uint8_t[]){0x07, 0xb0, 0xf2, 0xea, 0x76, 0x10, 0x01, 0x8a, 0x22, 0xc5}, actual, 10);
    ck_assert_msg(
        memcmp(expected, actual, 11) == 0, 
        "\n\texpected: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"
        "\n\t  actual: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
        expected[0], expected[1], expected[2], expected[3], expected[4], expected[5], expected[6], expected[7], expected[8], expected[9], expected[10], 
        actual[0], actual[1], actual[2], actual[3], actual[4], actual[5], actual[6], actual[7], actual[8], actual[9], actual[10]);
}
END_TEST

Suite* crc_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("Utilities");

    tc_core = tcase_create("core");
    tcase_add_checked_fixture(tc_core, setup, NULL);

    tcase_add_test(tc_core, test_shift_nibbles);
    tcase_add_test(tc_core, test_shift_swap_nibbles);

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