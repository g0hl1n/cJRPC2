/* SPDX-License-Identifier: MIT */
#ifndef CJRPC2__H
#define CJRPC2__H

#include "cJSON/cJSON.h"

#define JSONRPC2_VERSION "2.0"

/* project version */
#define CJRPC2_VERSION_MAJOR 0
#define CJRPC2_VERSION_MINOR 0
#define CJRPC2_VERSION_PATCH 1
#define CJRPC2_COPYRIGHT     "Copyright (c) 2020 Richard Leitner"
#define CJRPC2_URL	     "https://github.com/g0hl1n/cJRPC2"

/* fixed JSONRPC2 error codes */
#define JSONRPC2_EPARSE	 -32700 /**< Parse error: Invalid JSON was received by the server */
#define JSONRPC2_EIREQ	 -32600 /**< Invalid Request: The JSON sent is not a valid Request object */
#define JSONRPC2_ENOMET	 -32601 /**< Method not found: The method doesn't exist/is not available */
#define JSONRPC2_EIPARAM -32602 /**< Invalid params: Invalid method parameter(s) */
#define JSONRPC2_EINTERN -32603 /**< Internal error: Internal JSON-RPC error */
/* -32000 to -32099 Server error: Reserved for implementation-defined server-errors. */

/* constants */
#define CJRPC2_RET_SUCCESS 0
#define CJRPC2_RET_ERROR   1

struct cjrpc2_method {
	const char *name;
	int (*func)(const cJSON *params, cJSON **resp);
};

struct cjrpc2_method_entry {
	struct cjrpc2_method *method;
	struct cjrpc2_method_entry *next;
};

struct cjrpc2_handler {
	struct cjrpc2_method_entry *mlist_head;
};

/**
 * @fn
 * @brief get the cJRPC2 version as string
 * @retval NULL terminated cJRPC2 version
 */
const char *cjrpc2_version(void);

/**
 * @fn
 * @brief create a new cJRPC2 handler
 * @param methods NULL terminated array of methods to register
 * @retval new handler instance on success
 * @retval NULL on error
 * @retval errno EINVAL or ENOMEN on error
 */
struct cjrpc2_handler *cjrpc2_new_handler(struct cjrpc2_method *methods);

/**
 * @fn
 * @brief free a existing cJRPC2 handler
 * @param h handler to free
 */
void cjrpc2_free_handler(struct cjrpc2_handler *h);

/**
 * @fn
 * @brief handle an incoming JSONRPC2.0 request
 * @param h handler to use
 * @param req request string
 * @retval JSONRPC2.0 response string on success
 * @retval NULL on error
 * @retval errno EINVAL or ENOMEM on error
 */
char *cjrpc2_handle_request(struct cjrpc2_handler *h, const char *req);

/**
 * @fn
 * @brief create JSONRPC2.0 error response item
 * @param code error code to use
 * @param msg message to use (may be NULL)
 * @param data additional data to use (may be NULL)
 * @retval cJSON error response item on success
 * @retval NULL on error
 * @retval errno ENOMEM on error
 */
cJSON *cjrpc2_impl_resp_error(double code, const char *msg, cJSON *data);

/**
 * @fn
 * @brief create an JSONRPC2.0 request
 * @param method method to call
 * @param j_params parameter for the method (NULL for none)
 * @param j_id id for the call (NULL for notification)
 * @retval cJSON request (must be cJSON_Delete()'ed by the caller)
 * @retval NULL on error
 * @retval errno on error
 */
cJSON *cjrpc2_create_request(const char *method, cJSON *j_params, cJSON *j_id);

/**
 * @fn
 * @brief create an JSONRPC2.0 request string
 * @param method method to call
 * @param j_params parameter for the method (NULL for none)
 * @param j_id id for the call (NULL for notification)
 * @retval request string (must be free()'d by the caller)
 * @retval NULL on error
 * @retval errno on error
 */
char *cjrpc2_create_request_str(const char *method, cJSON *j_params, cJSON *j_id);

#endif
