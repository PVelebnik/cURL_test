#include <iostream>
#include <string>

#define  CURL_STATICLIB
#include "curl/curl.h"

#ifdef _DEBUG
#	pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#	pragma comment  (lib, "curl/libcurl_a.lib")
#endif

struct PostData
{
	const char *ptr;
	size_t size;
};

size_t read_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	PostData *post_data = (PostData*)userp;

	size_t byte_len = size * nmemb;
	if (post_data->size < byte_len)
	{
		byte_len = post_data->size;
	}
	memcpy(ptr, post_data->ptr, byte_len);
	post_data->ptr += byte_len;
	post_data->size -= byte_len;
	return byte_len;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	FILE *f = (FILE*)userp;
	return fwrite(ptr, size, nmemb, f);
}

void wmain(int argc, wchar_t* argv[])
{
	CURL *curl = curl_easy_init();
	if (!curl) 
	{
		fprintf(stderr, "curl_easy_init() failed\n");
		return;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://posttestserver.com/post.php");
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	// Set callback on sending data
	const char *message = "È tenersi per mano andare lontano, la felicità\n"
		"E il tuo sguardo innocente in mezzo alla gente, la felicità\n"
		"È restare vicini come bambini, la felicità\n"
		"Felicità\n";
	PostData post_data;
	post_data.ptr = message;
	post_data.size = strlen(message);
	curl_easy_setopt(curl, CURLOPT_READDATA, &post_data);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(message));

	// Set callback on receiving data
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
}