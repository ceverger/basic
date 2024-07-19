#ifndef BUFFER_HPP
#define BUFFER_HPP

	class Buffer : Basic 
	{

		public:

			/* Конструкторы и деструкторы класса */
			
				Buffer() : Basic() {}

				virtual ~Buffer() {}

			/* Селекторы класса */

				 int getSize() const { return m_end - m_pos; }
				 int getLen()  const { return   cur - m_pos; }
				 int getFree() const { return m_end - cur;   }

			/* Другие методы класса */

				int load(const char *filename);

			/* Чекеры класса */
	
				virtual bool isFull() const
				{
					return getFree() > 0 ? true : false;
				}

				virtual bool isEmpty() const
				{
					return getFree() > 0 ? true : false;
				}
				 
	}; // class Basic 

#endif // BASIC_HPP
