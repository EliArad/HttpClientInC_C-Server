// HttpClientConsoleApp.cpp : Defines the entry point for the console application.
//
#include "HTTPClientCommon.h"
#include "HttpClientApi.h"
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS			// stop complaining about deprecated functions
#include "jWrite.h"

int main(int argc, char* argv[])
{

	 
	char response[256] = "";
	char api[256] = "api/result/";
	int ret_code = HTTP_CLIENT_SUCCESS;

	char buffer[1024];
	unsigned int buflen = 1024;
	int err;

	jwOpen(buffer, buflen, JW_OBJECT, JW_PRETTY);		// start root object

	jwObj_string("key", "value");				// add object key:value pairs
	jwObj_int("int", 1);
	jwObj_double("double", 1.234);
	jwObj_null("nullThing");
	jwObj_bool("bool", 1);
	jwObj_array("EmptyArray");
	// empty array
	jwEnd();
 
	 	 
	ret_code = sendHTTP(VerbPost, api, buffer, strlen(buffer), response, HC_BUFFER_SIZE, HC_HTTP_TIMEOUT, 1);
	if (ret_code != HTTP_CLIENT_SUCCESS)
	{
		printf("http action retruned with %d", ret_code);
	} 
	else
	{
#if 0 
		struct json_token   tokens[HC_STR_LENGTH];
		parse_json(response, HC_BUFFER_SIZE, tokens, sizeof(tokens));
		const struct json_token *tok = find_json_token(tokens, "result._id");
		if (tok)
		{
			//strncpy(cookingResultId, tok->ptr, tok->len);
		}
		else  {
			printf("no cooking result id");
		}
#endif 
	}
 
	return 1;
}

