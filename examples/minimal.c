/* SPDX-License-Identifier: MIT */

#include "cJRPC2.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int impl_get_version(const cJSON *params, cJSON **resp)
{
	(void)params; /* unused */

	*resp = cJSON_CreateString(cjrpc2_version());

	return CJRPC2_RET_SUCCESS;
}

int impl_echo(const cJSON *params, cJSON **resp)
{
	if (!params) {
		*resp = cjrpc2_impl_resp_error(JSONRPC2_EIPARAM, "Invalid params", NULL);
		return CJRPC2_RET_ERROR;
	}

	*resp = cJSON_Duplicate(params, cJSON_True);
	return CJRPC2_RET_SUCCESS;
}

static struct cjrpc2_method methods[] = {
	{"get-version", &impl_get_version},
	{"echo", &impl_echo},
	{NULL, NULL},
};

int main(int argc, char **argv)
{
	struct cjrpc2_handler *h;
	char *req, *ret;

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

	req = cjrpc2_create_request_str("get-version", NULL, cJSON_CreateNumber(42));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	req = cjrpc2_create_request_str("echo", cJSON_CreateString("foo"),
					cJSON_CreateString("bar"));
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
