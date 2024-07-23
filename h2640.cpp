#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "byte.hpp"
#include "h2640.hpp"

	namespace H264 
	{
		static bool startcode3(uint8_t *buf)
		{
			/* Поиск в видео-файле кодека h264 стартового кода длиной 3 байта */
			
				return (buf[0] == 0 && buf[1] == 0 && buf[2] == 1) ? true : false;
		}

		static bool startcode4(uint8_t *buf)
		{
			/* Поиск в видео-файле кодека h264 стартового кода длиной 4 байта */

				return (buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) ? true : false;
		}

		int Stream::accessUnitCount()
		{
			if(!isInit()) return -1;

			int count = 0;
			uint8_t header, *pos = m_pos;
			
			while(pos < m_end)
			{
				if(startcode3(pos) || startcode4(pos))
				{
					while(*pos++ != 1);
					header = *pos;
					if(GetPayloadType(header) == 9 || GetPayloadType(header) == 10) count++;
				}

				pos++;
			}
			
			return count;
		}

		AccessUnit & Stream::nextAccessUnit(AccessUnit & au)
		{
			if(!isInit()) return au;
			if(au.isInit()) return au;
			if(cur == m_end) return au;

			uint8_t header;
			uint8_t *pos = nullptr, *end = nullptr;

			while(cur < m_end)
			{
				if(startcode3(cur) || startcode4(cur))
				{
					if(pos == nullptr)
					{
						pos = cur;
						while(*cur++ != 1);
						header = *cur;

						if(GetPayloadType(header) == 9 || GetPayloadType(header) == 10)
							au.setPos(pos);
					}
					else
					{
						end = cur;
						while(*cur++ != 1);
						header = *cur;

						if(GetPayloadType(header) == 9 || GetPayloadType(header) == 10)
						{
							au.setEnd(end);
							cur = end;
							break;
						}
					}
				}

				cur++;
			}

			if(cur == m_end) au.setEnd(m_end);

			return au;					
		}

		AccessUnit & Stream::currentAccessUnit(AccessUnit & au) 
		{
			if(!isInit()) return au;
			if(au.isInit()) return au;

			uint8_t header, *temp, *pos = cur;

			if(pos == m_pos)
			{
				if(!startcode3(pos) || !startcode4(pos)) return au;

				temp = pos;
				while(*pos++ != 1);
				header = *pos;

				if(GetPayloadType(header) != 9) return au;
				au.setPos(temp);

				while(pos < m_end)
				{
					if(startcode3(pos) || startcode4(pos))
					{
						temp = pos;
						while(*pos++ != 1);
						header = *pos;

						if(GetPayloadType(header) == 9 || GetPayloadType(header) == 10)
						{
							au.setEnd(temp);
							break;
						}
						
						pos++;
					}
				}
			}
			else
			{
				au.setEnd(pos);
				pos = (pos == m_end) ? pos - 4 : pos;

				while(pos > m_pos)
				{
					if(startcode3(pos))
					{
						if(pos - 1 >= m_pos && startcode4(pos - 1)) pos--;

						temp = pos;
						while(*pos++ != 1);
						header = *pos;

						if(GetPayloadType(header) == 9 || GetPayloadType(header) == 10)
						{
							au.setPos(temp);
							break;
						}
						else pos = temp;
					}

					pos--;
				}
			}
			
			return au;
		}

		int AccessUnit::nalUnitCount()
		{
			/* Метод подсчитывает количество nalu в потоке h264 */

			if(!isInit()) return -1;

			int count = 0;
			uint8_t *pos = cur;
			
			while(pos < m_end)
			{
				if(startcode3(pos) || startcode4(pos))
				{
					while(*pos++ != 1);
					count++;
				}

				pos++;
			}
			
			return count;
		}

		NalUnit & AccessUnit::nextNalUnit(NalUnit & nu)
		{
			/* Метод парсит очередной nalu из потока h264
				и сохраняет данные о нём в объекте nu      */

			if(!isInit()) return nu;
			if(nu.isInit()) return nu;
			if(cur == m_end) return nu;

			uint8_t *pos = nullptr;

			while(cur < m_end)
			{
				if(startcode3(cur) || startcode4(cur))
				{
					if(pos == nullptr)
					{
						pos = cur;
						nu.setPos(pos);
						while(*cur++ != 1);
					}
					else
					{
						nu.setEnd(cur);
						break;					
					}
				}

				cur++;
			}

			if(cur == m_end) nu.setEnd(cur);

			nu.init();

			return nu;
		}

		NalUnit & AccessUnit::currentNalUnit(NalUnit & nu)
		{
			/* Метод парсит текущий nalu из потока h264
				и сохраняет данные о нём в объекта nu     */

			if(!isInit()) return nu;
			if(nu.isInit()) return nu;

			uint8_t *pos = cur;

			if(pos == m_pos)
			{
				if(!startcode3(pos) || startcode4(pos)) return nu;

				nu.setPos(pos);
				while(*pos++ != 1);

				while(pos < m_end)
				{
					if(startcode3(pos) || startcode4(pos))
					{	
						nu.setEnd(pos);
						break;
					}

					pos++;
				}
			}
			else
			{
				nu.setEnd(pos);
				pos = (pos == m_end) ? pos - 4 : pos;

				while(pos > m_pos)
				{
					if(startcode3(pos))
					{
						if(pos - 1 != m_pos && startcode4(pos - 1)) pos--;
						nu.setPos(pos);
						break;
					}

					pos--;
				}
			}

			nu.init();

			return nu;
		}


		NalUnit::NalUnit() : Byte::Array()
		{
			header = 0;
			startCode = nullptr;
		}


		uint8_t NalUnit::getHeader() const
		{
			return *m_pos;
		}

		uint8_t *NalUnit::getStartCode() const 
		{
			return startCode;
		}

		int NalUnit::getStartCodeLen() const 
		{ 
			return m_pos - startCode;
		}

		uint8_t NalUnit::getForbiddenBit() const
		{
			return header >> 7;
		}

		uint8_t NalUnit::getReferenceIDC() const
		{
			return (header >> 5) & 0x03;
		}

		uint8_t NalUnit::getPayloadType() const
		{
			return header & 0x1F;
		}

		void NalUnit::setPos(uint8_t *pos)
		{
			if(isInit()) return;

			m_pos = pos;
		}

		void NalUnit::setEnd(uint8_t *end)
		{
			if(isInit()) return;

			m_end = end;			
		}

		int NalUnit::payload(char *buf, int bufsize)
		{
			if(!isInit()) return -1;

			int i = 0;
			
			while(i < bufsize && cur < m_end)
			{
				*buf++ = *cur++;
				 i++;
			}

			return i;
		}

		void NalUnit::init()
		{
			if(isInit()) return;

			if(startcode3(m_pos) == false && startcode4(m_pos) == false) return;
			
			startCode = m_pos;

			cur = m_pos;
			
			while(*cur++ != 1);

			m_pos = cur;

			header = *m_pos;

			cur = m_pos;
		}

		void NalUnit::clear()
		{
			header = 0;
			startCode = nullptr;
			m_pos = nullptr;
			m_end = nullptr;
		}

		bool NalUnit::isInit() const
		{
			if(!Byte::Array::isInit()) return false;
			if(startCode == nullptr) return false;

			return true;
		}

#if 0

		int Packer::pack(AccessUnit & au)
		{
			if(!isInit()) return -1;
			if(!au.isInit()) return -1;

			H264NalUnit nu;

			uint32_t payloadSize = 0;
			uint8_t *naluPos, nri = 0;
			uint8_t  fuaIndicator, fuaHeader, stapaHeader = 0;
			int ret, len, packetNumber, otherSize, n = 0, naluCount = 0;

			if(cur < m_end)
			{
				if(mode == UnknowMode)
				{
					mode = StapaMode;
					naluCount = au.getNaluCount();
				}

				if(mode == FuaMode)
				{
					if(au_cur != au_end)
					{
						*cur++ = H264GetFuaIndicator(*au_pos);
						*cur++ = H264GetFuaHeader(*au_pos);

						if(au_cur == au_pos)
						{
							*(m_cur - 1) = H264SetFuaStartBit(1, *(m_cur - 1));
							 au_cur++;
						}

						while(m_cur != m_end && au_cur != au_end) 
							*m_cur++ = *au_cur++;

						if(au_cur == au_end)
						{
							*(m_pos + 1) = H264SetFuaEndBit(1, *(m_pos + 1));

							mode = UnknowMode;
						}
						
						len = getBufLen();
						m_cur = m_pos;
						return len;
					}
				}

				if(mode == SingleMode)
				{
					while(au_cur != au_end) *m_cur++ = *au_cur++;
					mode = UnknowMode;
					len = getBufLen();
					m_cur = m_pos;
					return len;	
				}
				
				if(mode == StapaMode)
				{
					while(n < naluCount)
					{
						au.nextNalUnit(nu);

						// Обработка случая, когда nal unit является
						// концом потока

						if(nu.getPayloadType() == 10)
						{
							ret = nu.payload(cur, getFree());
							cur += ret;
							mode = UnknowMode;
							len = getBufLen();
							cur = m_pos;
							return len;						
						}

						// Обработка случая, когда размер nal unit
						// либо больше размера самого буфера, либо
						// больше, чем размер свободного места
						// в буфере

						if(nu.getSize() > getSize() || nu.getSize() > getFree())
						{
							mode = nu.getSize() > getSize() ? FuaMode : SingleMode;
						  *m_pos = H264SetReferenceIDC(nri, *m_pos);
							len = getBufLen();
							cur = m_pos;
							return len;
						}

						payloadSize = nu.getSize();
						nri = nu.getReferenceIDC() > nri ? nu.getReferenceIDC() : nri;
						
						if(cur == m_pos)
						{
							stapaHeader = H264GetStapaHeader(nu.getHeader());
							*cur++ = stapaHeader;
						}

						*cur++ = (uint8_t)((payloadSize >> 8) & 0x000000FF);
						*cur++ = (uint8_t)(payloadSize & 0x000000FF);

						ret = nu.payload(cur, getFree());
						cur += ret;
					}

					// Обработка случая, когда все nal unit, которые 
					// содержатся в access unit, помещаются в буфер

					*m_pos = H264SetReferenceIDC(nri, *m_pos);
					 len = getBufLen();
					 cur = m_pos;
					 mode = UnknowMode;
					 return len;
				}
			}

			return 0;
		}

#endif

		uint8_t GetForbiddenBit(uint8_t header)
		{	
			return header >> 7;	
		}

		uint8_t GetReferenceIDC(uint8_t header)
		{
			return (header >> 5) & 0x03;
		}

		uint8_t GetPayloadType(uint8_t header)
		{
			return header & 0x1F;
		}

		uint8_t SetForbiddenBit(uint8_t forbiddenBit, uint8_t header)
		{
			header = (forbiddenBit << 7) | (header & 0x7F);

			return header;
		}

		uint8_t SetReferenceIDC(uint8_t referenceIdc, uint8_t header)
		{
			header = ((referenceIdc << 5) & 0x60) | (header & 0x9F);

			return header;
		}

		uint8_t SetPayloadType(uint8_t payloadType, uint8_t header)
		{
			header = (payloadType & 0x1F) | (header & 0xE0);

			return header;
		}

		uint8_t GetStapaHeader(uint8_t header)
		{
			header = (header & 0xE0) | 24;

			return header;
		}

		uint8_t GetFuaIndicator(uint8_t header)
		{
			header = (header & 0xE0) | 28;

			return header;
		}

		uint8_t GetFuaHeader(uint8_t header)
		{
			header = header & 0x1F;

			return header;
		}

		uint8_t GetFuaStartBit(uint8_t fuaHeader)
		{
			uint8_t startBit = (fuaHeader & 0x80) >> 7;

			return startBit;
		}

		uint8_t GetFuaEndBit(uint8_t fuaHeader)
		{
			uint8_t endBit = (fuaHeader & 0x40) >> 6;

			return endBit;
		}

		uint8_t SetFuaStartBit(uint8_t startBit, uint8_t fuaHeader)
		{
			fuaHeader = (fuaHeader & 0x1F) | (startBit << 7);
			
			return fuaHeader;
		}

		uint8_t SetFuaEndBit(uint8_t endtBit, uint8_t fuaHeader)
		{
			fuaHeader = (fuaHeader & 0x1F) | (endtBit << 6);

			return fuaHeader;
		}

		uint8_t ResetFuaBits(uint8_t fuaHeader)
		{
			return fuaHeader & 0x1F;
		}

	} // namespace H264

