/* SPDX-License-Identifier: MIT */

#include "cJRPC2.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int impl_foo1(const cJSON *params, cJSON **resp)
{
	(void)params; /* unused */

	*resp = cJSON_CreateString("foo1");

	return CJRPC2_RET_SUCCESS;
}

int impl_foo2(const cJSON *params, cJSON **resp)
{
	(void)params; /* unused */

	*resp = cJSON_CreateString("foo2");

	return CJRPC2_RET_SUCCESS;
}

static struct cjrpc2_method foo[] = {
	{"foo.1", &impl_foo1},
	{"foo.2", &impl_foo2},
	{NULL, NULL},
};

int impl_bar1(const cJSON *params, cJSON **resp)
{
	(void)params; /* unused */

	*resp = cJSON_CreateString("bar1");

	return CJRPC2_RET_SUCCESS;
}

int impl_bar2(const cJSON *params, cJSON **resp)
{
	(void)params; /* unused */

	*resp = cJSON_CreateString("bar1");

	return CJRPC2_RET_SUCCESS;
}

static struct cjrpc2_method bar[] = {
	{"bar.1", &impl_bar1},
	{"bar.2", &impl_bar2},
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

	h = cjrpc2_new_handler_m(2, foo, bar);
	if (!h) {
		fprintf(stderr, "Unable to create new cJRPC2 handler: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	req = cjrpc2_create_request_str("foo.1", NULL, cJSON_CreateNumber(11));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	req = cjrpc2_create_request_str("foo.2", NULL, cJSON_CreateNumber(12));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	req = cjrpc2_create_request_str("bar.1", NULL, cJSON_CreateNumber(21));
	ret = cjrpc2_handle_request(h, req);
	free(req);
	if (!ret) {
		fprintf(stderr, "Unable to handle cJRPC2 request: %s\n", strerror(errno));
		cjrpc2_free_handler(h);
		return EXIT_FAILURE;
	}
	printf("%s\n", ret);
	free(ret);

	req = cjrpc2_create_request_str("bar.2", NULL, cJSON_CreateNumber(22));
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
