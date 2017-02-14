#ifndef HTTP_CLIENT_API_H
#define HTTP_CLIENT_API_H


#define HC_STR_LENGTH 256
#define HC_HTTP_TIMEOUT  5
#define HC_BUFFER_SIZE   1024
#define MD5_DIGEST_LENGTH   16
#define HC_FILE_SIZE        50000




int sendHTTP(HTTP_VERB verb, char* api, void *pData,
			 unsigned int nDataLength,
			 char* pDataOut,
			 unsigned int nDataOutLength,
			 unsigned int Timeout,
			 int isJson);


#endif 