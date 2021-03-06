/* SPDX-License-Identifier: MIT */

#include "cJRPC2.h"
#include "cJSON/cJSON.h"

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is a safeguard to prevent copy-pasters from using incompatible C and header files */
#if (CJRPC2_VERSION_MAJOR != 0) || (CJRPC_VERSION_MINOR != 0) || (CJRPC2_VERSION_PATCH != 2)
	#error cJRPC2.h and cJRPC2.c have different versions. Make sure that both have the same.
#endif

static cJSON *cjrpc2_create_skeleton()
{
	cJSON *j_skel, *j_jsonrpc;

	j_skel = cJSON_CreateObject();
	j_jsonrpc = cJSON_CreateStringReference(JSONRPC2_VERSION);
	if (!j_skel || !j_jsonrpc) {
		cJSON_Delete(j_skel);
		cJSON_Delete(j_jsonrpc);
		errno = ENOMEM;
		return NULL;
	}
	cJSON_AddItemToObject(j_skel, "jsonrpc", j_jsonrpc);

	return j_skel;
}

static cJSON *cjrpc2_create_response(cJSON *j_result, cJSON *j_id)
{
	cJSON *j_resp;

	j_resp = cjrpc2_create_skeleton();
	if (!j_resp) {
		/* errno already set by cjrpc2_create_skeleton() */
		return NULL;
	}

	if (!j_result) {
		j_result = cJSON_CreateNull();
	}
	cJSON_AddItemToObject(j_resp, "result", j_result);

	if (!j_id) {
		j_id = cJSON_CreateNull();
	}
	cJSON_AddItemToObject(j_resp, "id", j_id);

	return j_resp;
}

static cJSON *cjrpc2_create_response_error2(cJSON *j_err, cJSON *j_id)
{
	cJSON *j_resp;

	j_resp = cjrpc2_create_skeleton();
	if (!j_resp) {
		/* errno already set by cjrpc2_create_skeleton() */
		return NULL;
	}

	if (!j_err) {
		j_err = cJSON_CreateNull();
	}
	cJSON_AddItemToObject(j_resp, "error", j_err);

	if (!j_id) {
		j_id = cJSON_CreateNull();
	}
	cJSON_AddItemToObject(j_resp, "id", j_id);

	return j_resp;
}

static cJSON *cjrpc2_create_response_error(double code, const char *msg, cJSON *data, cJSON *j_id)
{
	cJSON *j_err;

	j_err = cjrpc2_impl_resp_error(code, msg, data);
	if (!j_err) {
		/* errno already set by cjrpc2_imlp_resp_error() */
		return NULL;
	}

	return cjrpc2_create_response_error2(j_err, j_id);
}

cJSON *cjrpc2_impl_resp_error(double code, const char *msg, cJSON *data)
{
	cJSON *ret;

	ret = cJSON_CreateObject();
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}
	cJSON_AddNumberToObject(ret, "code", code);

	if (msg) {
		cJSON_AddStringToObject(ret, "message", msg);
	}

	if (data) {
		cJSON_AddItemToObjectCS(ret, "data", data);
	}

	return ret;
}

cJSON *cjrpc2_create_request(const char *method, cJSON *j_params, cJSON *j_id)
{
	cJSON *j_req;

	j_req = cjrpc2_create_skeleton();
	if (!j_req) {
		/* errno already set by cjrpc2_create_skeleton() */
		return NULL;
	}

	cJSON_AddStringToObject(j_req, "method", method);

	if (j_params) {
		cJSON_AddItemToObject(j_req, "params", j_params);
	}

	if (!j_id) {
		j_id = cJSON_CreateNull();
	}
	cJSON_AddItemToObject(j_req, "id", j_id);

	return j_req;
}

