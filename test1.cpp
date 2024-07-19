#include <cstdio>
#include "h264.hpp"

	int main(int argc, char *argv[])
	{
		if(argc < 2)
		{
			printf("main::invalid_arguments\n");

			return 1;
		}

		H264::AccessUnit au;
		H264::Parser parser;
		H264::Stream stream;

		int n = 0, naluCount, auCount;

		stream.load(argv[1]);

		if(!stream.isInit())
		{
			printf("stream is not init\n");
			return 1;			
		}

		printf("stream_size = %d\n", stream.getSize());

		auCount = stream.accessUnitCount();

		if(!stream.parser(parser).isInit())
		{
			printf("parser is not init\n");
			return 1;
		}

		while(n < auCount)
		{
			if(!parser.nextAccessUnit(au).isInit())
			{
				printf("au is not init\n");
				return 1;
			}

			printf("number = %d, au_size = %d\n", n, au.getSize());

			au.clear();

			n++;
		}

		printf("au_count = %d\n", n);

		return 0;
	}
