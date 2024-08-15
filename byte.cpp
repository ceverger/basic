#include <cstdio>
#include <cstring>
#include "byte.hpp"

	namespace Byte 
	{
		/* Пространство имён модуля Byte, который предназначен 
			для управления массивом байт типа uint8_t */
			
			Array::Array()
			{
				/* Конструктор по умолчанию */

					m_pos = nullptr;
					  cur = nullptr;
					m_end = nullptr;
			}

			Array::Array(uint8_t *buf, int bufsize)
			{
				/* Пользовательский конструктор класса */

					m_pos = buf;
					  cur = m_pos;
					m_end = m_pos + bufsize;
			}

			Array::Array(const Array & array)
			{
				/* Конструктор копирования */
				
					m_pos = array.m_pos;
					  cur = array.cur;
					m_end = array.m_end;
			}

			Array & Array::operator=(const Array & item)
			{
				/* Конструтор присваивания */

					m_pos = item.m_pos;
					  cur = item.cur;
					m_end = item.m_end;

					return *this;
			}

			int Array::getSize() const
			{
				/* Получение размера массива байтов */

					if(!isInit()) return 0;

					return m_end - m_pos;
			}

			Array & Array::getItem(Array & array)
			{
				/* Инициализация параметров объекта array
					значениями параметров текущего объекта */

					array.m_pos = m_pos;
					array.cur = cur;
					array.m_end = m_end;

					return array;
			}

			void Array::setPos(uint8_t *pos)
			{
				/* Установка начальной и текущей позиции массива байт */

					if(isInit()) return;

					m_pos = pos;
					cur = m_pos;
			}

			void Array::setEnd(uint8_t *end)
			{
				/* Установка конечной позиции массива байт */

					if(isInit()) return;

					m_end = end;					
			}

			void Array::setBuf(uint8_t *buf, int bufsize)
			{
				/* Установка начальной и конечной позиции массива байт */

					m_pos = buf;
					  cur = m_pos;
					m_end = m_pos + bufsize;	
			}

			Array & Array::setItem(const Array & item)
			{
				/* Инициализация параметров текущего объекта
					значениями параметров объекта item */

					m_pos = item.m_pos;
					  cur = item.cur;
					m_end = item.m_end;

					return *this;
			}

			void Array::reset()
			{
				/* Сброс текущей позиции в массиве байт */
			
					cur = m_pos;
			}

			void Array::clear()
			{
				/* Очистка данных объекта */

					m_pos = nullptr;
					  cur = nullptr;
					m_end = nullptr;
			}

			bool Array::isInit() const
			{
				/* Проверка того, была ли произведена инициализация объекта */

					return m_pos == nullptr || m_end == nullptr ? false : true;
			}

			bool Array::isReset() const
			{
				/* Проверка того, cброшена ли текущая позиция в начало буфера */

					return cur == m_pos ? true : false;
			}

			bool Array::isComplete() const
			{
				/* Проверка того, равна ли текущая позиция концу буфера */

					return cur == m_end ? true : false;
			}


		/* Реализация методов класса Buffer */

			Buffer::Buffer() : Array()
			{
				// Конструктор по умолчанию
			}

			Buffer::Buffer(uint8_t *buf, int bufsize) : Array(buf, bufsize)
			{
				// Пользовательский конструктор
			}

			Buffer::Buffer(const Array & array) : Array(array)
			{
				// Конструктор копирования
			}

			int Buffer::getLen() const
			{
				/* Метод возвращает количество записанных
					в буфер данных (валидных байт) */

					if(!isInit()) return -1;

					return cur - m_pos;
			}

			int Buffer::getFree() const
			{
				/* Метод возвращает количество 
					свободного места в буфере */

					if(!isInit()) return -1;

					return m_end - cur;
			}

			int Buffer::getValue(int index) const
			{
				/* Получение значения элемента массива байт */

					return m_pos[index];
			}

			int Buffer::getData(uint8_t *buf, int bufsize) const
			{
				/* Метод копирует данные в другой буфер 
					и возвращет количество скопированных байт */

					if(!isInit()) return -1;

					int i = 0;

					while(i < getLen() && i < bufsize)
					{
						buf[i] = m_pos[i];
						i++;
					}

					return i;
			}

			Array & Buffer::getData(Array & array)
			{
				/* Метод копирует данные в буфер другого объекта
					и возвращет количество скопированных байт */

					if(!isInit()) return *this;
					if(!array.isInit()) return *this;

					uint8_t *pos = m_pos;
					
					array.reset();

					while(pos < cur && array.cur < array.m_end)
					{
						*pos++ = *array.cur++;
					}

					return *this;
			}

			void Buffer::setValue(int index, uint8_t value)
			{
				/* Метод инициализирует один из элементов буфера */

					m_pos[index] = value;
			}

			void Buffer::setData(const uint8_t *buf, int bufsize)
			{
				/* Метод копирует данные в буфер текущего объекта */

					if(!isInit()) return;

					int i = 0;

					reset();

					while(i < bufsize && cur < m_end)
					{
						*cur++ = *buf++;
						 i++;
					}
			}

			Array & Buffer::setData(Array & array)
			{
				if(!isInit()) return array;
				if(!array.isInit()) return array;

				reset();

				uint8_t *pos = array.m_pos;

				while(pos < array.cur && cur < m_end)
				{
					*pos++ = *cur++;
				}

				return array;
			}

			int Buffer::addValue(uint8_t value)
			{
				/* Метод добавляет новый валидный 
					элемент в конец буфера */

					if(!isInit()) return -1;

					if(cur < m_end)
					{
						*cur++ = value;
						 return 1;
					}

					return 0;
			}

		   int Buffer::addData(uint8_t *buf, int len)
			{
				/* Добавление новых данных в конец буфера */

					if(!isInit()) return -1;

					int i = 0;

					while(i < len && cur < m_end)
					{
						*cur++ = *buf++;
						 i++;						
					}
					
					return i;
			}

			Array & Buffer::addData(Array & array)
			{
				/* Копирование новых данных в конец 
					буфера из другого объекта */

					if(!isInit()) return *this;
					if(!array.isInit()) return *this;

					uint8_t *buf = array.m_pos;

					while(buf < array.cur && cur < m_end)
					{
						*cur++ = *buf++;
					}
					
					return *this;					
				}

			void Buffer::zero()
			{
				if(!isInit()) return;

				bzero(m_pos, getSize());
			}

	}; // namespace Byte 