#if 0

	int H264Parser::loadFile(const char *filename)
	{
		/* Загрузка потока видео-файла h264 в буфер */
		
		if(filename == nullptr)
		{
			printf("H264loadFile(): invalid argument");
			return -1;
		}
		
		int fd, bufsize;
		struct stat file_info;

		fd = open(filename, O_RDONLY);
		
		if(fd < 0)
		{
			perror("open()");
			return -1;
		}
		
		if(fstat(fd, &file_info) < 0)
		{
			perror("fstat()");
			return -1;
		}
		
		bufsize = (int) file_info.st_size;	
		buf_pos = (uint8_t *) calloc(bufsize, sizeof(uint8_t));

		if(buf_pos == nullptr)
		{
			printf("H264::loadFile(): allocation error");
			return -1;
		}

		if(read(fd, buf_pos, bufsize) < 0)
		{
			perror("read");
			return -1;
		}

		buf_pos[bufsize] = '\0';
		buf_end = buf_pos + bufsize;

		return bufsize;
	}

	H264Packer::H264Packer()
	{
		mode = UnknowMode;

		m_pos = nullptr;
		m_cur = nullptr;
		m_end = nullptr;

		au_pos = nullptr;
		au_cur = nullptr;
		au_end = nullptr;
	}

	void H264Packer::setPos(uint8_t *pos)
	{
		if(m_cur != nullptr) return;

		m_pos = pos;
	}

	void H264Packer::setEnd(uint8_t *end)
	{
		if(m_cur != nullptr) return;

		m_end = end;
	}

	void  H264Packer::setBuf(uint8_t *buf, int bufsize)
	{
		if(m_cur != nullptr) return;

		m_pos = buf;
		m_end = m_pos + bufsize;		
	}

	int H264Packer::pack(H264Parser & parser)
	{
		if(m_cur == nullptr) return -1;

		H264NalUnit nu;
		H264AccessUnit au;

		uint32_t payloadSize = 0;
		uint8_t *naluPos, nri = 0;
		uint8_t  fuaIndicator, fuaHeader, stapaHeader = 0;

		int len, packetNumber, otherSize;

		if(m_cur < m_end)
		{
			if(mode == UnknowMode)
			{
				mode = StapaMode;
				parser.parseAccessUnit(au);
				if(au.getSize() == 0) return 0;
				m_cur = m_pos;
				au_end = au.getEnd();
			}

			if(mode == FuaMode)
			{
				if(au_cur != au_end)
				{
					*m_cur++ = H264GetFuaIndicator(*au_pos);
					*m_cur++ = H264GetFuaHeader(*au_pos);

					if(au_cur == au_pos)
					{
						*(m_cur - 1) = H264SetFuaStartBit(1, *(m_cur - 1));
						 au_cur++;
					}

					while(m_cur != m_end && au_cur != au_end) 
						*m_cur++ = *au_cur++;

					if(au_cur == au_end)
					{
						*(m_pos + 1) = H264SetFuaEndBit(1, *(m_pos + 1));

						mode = UnknowMode;
					}
					
					len = getBufLen();
					m_cur = m_pos;
					return len;
				}
			}

			if(mode == SingleMode)
			{
				while(au_cur != au_end) *m_cur++ = *au_cur++;
				mode = UnknowMode;
				len = getBufLen();
				m_cur = m_pos;
				return len;	
			}
			
			if(mode == StapaMode)
			{
				while(au_cur != au_end)
				{
					au.parseNalUnit(nu);
					au_pos = nu.getPos();
					au_cur = au_pos;

					// Обработка случая, когда nal unit является
					// концом потока

					if(nu.getPayloadType() == 10)
					{
						while(au_cur != au_end) *m_cur++ = *au_cur++;
						mode = UnknowMode;
						len = getBufLen();
						m_cur = m_pos;
						return len;						
					}

					// Обработка случая, когда размер nal unit
					// либо больше размера самого буфера, либо
					// больше, чем размер свободного места
					// в буфере

					if(nu.getSize() > getBufSize() - getBufLen() - 2)
					{
						mode = nu.getSize() > getBufSize() ? FuaMode : SingleMode;
					  *m_pos = H264SetReferenceIDC(nri, *m_pos);
						len = getBufLen();
						m_cur = m_pos;
						return len;
					}

					payloadSize = nu.getSize();
					nri = nu.getReferenceIDC() > nri ? nu.getReferenceIDC() : nri;
					
					if(m_cur == m_pos)
					{
						stapaHeader = H264GetStapaHeader(nu.getHeader());
						*m_cur++ = stapaHeader;
					}

					*m_cur++ = (uint8_t)((payloadSize >> 8) & 0x000000FF);
					*m_cur++ = (uint8_t)(payloadSize & 0x000000FF);

					while(au_cur != nu.getEnd()) *m_cur++ = *au_cur++;
				}

				// Обработка случая, когда все nal unit, которые 
				// содержатся в access unit, помещаются в буфер

				*m_pos = H264SetReferenceIDC(nri, *m_pos);
				 len = getBufLen();
				 m_cur = m_pos;
				 mode = UnknowMode;
				 return len;
			}
		}

		return 0;
	}

	void H264Packer::init()
	{
		if(m_pos == nullptr || m_end == nullptr) return;

		m_cur = m_pos;
	}

	void H264Packer::reset()
	{
		mode = UnknowMode;
		m_cur = m_pos;

		au_pos = nullptr;
		au_cur = nullptr;
		au_end = nullptr;
	}

	void H264Packer::clear()
	{
		mode = UnknowMode;

		m_pos = nullptr;
		m_cur = nullptr;
		m_end = nullptr;

		au_pos = nullptr;
		au_cur = nullptr;
		au_end = nullptr;
	}

	H264Unpacker::H264Unpacker()
	{
		m_pos = nullptr;
		m_cur = nullptr;
		m_end = nullptr;		
	}

	void H264Unpacker::setPos(uint8_t *pos)
	{
		if(m_cur != nullptr) return;
		m_pos = pos;
	}

	void H264Unpacker::setEnd(uint8_t *end)
	{
		if(m_cur != nullptr) return;
		m_end = end;		
	}

	void H264Unpacker::setBuf(uint8_t *buf, int bufsize)
	{
		if(m_cur != nullptr) return;

		m_pos = buf;
		m_end = buf + bufsize;
	}

	int H264Unpacker::unpack(uint8_t *buf, int bufsize)
	{
		if(m_pos == nullptr || m_end == nullptr || m_cur == nullptr) return -1;
		if(m_cur == m_end) return 0;

		int len;
		uint32_t payloadSize;
		uint8_t naluHeader = 0, payloadType;
		uint8_t *pos, *cur, *end, *temp, *last;

		pos = buf;
		cur = pos;
		end = pos + bufsize;
		last = m_cur;

		if(H264GetPayloadType(*cur) == 24)
		{
			cur++;
			while(cur < end)
			{
				payloadSize = 0;
				payloadSize = (payloadSize | *cur) << 8;
				cur++;
				payloadSize |= *cur;
				cur++;

				payloadType = H264GetPayloadType(*cur);
				if(payloadType == 9 || payloadType == 8 || payloadType == 7)
				{
					if(getBufFree() < 4) return -1;

					*m_cur++ = 0;
					*m_cur++ = 0;
					*m_cur++ = 0;
					*m_cur++ = 1;
				}
				else
				{
					if(getBufFree() < 3) return -1;

					*m_cur++ = 0;
					*m_cur++ = 0;
					*m_cur++ = 1;				
				}

				temp = cur + payloadSize;

				if(getBufFree() < temp - cur) return -1;

				while(cur < temp) *m_cur++ = *cur++;
			}

			return m_cur - last;
		}

		if(H264GetPayloadType(*cur) == 28)
		{
			if(getBufFree() < bufsize) return -1;
			
			naluHeader = H264SetForbiddenBit(H264GetForbiddenBit(*cur), naluHeader);
			naluHeader = H264SetReferenceIDC(H264GetReferenceIDC(*cur), naluHeader);

			cur++;

			if(H264GetFuaStartBit(*cur))
			{
				if(getBufFree() < 3) return -1;

				naluHeader = H264SetPayloadType(H264GetPayloadType(*cur), naluHeader);

				*m_cur++ = 0;
				*m_cur++ = 0;
				*m_cur++ = 1;
				*m_cur++ = naluHeader;
			}

			cur++;
	
			while(cur < end) *m_cur++ = *cur++;

			return m_cur - last;
		}

		payloadType = H264GetPayloadType(*cur);

		if(payloadType == 10)
		{
			if(getBufFree() < 5) return -1;

			*m_cur++ = 0;
			*m_cur++ = 0;
			*m_cur++ = 0;
			*m_cur++ = 1;
			*m_cur++ = *cur;

			return m_cur - last;
		}
		else
		{
			if(getBufFree() < 3) return -1;

			*m_cur++ = 0;
			*m_cur++ = 0;
			*m_cur++ = 1;			
		}

		if(getBufFree() < bufsize) return -1;

		while(cur < end) *m_cur++ = *cur++;

		return m_cur - last;
	}

	void H264Unpacker::init()
	{
		if(m_cur != nullptr) return;
		m_cur = m_pos;
	}

	void H264Unpacker::reset()
	{
		m_cur = m_pos;
	}

	void H264Unpacker::clear()
	{
		m_pos = nullptr;
		m_cur = nullptr;
		m_end = nullptr;		
	}

#endif