char *cjrpc2_create_request_str(const char *method, cJSON *j_params, cJSON *j_id)
{
	cJSON *req;
	char *req_str;

	req = cjrpc2_create_request(method, j_params, j_id);
	if (!req) {
		/* errno set by cjrpc2_create_request() */
		return NULL;
	}
	req_str = cJSON_PrintUnformatted(req);
	cJSON_Delete(req);

	return req_str;
}

const char *cjrpc2_version(void)
{
	static char version[15];
	sprintf(version, "%i.%i.%i", CJRPC2_VERSION_MAJOR, CJRPC2_VERSION_MINOR,
		CJRPC2_VERSION_PATCH);

	return version;
}

struct cjrpc2_handler *cjrpc2_new_handler(struct cjrpc2_method *methods)
{
	return cjrpc2_new_handler_m(1, methods);
}

struct cjrpc2_handler *cjrpc2_new_handler_m(size_t count, ...)
{
	va_list ap;
	struct cjrpc2_handler *h;
	struct cjrpc2_method *methods;
	struct cjrpc2_method_entry *me;
	unsigned int i, j;

	if (!count) {
		errno = EINVAL;
		return NULL;
	}

	h = (struct cjrpc2_handler *)malloc(sizeof(struct cjrpc2_handler));
	if (!h) {
		goto exit_free_enomem;
	}

	h->mlist_head = (struct cjrpc2_method_entry *)malloc(sizeof(struct cjrpc2_method_entry));
	if (!h->mlist_head) {
		goto exit_free_enomem;
	}
	me = h->mlist_head;
	me->method = NULL;
	me->next = NULL;

	va_start(ap, count);
	for (j = 0; j < count; j++) {
		methods = va_arg(ap, struct cjrpc2_method *);
		if (!methods)
			continue;

		for (i = 0; methods[i].name; i++) {
			if (!me->method) {
				/* populate first */
				me->method = &methods[i];
				me->next = NULL;
				continue;
			}

			me->next = (struct cjrpc2_method_entry *)malloc(
				sizeof(struct cjrpc2_method_entry));
			if (!h->mlist_head) {
				goto exit_free_enomem;
			}
			me = me->next;
			me->method = &methods[i];
			me->next = NULL;
		}
	}
	va_end(ap);

	return h;

exit_free_enomem:
	cjrpc2_free_handler(h);
	errno = ENOMEM;
	return NULL;
}

void cjrpc2_free_handler(struct cjrpc2_handler *h)
{
	struct cjrpc2_method_entry *me, *mef;

	if (!h) {
		return;
	}
	mef = h->mlist_head;
	while (mef) {
		me = mef->next;
		free(mef);
		mef = me;
	}
	free(h);
}

char *cjrpc2_handle_request(struct cjrpc2_handler *h, const char *req)
{
	struct cjrpc2_method_entry *me;
	cJSON *j_req, *j_reqjsonrpc, *j_method, *j_params, *j_id;
	cJSON *j_resp, *j_result;
	char *ret;

	if (!h) {
		errno = EINVAL;
		return NULL;
	}

	/* parse and validate request */
	j_req = cJSON_Parse(req);
	if (!j_req) {
		j_resp = cjrpc2_create_response_error(JSONRPC2_EPARSE, "parse error", NULL, NULL);
		goto exit_ret;
	}

	j_reqjsonrpc = cJSON_GetObjectItem(j_req, "jsonrpc");
	j_method = cJSON_GetObjectItem(j_req, "method");
	if (!j_reqjsonrpc || !cJSON_IsString(j_reqjsonrpc) ||
	    strcmp(j_reqjsonrpc->valuestring, JSONRPC2_VERSION) || !j_method ||
	    !cJSON_IsString(j_method) || !j_method->valuestring) {
		j_resp =
			cjrpc2_create_response_error(JSONRPC2_EIREQ, "invalid request", NULL, NULL);
		goto exit_ret;
	}

	j_id = cJSON_DetachItemFromObject(j_req, "id");
	j_params = cJSON_GetObjectItem(j_req, "params");

	/* find function & execute */
	j_resp = NULL;
	me = h->mlist_head;
	while (me) {
		if (strcmp(me->method->name, j_method->valuestring)) {
			me = me->next;
			continue;
		}
		if (me->method->func(j_params, &j_result) == CJRPC2_RET_SUCCESS) {
			if (j_id) {
				j_resp = cjrpc2_create_response(j_result, j_id);
			}
			goto exit_ret;
		}
		if (j_id) {
			j_resp = cjrpc2_create_response_error2(j_result, j_id);
		}
		goto exit_ret;
	}
	if (j_id) {
		j_resp = cjrpc2_create_response_error(JSONRPC2_ENOMET, "method not found", NULL,
						      j_id);
	}

exit_ret:
	if (j_resp) {
		ret = cJSON_PrintUnformatted(j_resp);
		cJSON_Delete(j_resp);
	} else {
		/* notification */
		ret = (char *)malloc(1);
		if (!ret) {
			/* errno set by malloc() */
			return NULL;
		}
		*ret = '\0';
	}
	cJSON_Delete(j_req);
	return ret;
}

