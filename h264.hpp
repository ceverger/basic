#include "basic.hpp"

#ifndef H264_HPP
#define H264_HPP

	namespace H264 
	{
		class Parser;
		class NalUnit;
		class AccessUnit;

		class Stream : public Basic
		{
			public:

				/* Констркуторы и деструкторы класса */

		     		Stream() : Basic() {}
					virtual ~Stream() {}

				/* Селекторы класса */

					int nalUnitCount();
					int accessUnitCount();

		 		/* Другие методы класса */

					Parser & parser(Parser & p);

		}; // class Stream


		class Parser : public Basic
		{
			public:

			/* Конструкторы и деструкторы класса */

				Parser() : Basic () {}
				virtual ~Parser() {}

			/* Другие методы класса */
				
				AccessUnit & nextAccessUnit(AccessUnit & au);
				NalUnit & nextNalUnit(NalUnit & nu);

		}; // class Parser


		class NalUnit : public Basic
		{
			private:

				uint8_t header;     // Заголовок nal unit
				uint8_t *startCode; // Указатель на начальную позиция стартового кода

			public:

			/* Конструкторы и деструкторы класса */

				NalUnit() : Basic()
				{
					header = 0;
					startCode = nullptr;
				}

				virtual ~NalUnit() {}

			/* Селекторы класса */		

				uint8_t  getHeader() const { return *m_pos; }
				uint8_t *getStartCode() const { return startCode; }
				    int  getStartCodeLen() const { return m_pos - startCode; }

				uint8_t getForbiddenBit() const;
				uint8_t getReferenceIDC() const;
				uint8_t getPayloadType()  const;

			/* Модификаторы класса */

				virtual void setPos(uint8_t *pos);
				virtual void setEnd(uint8_t *end);

			/* Другие методы класса */

				int payload(char *buf, int bufsize);

				virtual void init();
				virtual void clear();

			/* Чекеры класса */
	
				virtual bool isInit() const;
				
		}; // class NalUnit


		class AccessUnit : public Basic
		{
			public:

				/* Конструкторы и деструкторы класса */

					AccessUnit() : Basic() {}

					virtual ~AccessUnit() {}

				/* Другие методы класса */

					int naluCount();
					NalUnit & nextNalUnit(NalUnit & nu);
					NalUnit & currentNalUnit(NalUnit & nu);
					
			
		}; // class AccessUnit


		class Packer : Basic
		{
			public:
				
				enum { UnknowMode, StapaMode, FuaMode, SingleMode };

			private:

				int mode; // Текущий режим упаковки

			public:

				/* Конструкторы и деструкторы класса */

					Packer() : Basic() 
					{ 
						mode = UnknowMode; 
					}

					virtual ~Packer() {}

				/* Селекторы класса */

					int getMode() const { return mode;        }
					int getLen()  const { return cur - m_pos; }
					int getFree() const { return m_end - cur; }

				/* Другие методы класса */

					int pack(AccessUnit & au);
					int data(void *buf, int bufsize);

		}; // class Packer


		class Unpacker : public Basic
		{
			public:

				/* Конструкторы и деструкторы класса */
			
					Unpacker();
					virtual ~Unpacker() {}

				/* Другие методы классса */

					int unpack(uint8_t *buf, int bufsize);
				
		}; // class Unpacker


	uint8_t GetForbiddenBit(uint8_t header);
	uint8_t GetReferenceIDC(uint8_t header);
	uint8_t GetPayloadType(uint8_t header);

	uint8_t SetForbiddenBit(uint8_t forbiddenBit, uint8_t header);
	uint8_t SetReferenceIDC(uint8_t referenceIDC, uint8_t header);
	uint8_t SetPayloadType(uint8_t payloadType, uint8_t header);

	uint8_t GetStapaHeader(uint8_t header);

	uint8_t GetFuaIndicator(uint8_t header);
	uint8_t GetFuaHeader(uint8_t header);
	uint8_t GetFuaStartBit(uint8_t fuaHeader);
	uint8_t GetFuaEndBit(uint8_t fuaHeader);

	uint8_t SetFuaStartBit(uint8_t startBit, uint8_t fuaHeader);
	uint8_t SetFuaEndBit(uint8_t endtBit, uint8_t fuaHeader);
	uint8_t ResetFuaBits(uint8_t fuaHeader);
		
	}; // namespace H264

#endif // H264_HPP
