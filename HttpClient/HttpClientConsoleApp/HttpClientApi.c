#include "HttpClient.h"
#include "stdio.h"
#include "stdlib.h"
#include "HttpClientApi.h"
#include "HTTPClientCommon.h"


static char serverUrl[250];
void HTTP_CLIENT_SetServerUrl(char *url)
{
	strcpy(serverUrl, url);
}

int sendHTTP(HTTP_VERB verb, 
			 char* api, 
			 void *pData, 
			 unsigned int nDataLength, 
			 char* pDataOut, 
			 unsigned int nDataOutLength, 
			 unsigned int Timeout, 
			 int isJson)
{

	INT32                   nRetCode = 0;
	UINT32                  nSize, nTotal = 0;
	CHAR                    Uri[HC_STR_LENGTH];
	HTTP_SESSION_HANDLE     pHTTP;
	const struct json_token *tok = NULL;

	do
	{
		strcpy((char *)&Uri, serverUrl);
		strncpy(&Uri[strlen(serverUrl)], api, (HC_STR_LENGTH - strlen(serverUrl)));

		// Open the HTTP request handle
		pHTTP = HTTPClientOpenRequest(HTTP_CLIENT_FLAG_KEEP_ALIVE | HTTP_CLIENT_FLAG_SEND_CHUNKED | HTTP_CLIENT_FLAG_NO_CACHE | isJson << 3);

		// Set the Verb
		if ((nRetCode = HTTPClientSetVerb(pHTTP, verb)) != HTTP_CLIENT_SUCCESS)
		{
			printf("HTTPClientSetVerb failed");
			break;
		}


		// Send a request
		if ((nRetCode = HTTPClientSendRequest(pHTTP, Uri, pData, nDataLength, TRUE, Timeout, 0)) != HTTP_CLIENT_SUCCESS)
		{
			printf("HTTPClientSendRequest failed with ret code %d", nRetCode);
			break;
		}

		//        if(socketTimeoutReported == true)
		//        {
		//        	PINFO ("HTTP socket timeout resolved");
		//        	socketTimeoutReported = false;
		//        }

		// Retrieve the the headers and analyze them
		if ((nRetCode = HTTPClientRecvResponse(pHTTP, Timeout)) != HTTP_CLIENT_SUCCESS)
		{
			printf("HTTPClientRecvResponse failed with ret code %d", nRetCode);
			break;
		}

		// Get the data until we get an error or end of stream code
		// printf("Each dot represents %d bytes:\n",HTTP_BUFFER_SIZE );
		while (nDataOutLength && (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS))
		{
			// Get the data
			nRetCode = HTTPClientReadData(pHTTP, (pDataOut + nTotal), nDataOutLength, 0, &nSize);
			nTotal += nSize;
			nDataOutLength = nDataOutLength - nSize;
		}
		printf("recieve: %s", pDataOut);
		HTTPClientCloseRequest(&pHTTP);

	} while (0);

	//struct json_token   tokens[HC_STR_LENGTH];
	//(pDataOut, nDataOutLength, tokens, sizeof(tokens));
	//tok = find_json_token(tokens, "status");

	//if (tok && !strcmp("OK", tok->ptr))
	{
		printf("server has returned an error");
		nRetCode = HTTP_CLIENT_ERROR_SERVER_ERROR;
	}

	return (nRetCode == HTTP_CLIENT_EOS) ? HTTP_CLIENT_SUCCESS : nRetCode;
};

