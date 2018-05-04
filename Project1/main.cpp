#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


int main()
{
	auto fileStream = std::make_shared<ostream>();

	pplx::task<void> requestTask = fstream::open_ostream(U("result.html")).then([=](ostream outFile)
	{
		*fileStream = outFile;

		http_client_config conf;
		conf.set_timeout(seconds(5));

		http_client client(U("http://finance.naver.com/"));
		uri_builder builder(U("sise/sise_index.nhn"));
		builder.append_query(U("code"), ("KOSPI"));
		return client.request(methods::GET, builder.to_string());
	}).then([=](http_response response)
	{
		printf("Receive response status code:%u\n", response.status_code());

		return response.body().read_to_end(fileStream->streambuf());
	}).then([=](size_t nVal)
	{
		printf("Size is %u\n", nVal);

		return fileStream->close();
	});

	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
	int cnt = 1;
}