enum cjrpc2_param_status cjrpc2_get_param_double(const cJSON *params, const char *name,
						 double *value)
{
	cJSON *p_item;

	if (!(p_item = cJSON_GetObjectItem(params, name))) {
		return PARAM_MISSING;
	}
	if (!cJSON_IsNumber(p_item)) {
		return PARAM_WRONG_TYPE;
	}

	*value = cJSON_GetNumberValue(p_item);
	return PARAM_OK;
}

enum cjrpc2_param_status cjrpc2_get_param_double_range(const cJSON *params, const char *name,
						       double *value, const double min,
						       const double max)
{
	enum cjrpc2_param_status pstat;
	double dval;

	if (PARAM_OK != (pstat = cjrpc2_get_param_double(params, name, &dval))) {
		return pstat;
	}

	if (dval < min || dval > max) {
		return PARAM_OO_RANGE;
	}

	*value = dval;
	return PARAM_OK;
}

enum cjrpc2_param_status cjrpc2_get_param_int(const cJSON *params, const char *name, int *value)
{
	return cjrpc2_get_param_int_range(params, name, value, INT_MIN, INT_MAX);
}

enum cjrpc2_param_status cjrpc2_get_param_int_range(const cJSON *params, const char *name,
						    int *value, const int min, const int max)
{
	enum cjrpc2_param_status pstat;
	double dval;

	if (PARAM_OK != (pstat = cjrpc2_get_param_double_range(params, name, &dval, min, max))) {
		return pstat;
	}

	if (floor(dval) != dval) {
		return PARAM_NUM_NOINT;
	}

	*value = (int)dval;
	return PARAM_OK;
}

enum cjrpc2_param_status cjrpc2_get_param_bool(const cJSON *params, const char *name, bool *value)
{
	cJSON *p_item;

	if (!(p_item = cJSON_GetObjectItem(params, name))) {
		return PARAM_MISSING;
	}
	if (!cJSON_IsBool(p_item)) {
		return PARAM_WRONG_TYPE;
	}

	*value = cJSON_IsTrue(p_item);
	return PARAM_OK;
}

enum cjrpc2_param_status cjrpc2_get_param_string(const cJSON *params, const char *name,
						 char **value)
{
	cJSON *p_item;
	char *json_str;
	size_t value_size;

	if (!(p_item = cJSON_GetObjectItem(params, name))) {
		return PARAM_MISSING;
	}
	if (!cJSON_IsString(p_item)) {
		return PARAM_WRONG_TYPE;
	}
	json_str = cJSON_GetStringValue(p_item);
	value_size = strlen(json_str) + 1;

	if (!(*value = (char *)malloc(value_size))) {
		return PARAM_NOMEM;
	}

	strncpy(*value, json_str, value_size);
	return PARAM_OK;
}
