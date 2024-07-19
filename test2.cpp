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
		H264::AccessUnit au;
		H264::Parser parser;
		H264::Stream stream;

		int i = 0, j = 0, naluCount, auCount;

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

		while(j < auCount)
		{
			parser.nextAccessUnit(au);

			naluCount = au.naluCount();
			printf("au_number = %d, au_size = %d, nalu_count = %d\n", j, au.getSize(), naluCount);

			while(i < naluCount)
			{
				au.nextNalUnit(nu);

				printf(
						"nalu_number = %d, startCode_len = %d, forbiddenBit = %d, referenceIDC = %d, payloadType = %d, payloadLen = %d\n",
						 i,
						 nu.getStartCodeLen(),
						 nu.getForbiddenBit(),
						 nu.getReferenceIDC(),
						 nu.getPayloadType(),
						 nu.getSize()
						);

				nu.clear();		
				i++;
			}

			i = 0;

			au.clear();

			j++;
		}

		return 0;
	}
