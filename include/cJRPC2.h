/* SPDX-License-Identifier: MIT */
#ifndef CJRPC2__H
#define CJRPC2__H

#include <stdbool.h>

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

enum cjrpc2_param_status {
	PARAM_OK,	  /**< parameter is OK */
	PARAM_MISSING,	  /**< parameter was missing */
	PARAM_WRONG_TYPE, /**< parameter was present but had a wrong type */
	PARAM_NUM_NOINT,  /**< parameter value is a number, but not an integer as requested */
	PARAM_OO_RANGE,	  /**< parameter was present but value was out of range */
	PARAM_NOMEM	  /**< failed to allocate memory for the parameter value */
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
 * @brief create a new cJRPC2 handler with multiple cjrpc2_method arrays
 * @param count number of cjrpc2_method arrays to follow
 * @param ... count number of NULL terminated array of methods to register
 * @retval new handler instance on success
 * @retval NULL on error
 * @retval errno EINVAL or ENOMEN on error
 */
struct cjrpc2_handler *cjrpc2_new_handler_m(size_t count, ...);

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
 * @retval JSONRPC2.0 response string on success (must be free()' by the caller)
 * @retval emtpy string on notification request (must be free()' by the caller)
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

/**
 * @fn
 * @brief get a double parameter by its name from an cJRPC2 params list
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the double value at (allocation & freeing must be done by the
 * caller)
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_double(const cJSON *params, const char *name,
						 double *value);

/**
 * @fn
 * @brief get an double parameter by its name from an cJRPC2 params list and check if it fits in a
 * given range
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the double value at (allocation & freeing must be done by the
 * caller)
 * @param min minimal allowed value
 * @param max maximal allowed value
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_double_range(const cJSON *params, const char *name,
						       double *value, const double min,
						       const double max);

/**
 * @fn
 * @brief get an integer parameter by its name from an cJRPC2 params list
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the integer value at (allocation & freeing must be done by the
 * caller)
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_int(const cJSON *params, const char *name, int *value);

/**
 * @fn
 * @brief get an integer parameter by its name from an cJRPC2 params list and check if it fits in a
 * given range
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the integer value at (allocation & freeing must be done by the
 * caller)
 * @param min minimal allowed value
 * @param max maximal allowed value
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_int_range(const cJSON *params, const char *name,
						    int *value, const int min, const int max);

/**
 * @fn
 * @brief get an boolean parameter by its name from an cJRPC2 params list
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the bool value at (allocation & freeing must be done by the caller)
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_bool(const cJSON *params, const char *name, bool *value);

/**
 * @fn
 * @brief get a string parameter by its name from an cJRPC2 params list
 * @param params parameter list
 * @param name parameter to get
 * @param value pointer to store the NULL terminated char array at (allocated by the function,
 * freeing must be done by the caller)
 * @retval cjrpc2_param_status
 */
enum cjrpc2_param_status cjrpc2_get_param_string(const cJSON *params, const char *name,
						 char **value);

#endif
