/* SPDX-License-Identifier: MIT */

#include <stdarg.h>

#include "cJRPC2.h"

#include <setjmp.h>
#include <stdarg.h>

#include <cmocka.h>

#define VALUE_INIT 1.001

/*******************************************************************************
 * Test functions
 ******************************************************************************/
static void test_params_object_null(void **state)
{
	enum cjrpc2_param_status pstat;
	double value;

	(void)state; /* unused */

	value = VALUE_INIT;
	pstat = cjrpc2_get_param_double(NULL, "foo", &value);

	assert_int_equal(pstat, PARAM_MISSING);
	assert_float_equal(value, VALUE_INIT, 0);
}

static void test_params_object_empty(void **state)
{
	enum cjrpc2_param_status pstat;
	cJSON *params;
	double value;

	(void)state; /* unused */

	params = cJSON_CreateObject();

	value = VALUE_INIT;
	pstat = cjrpc2_get_param_double(params, "foo", &value);

	cJSON_Delete(params);
	assert_int_equal(pstat, PARAM_MISSING);
	assert_float_equal(value, VALUE_INIT, 0);
}

static void test_param_missing(void **state)
{
	enum cjrpc2_param_status pstat;
	cJSON *params;
	double value;

	(void)state; /* unused */

	params = cJSON_CreateObject();
	cJSON_AddStringToObject(params, "test", "value");

	value = VALUE_INIT;
	pstat = cjrpc2_get_param_double(params, "foo", &value);

	cJSON_Delete(params);
	assert_int_equal(pstat, PARAM_MISSING);
	assert_float_equal(value, VALUE_INIT, 0);
}

static void test_param_wrong_type(void **state)
{
	enum cjrpc2_param_status pstat;
	cJSON *params;
	double value;

	(void)state; /* unused */

	params = cJSON_CreateObject();
	cJSON_AddStringToObject(params, "foo", "bar");

	value = VALUE_INIT;
	pstat = cjrpc2_get_param_double(params, "foo", &value);

	cJSON_Delete(params);
	assert_int_equal(pstat, PARAM_WRONG_TYPE);
	assert_float_equal(value, VALUE_INIT, 0);
}

static void test_param_ok(void **state)
{
	enum cjrpc2_param_status pstat;
	cJSON *params;
	double pvalue, value;

	(void)state; /* unused */

	params = cJSON_CreateObject();
	pvalue = 13.37;
	cJSON_AddNumberToObject(params, "foo", pvalue);

	value = VALUE_INIT;
	pstat = cjrpc2_get_param_double(params, "foo", &value);

	cJSON_Delete(params);
	assert_int_equal(pstat, PARAM_OK);
	assert_float_equal(pvalue, value, 0);
}

/*******************************************************************************
 * Test main
 ******************************************************************************/
int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_params_object_null),
		cmocka_unit_test(test_params_object_empty),
		cmocka_unit_test(test_param_missing),
		cmocka_unit_test(test_param_wrong_type),
		cmocka_unit_test(test_param_ok),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
