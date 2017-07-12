#include <iostream>
#include <string>

#define  CURL_STATICLIB
#include "curl/curl.h"

#ifdef _DEBUG
#	pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#	pragma comment  (lib, "curl/libcurl_a.lib")
#endif

static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data)
{
	if (writer_data == NULL)
		return 0;
	writer_data->append(data, size*nmemb);
	return size*nmemb;
}

int wmain(int argc, wchar_t* argv[])
{
	std::string content;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = nullptr;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://curl.haxx.se/");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	std::cout << content;

	std::cout << "\nPress any key to exit!";
	std::cin.get();
	return EXIT_SUCCESS;
}