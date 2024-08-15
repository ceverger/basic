#include <stdint.h>
#include "byte.hpp"

#ifndef RTP_HPP
#define RTP_HPP

	namespace Rtp 
	{
		/* Пространство имён модуля для работы с RTP-потоком */

		class Header
		{
			/* Данный класс предназначен для работы с заголовком RTP-пакета */

			friend class Packet;

			public:

			// Минимальный размер заголовка rtp-пакета

				enum { HeaderSize = 12 };

			private:

				uint8_t *pos; // Начальная позиция буфера заголовка
				uint8_t *end; // Конечная позиция буфера заголовка

			public:

			// Конструкторы и деструкторы класса

				Header();
				virtual ~Header() {}

			// Селекторы класса

				 int getSize() const;

				 uint8_t getV()    const;
				 uint8_t getP()    const;
				 uint8_t getX()    const;
				 uint8_t getCC()   const;
				 uint8_t getM()    const;
				 uint8_t getPT()   const;
				uint16_t getSEQ()  const;
				uint32_t getTS()   const;
				uint32_t getSSRC() const;

			// Модификаторы класса

				void setV(uint8_t v);
				void setP(uint8_t p);
				void setX(uint8_t x);
				void setCC(uint8_t cc);
				void setM(uint8_t m);
				void setPT(uint8_t pt);
				void setSEQ(uint16_t seq);
				void setTS(uint32_t ts);
				void setSSRC(uint32_t ssrc);

				void setHeader(
									 uint8_t v,
									 uint8_t p,
									 uint8_t x,
									 uint8_t cc,
									 uint8_t m,
									 uint8_t pt,
									 uint16_t seq,
									 uint32_t ts,
									 uint32_t ssrc
								  );

		// Другие методы класса

			virtual void show() const;

		}; // class Header


		class Packet : public Byte::Buffer
		{

			/* Класс предназначен для работы с буфером rtp-пакета */

			private:

				uint8_t *h_end; /* Указатель на конечную позиция части буфера, 
									    которая содержит заголовок rtp-пакета */

			public:

				// Конструкторы и деструкторы класса

					Packet();
					Packet(uint8_t *buf, int bufsize);
					Packet(const Array & array);
					virtual ~Packet() {}

				// Селекторы класса

					virtual int getHeaderSize() const;
					virtual Header getHeader() const;
					virtual Header & getHeader(Header & header);

					virtual int getPayloadLen() const;
					virtual int getPayload(uint8_t *buf, int bufsize);

				// Модификаторы класса

					virtual void setHeaderSize(int size);
					virtual void setPayload(uint8_t *buf, int len);

		}; // class Packet

	}; // namespace Rtp

#endif // RTP_HPP
