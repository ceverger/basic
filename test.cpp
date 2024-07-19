#include <cstdio>
#include "h264.hpp"

	int main(int argc, char *argv[])
	{
		if(argc < 2)
		{
			printf("main::invalid_arguments\n");

			return 1;
		}

		H264::NalUnit nu;
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

		naluCount = stream.nalUnitCount();

		if(!stream.parser(parser).isInit())
		{
			printf("parser is not init\n");
			return 1;				
		}

		while(n < naluCount)
		{
			if(!parser.nextNalUnit(nu).isInit())
			{
				printf("nalu is not init\n");
				return 1;
			}

			printf(
						"number = %d, startCode_len = %d, forbiddenBit = %d, referenceIDC = %d, payloadType = %d, payloadLen = %d\n",
						 n,
						 nu.getStartCodeLen(),
						 nu.getForbiddenBit(),
						 nu.getReferenceIDC(),
						 nu.getPayloadType(),
						 nu.getSize()
					);

			n++;
			nu.clear();
		}

		printf("nalu_count = %d\n", n);

		return 0;
	}
