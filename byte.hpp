#include <cstdint>

#ifndef BYTE_HPP
#define BYTE_HPP

	namespace Byte 
	{
		/* Модуль BYTE предназначен для работы с массивом байтов типа uint8_t */

		class Array
		{
			/* Базовый класс для работы с массивом байт типа uint8_t.  
				Класс предназначен исключительно для чтения массива и 
				сохранения информации о нём. Модифицировать массив байт 
				данный класс не может                                   */

			protected:

				uint8_t *m_pos; // Начальная позиция в массиве байт
				uint8_t *cur;   // Текущая позиция в массиве байт
				uint8_t *m_end; // Конечная позиция в массиве байт		

			public:

			/* Конструкторы и деструкторы класса */
			
				Array();
				Array(const Array & item);
				Array & operator=(const Array & item);

				virtual ~Array() {}

			/* Селекторы класса */

				 int getSize() const;
				 Array & getItem(Array & item) const;

			/* Модификаторы класса */

				virtual void setPos(uint8_t *pos);
				virtual void setEnd(uint8_t *end);
				virtual void setBuf(uint8_t *buf, int bufsize);
				virtual void setItem(const Array & item);

		 	/* Другие методы класса */

				virtual void reset();
				virtual void clear();

			/* Чекеры класса */
	
				virtual bool isInit() const;					

		}; // class Array

		
		class Buffer : Array
		{
			/* Класс предназначен для записи данных в массив байт
				и предоставления доступа к ним, а также их модификации. */

			public:

			/* Конструкторы и деструкторы класса */
			
				Buffer() : Array() {}
				virtual ~Array() {}

			/* Селекторы класса */

				int getLen() const;
				int getFree() const;

				uint8_t getValue(int index);
				int getData(uint8_t *buf, int bufsize);
				Array & getData(Array & item);

			/* Модификаторы класса */

				void setValue(int index, uint8_t value);
				void setData(uint8_t *buf, int bufsize);
				void setData(const Array & item);

			/* Другие методы класса */
			
				int addData(uint8_t value);
			   int addData(uint8_t *buf, int len);
				int addData(const Array & item);
			
		} // class Buffer

	}; // namespace Byte 

#endif // BYTE_HPP
