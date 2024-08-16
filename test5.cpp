#include <cstdio>
#include "rtsp.hpp"

int main(int argc, char *argv[])
{
	int ret, bufsize = 2048;
	uint8_t buf[bufsize], result[bufsize];
	Rtsp::Request::Generator requestGenerator;

	requestGenerator.setBuf(buf, bufsize);
	requestGenerator.generate("OPTIONS", "rtsp://192.168.5.114:8554/test.264", 1.0);

	ret = requestGenerator.getData(result, bufsize);
	result[ret] = '\0';
	printf("%s", result);
	requestGenerator.zero();

	requestGenerator.setMethod("OPTIONS");
	ret = requestGenerator.getData(result, bufsize);
	result[ret] = '\0';
	printf("%s\n", result);

	requestGenerator.setURI("rtsp://192.168.5.114:8554/test.264");
	ret = requestGenerator.getData(result, bufsize);
	result[ret] = '\0';
	printf("%s\n", result);

	return 0;
}
