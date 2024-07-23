#include <stdio.h>
#include "files.hpp"
#include "byte.hpp"
#include "h2640.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cout << "Invalid arguments!" << std::endl;
		return 1;
	}

	uint8_t *buf;
	int ret, i = 0, j = 0;
	int naluCount, auCount;

	H264::NalUnit nu;
	H264::AccessUnit au;
	H264::Stream stream;

	ret = fileLoad(argv[1], (void **) &buf);
	if(ret < 0) return 1;

	stream.setBuf(buf, ret);

	std::cout << stream.getSize() <<  std::endl;
	auCount = stream.accessUnitCount();
	std::cout << auCount << std::endl;

	while(j < auCount)
	{
		stream.nextAccessUnit(au);
		std::cout << j << ' ' << au.getSize() << std::endl;
		naluCount = au.nalUnitCount();

		while(i < naluCount)
		{
			au.nextNalUnit(nu);
			std::cout << "   " << i << ' ' << nu.getSize() << std::endl;
			nu.clear();
			i++;
		}

		i = 0;
		au.clear();
		j++;
	}

	return 0;
}
