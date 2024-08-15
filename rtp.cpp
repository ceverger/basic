#include <cstdio>
#include "rtp0.hpp"

	namespace Rtp
	{
		Header::Header()
		{
			// Конструктор класса по умолчанию
	
				pos = nullptr;
				end = nullptr;
		}

		int Header::getSize() const
		{
			// Получение размера заголовка

				if(pos == nullptr || end == nullptr) return 0;

				return end - pos;
		}

		uint8_t Header::getV() const
		{
			/* Получение значения версии протокола */

				uint8_t v = (pos[0] >> 6) & 0x03;
				
				return v;			
		}

		uint8_t Header::getP() const
		{	
			/* Получение значения дополнения заголовка RTP */

				uint8_t p = (pos[0] >> 5) & 0x01;

				return p;		
		}

		uint8_t Header::getX() const
		{
			/* Получение значения расширения заголовка RTP */

				uint8_t x = (pos[0] >> 4) & 0x01;

				return x;
		}

		uint8_t Header::getCC() const
		{
			/* Получение количества источников синхронизации */

				uint8_t cc = pos[0] & 0x0F;

				return cc;
		}

		uint8_t Header::getM() const
		{	
			/* Получение значения бита маркера заголовка RTP */

				uint8_t m = (pos[1] >> 7)  & 0x01;

				return m;	
		}

		uint8_t Header::getPT() const
		{
			/* Получение типа полезной нагрузки заголовка RTP */

				uint8_t pt = pos[1] & 0x7F;

				return pt;		
		}

		uint16_t Header::getSEQ() const
		{
			/* Получение значения номера RTP-пакета */

				uint16_t seq = pos[2];

				seq = (seq << 8) | pos[3];

				return seq;
		}

		uint32_t Header::getTS() const
		{
			/* Получение временной метки RTP-пакета */

				uint32_t ts = 0;
				uint32_t temp = pos[4];

				ts = (temp << 24);

				temp = pos[5];

				ts |= temp << 16;

				temp = pos[6];

				ts |= temp << 8;

				temp = pos[7];

				ts |= temp;

				return ts;
		}

		uint32_t Header::getSSRC() const
		{
			/* Получение номера источника синхронизации */

				uint32_t ssrc = 0;
				uint32_t temp = pos[8];

				ssrc = (temp << 24);

				temp = pos[9];

				ssrc |= temp << 16;

				temp = pos[10];

				ssrc |= temp << 8;

				temp = pos[11];

				ssrc |= temp;

				return ssrc;
		}

		void Header::setV(uint8_t v)
		{
			/* Установка версии RTP-протокола */

				pos[0] = ((v << 6) & 0xC0) | (pos[0] & 0x3F);
		}

		void Header::setP(uint8_t p)
		{
			/* Установка значения дополнения заголовка RTP */

				pos[0] = ((p << 5) & 0x20) | (pos[0] & 0xDF);
		}

		void Header::setX(uint8_t x)
		{
			/* Установка значения расширения заголовка RTP  */

				pos[0] = ((x << 4) & 0x10) | (pos[0] & 0xEF);
		}

		void Header::setCC(uint8_t cc)
		{
			/* Установка значения количества источников синхронизации RTP-потока */

				pos[0] = (cc & 0x0F) | (pos[0] & 0xF0);
		}

		void Header::setM(uint8_t m)
		{
			/* Установка значения бита маркера заголовка RTP */

				pos[1] = ((m << 7) & 0x80) | (pos[1] & 0x7F);
		}

		void Header::setPT(uint8_t pt)
		{
			/* Установка значения типа полезной нагрузки заголовка RTP */

				pos[1] = (pt & 0x7F) | (pos[1] & 0x80);
		}

		void Header::setSEQ(uint16_t seq)
		{
			/* Установка значения номера RTP-пакета */

				pos[2] = (seq >> 8) & 0x00FF;
				pos[3] = seq & 0x00FF;
		}

		void Header::setTS(uint32_t ts)
		{
			/* Установка значения временной метки RTP-пакета */

				pos[4] = (ts >> 24) & 0x000000FF;
				pos[5] = (ts >> 16) & 0x000000FF;
				pos[6] = (ts >> 8) & 0x000000FF;
				pos[7] =  ts & 0x000000FF;
		}

		void Header::setSSRC(uint32_t ssrc)
		{
			/* Установка значения номера источника синхронизации */

				pos[8]  = (ssrc >> 24) & 0x000000FF;
				pos[9]  = (ssrc >> 16) & 0x000000FF;
				pos[10] = (ssrc >>  8) & 0x000000FF;
				pos[11] =  ssrc & 0x000000FF;
		}

		void Header::setHeader(
										uint8_t v,
										uint8_t p,
										uint8_t x,
										uint8_t cc,
										uint8_t m,
										uint8_t pt,
										uint16_t seq,
										uint32_t ts,
										uint32_t ssrc
									 )
		{

			setV(v);
			setP(p);
			setX(x);
			setM(m);
			setPT(pt);
			setSEQ(seq);
			setTS(ts);
			setSSRC(ssrc);
		}

		void Header::show() const
		{
			if(pos == nullptr || end == nullptr) return;

			printf(
					 "v = %d, p = %d, x = %d, cc = %d, m = %d, pt = %d, seq = %d, ts = %d, ssrc = %d\n",
					  getV(),
					  getP(),
					  getX(),
					  getCC(),
					  getM(),
					  getPT(),
					  getSEQ(),
					  getTS(),
					  getSSRC()
					);			
		}

		Packet::Packet() : Buffer()
		{
			// Конструктор класса по умолчанию
		}

		Packet::Packet(uint8_t *buf, int bufsize) : Buffer(buf, bufsize)
		{
			// Пользовательский конструктор			
		}

		Packet::Packet(const Array & array) : Buffer(array)
		{
			// Конструктор копирования
		}

		int Packet::getHeaderSize() const
		{
			if(!isInit()) return 0;
			if(h_end == nullptr) return 0;

			return h_end - m_pos;
		}

		Header Packet::getHeader() const
		{
			Header header;

			if(!isInit()) return header;
			if(h_end == nullptr) return header;
			if(getSize() < Header::HeaderSize) return header;

			header.pos = m_pos;
			header.end = h_end;

			return header;
		}

		Header & Packet::getHeader(Header & header)
		{
			if(!isInit()) return header;
			if(h_end == nullptr) return header;
			if(getSize() < Header::HeaderSize) return header;

			header.pos = m_pos;
			header.end = h_end;

			return header;
		}

		int Packet::getPayloadLen() const
		{
			if(h_end == nullptr) return -1;

			return cur - h_end; 
		}

		int Packet::getPayload(uint8_t *buf, int bufsize)
		{
			if(h_end == nullptr) return -1;
			if(!isInit()) return -1;
			if(getSize() < Header::HeaderSize) return -1;

			int i = 0;
			uint8_t *pos = h_end;
			uint8_t *end = buf + bufsize;

			while(pos < cur && buf < end)
			{
				*buf++ = *pos++;
				 i++;
			}

			return i;								
		}

		void Packet::setHeaderSize(int size)
		{
			if(!isInit()) return;
			if(getSize() < Header::HeaderSize) return;
			if(size < Header::HeaderSize) return;

			h_end = m_pos + size;
		}

		void Packet::setPayload(uint8_t *buf, int len)
		{
			if(!isInit()) return;
			if(h_end == nullptr) return;
			if(getSize() < Header::HeaderSize) return;

			uint8_t *end = buf + len;

			cur = h_end;

			while(cur < m_end && buf < end)
			{
				*cur++ = *buf++;
			}
		}

	}; // namespace Rtp
