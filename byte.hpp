#include <cstdint>

#ifndef BYTE_HPP
#define BYTE_HPP

	namespace Byte 
	{
		/* Модуль Byte предназначен для работы с массивом байтов типа uint8_t */

		class Array
		{
			/* Базовый класс для работы с массивом байт типа uint8_t.  
				Класс предназначен исключительно для чтения массива и 
				сохранения информации о нём. Модифицировать массив байт 
				данный класс не может */

				friend class Buffer;

			protected:

				uint8_t *m_pos; // Начальная позиция в массиве байт
				uint8_t *cur;   // Текущая позиция в массиве байт
				uint8_t *m_end; // Конечная позиция в массиве байт		

			public:

			/* Конструкторы и деструкторы класса */
			
				Array();
				Array(uint8_t *buf, int bufsize);
				Array(const Array & array);
				virtual ~Array() {}

			/* Селекторы класса */

				virtual int getSize() const;
				virtual Array & getItem(Array & array);

			/* Модификаторы класса */

				virtual void setPos(uint8_t *pos);
				virtual void setEnd(uint8_t *end);
				virtual void setBuf(uint8_t *buf, int bufsize);
				virtual Array & setItem(const Array & item);

		 	/* Другие методы класса */

				virtual void reset();
				virtual void clear();

			/* Чекеры класса */
	
				virtual bool isInit() const;
				virtual bool isReset() const;
				virtual bool isComplete() const;

			/* Перегрузка операций */

				virtual Array & operator=(const Array & item);

		}; // class Array


		class Buffer : public Array
		{
			/* Класс предназначен для записи данных в
				массив байт и предоставления доступа к ним. */

			public:

			/* Конструкторы и деструкторы класса */
			
				Buffer();
				Buffer(uint8_t *buf, int bufsize);
				Buffer(const Array & array);
				virtual ~Buffer() {}

			/* Селекторы класса */

				int getLen() const;
				int getFree() const;

				virtual int getValue(int index) const;
				virtual int getData(uint8_t *buf, int bufsize) const;
				virtual Array & getData(Array & array);

			/* Модификаторы класса */

				virtual void setValue(int index, uint8_t value);
				virtual void setData(const uint8_t *buf, int len);
				virtual Array & setData(Array & array);

			/* Другие методы класса */
			
				virtual int addValue(uint8_t value);
			   virtual int addData(uint8_t *buf, int len);
				virtual Array & addData(Array & array);

				virtual void zero();

		}; // class Buffer


		int copyBytes(uint8_t *src, uint8_t *dst, int len);

	}; // namespace Byte

#endif // BYTE_HPP
