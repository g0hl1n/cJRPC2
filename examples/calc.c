/* SPDX-License-Identifier: MIT */

#include "cJRPC2.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int impl_add(const cJSON *params, cJSON **resp)
{
	double a, b, result;

	/* get parameters */
	if (PARAM_OK != cjrpc2_get_param_double(params, "a", &a)) {
		*resp = cjrpc2_impl_resp_error(JSONRPC2_EIPARAM, "Invalid/missing param 'a'", NULL);
		return CJRPC2_RET_ERROR;
	}
	if (PARAM_OK != cjrpc2_get_param_double(params, "b", &b)) {
		*resp = cjrpc2_impl_resp_error(JSONRPC2_EIPARAM, "Invalid/missing param 'b'", NULL);
		return CJRPC2_RET_ERROR;
	}

	/* calculate and "jsonify" result */
	result = a + b;
	*resp = cJSON_CreateNumber(result);

	return CJRPC2_RET_SUCCESS;
}

int impl_multiply(const cJSON *params, cJSON **resp)
{
	double a, b, result;

	/* get parameters */
	if (PARAM_OK != cjrpc2_get_param_double(params, "a", &a)) {
		*resp = cjrpc2_impl_resp_error(JSONRPC2_EIPARAM, "Invalid/missing param 'a'", NULL);
		return CJRPC2_RET_ERROR;
	}
	if (PARAM_OK != cjrpc2_get_param_double(params, "b", &b)) {
		*resp = cjrpc2_impl_resp_error(JSONRPC2_EIPARAM, "Invalid/missing param 'b'", NULL);
		return CJRPC2_RET_ERROR;
	}

	/* calculate and "jsonify" result */
	result = a * b;
	*resp = cJSON_CreateNumber(result);

	return CJRPC2_RET_SUCCESS;
}

static struct cjrpc2_method methods[] = {
	{"add", &impl_add},
	{"multiply", &impl_multiply},
	{NULL, NULL},
};

int main(int argc, char **argv)
{
	struct cjrpc2_handler *h;
	char *req, *ret;
	cJSON *params;

	(void)argc; /* unused */
	(void)argv; /* unused */

	printf("This program includes:\n");
	printf("  cJRPC2, %s\n", CJRPC2_COPYRIGHT);
	printf("  cJSON, Copyright (c) 2009-2017 Dave Gamble and cJSON contributors\n\n");

	h = cjrpc2_new_handler(methods);
	if (!h) {
		fprintf(stderr, "Unable to create new cJRPC2 handler: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	params = cJSON_CreateObject();
	cJSON_AddNumberToObject(params, "a", 24.6);
	cJSON_AddNumberToObject(params, "b", 17.4);
	req = cjrpc2_create_request_str("add", params, cJSON_CreateNumber(1));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	params = cJSON_CreateObject();
	cJSON_AddNumberToObject(params, "a", 24.6);
	cJSON_AddNumberToObject(params, "b", 17.4);
	req = cjrpc2_create_request_str("multiply", params, cJSON_CreateNumber(2));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	cjrpc2_free_handler(h);

	return EXIT_SUCCESS;
}
