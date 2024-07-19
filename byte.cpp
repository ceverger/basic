#include "byte.hpp"

	namespace BYTE 
	{
		/* Реализация методов классов модуля BYTE */

			/* Реализация методов класса Array */
			
				Array::Array();
				{
					/* Конструктор по умолчанию */

					m_pos = nullptr;
					  cur = nullptr;
					m_end = nullptr;
				}

				Array::Array(const Array & item)
				{
					/* Конструктор копирования */
					
					m_pos = item.m_pos;
					  cur = item.cur;
					m_end = item.m_end;
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

					return m_end - m_pos;
				}

				Array & Array::getItem(Array & item) const
				{
					/* Копирование данных в другой объект */

					item.m_pos = m_pos;
					item.cur = cur;
					item.m_end = m_end;
				}

				void Array::setPos(uint8_t *pos);
				{
					/* Установка начальной и текущей позиции массива байт */
					
					// Если объект уже был инициализирован,
					// то работа метода завершается

						if(isInit()) return;

					m_pos = pos;
					cur = m_pos;
				}

				void Array::setEnd(uint8_t *end)
				{
					/* Установка конечной позиции массива байт */
					
					// Если объект уже был инициализирован,
					// то работа метода завершается

						if(isInit()) return;

					m_end = end;					
				}

				void Array::setBuf(uint8_t *buf, int bufsize)
				{
					/* Установка начальной и конечной позиции массива байт */

					// Если объект уже был инициализирован,
					// то работа метода завершается

						if(isInit()) return;

					m_pos = buf;
					m_end = buf + bufsize;					
				}

				void Array::setItem(const Array & item)
				{
					/* Копирование данных из другого объекта */

					// Если объект уже был инициализирован,
					// то работа метода завершается

						if(isInit()) return;

					m_pos = item.m_pos;
					  cur = item.cur;
					m_end = item.m_end;
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
					return m_pos == nullptr || m_end == nullptr ? false : true;
				}

	}; // namespace BYTE 
