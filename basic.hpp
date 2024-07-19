#include <cstdint>

#ifndef BASIC_HPP
#define BASIC_HPP

	class Basic 
	{
		protected:

			uint8_t *m_pos; // Начальная позиция буфера с данными
			uint8_t *cur;   // Текущая позиция буфера с данными
			uint8_t *m_end; // Конечная позиция буфера с данными

		public:

			/* Конструкторы и деструкторы класса */
			
				Basic() 
				{ 
					m_pos = nullptr;
					  cur = nullptr;
					m_end = nullptr;
				}

				virtual ~Basic() {}

			/* Селекторы класса */

				 int  getSize() const { return m_end - m_pos; }

			/* Модификаторы класса */

				virtual void setPos(void *pos)
				{
					m_pos = isInit() ? m_pos : (uint8_t *) pos;		
				}

				virtual void setEnd(void *end)
				{
					m_end = isInit() ? m_end : (uint8_t *) end;	
				}

				virtual void setItem(const Basic & item)
				{
					if(isInit()) return;
					if(!isInit()) return;

					m_pos = item.m_pos;
					  cur = item.cur;
					m_end = item.m_end;
				}

				virtual void setBuf(void *buf, int bufsize)
				{
					if(isInit()) return;

					m_pos = (uint8_t *) buf;
					m_end = (uint8_t *) buf + bufsize;
				}

		 	/* Другие методы класса */

				virtual void init()
				{
					cur = isInit() ? cur : m_pos;			
				}

				virtual void reset()
				{
					cur = m_pos;
				}

				virtual void clear()
				{
					m_pos = nullptr;
					  cur = nullptr;
					m_end = nullptr;	
				}

			/* Чекеры класса */
	
				virtual bool isInit() const
				{
					return (m_pos == nullptr || m_end == nullptr || cur == nullptr) ? false : true;
				}
				 
	}; // class Basic 

#endif // BASIC_HPP
