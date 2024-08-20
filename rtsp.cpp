#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtsp0.hpp"

	static bool checkMethod(const char *method)
	{
		if(method == nullptr) return false;
		if(strlen(method) == 0) return false;

		if(strcmp(method, "OPTIONS")       != 0)
		if(strcmp(method, "DESCRIBE")      != 0)
		if(strcmp(method, "ANNOUNCE")      != 0)
		if(strcmp(method, "SETUP")         != 0)
		if(strcmp(method, "PLAY")          != 0)
		if(strcmp(method, "PAUSE")         != 0)
		if(strcmp(method, "TEARDOWN")      != 0)
		if(strcmp(method, "GET_PARAMETER") != 0)
		if(strcmp(method, "SET_PARAMETER") != 0)
		return false;

		return true;
	}

	static bool checkURI(const char *uri)
	{
		if(uri == nullptr) return false;
		if(strlen(uri) == 0) return false;

		if(*uri == '*') return true;
		if(strstr(uri, "rtsp://") != nullptr) return true;

		return false;
	}

	static const char *searchMethod(const char *str)
	{
		const char *pos = strstr(str, "OPTION");
		if(pos == nullptr) pos = strstr(str, "DESCRIBE");
		if(pos == nullptr) pos = strstr(str, "ANNOUNCE");
		if(pos == nullptr) pos = strstr(str, "SETUP");
		if(pos == nullptr) pos = strstr(str, "PLAY");
		if(pos == nullptr) pos = strstr(str, "PAUSE");
		if(pos == nullptr) pos = strstr(str, "TEARDOWN");
		if(pos == nullptr) pos = strstr(str, "GET_PARAMETER");
		if(pos == nullptr) pos = strstr(str, "SET_PARAMETER");
		if(pos == nullptr) return nullptr;

		return pos;
	}

	static const char *searchURI(const char *str)
	{
		const char *pos = strstr(str, "*");

		if(pos == nullptr)
		{	
			pos = strstr(str, "rtsp://");
		}

		return pos;
	}

	static const char *searchVernum(const char *str)
	{
		const char *pos = strstr(str, "RTSP/");
		return pos;
	}

	namespace Rtsp
	{
		Request::Request() : Buffer() 
		{
			// Конструктор по умолчанию
		}

		Request::Request(char *buf, int bufsize) : Buffer((uint8_t *)buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Request::Request(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		void Request::generate(const char *method, const char *uri, double vernum)
		{
			// Генерация основной строки rtsp-запроса

				if(!isInit()) return;
				if(method == nullptr || uri == nullptr) return;
				if(strlen(method) == 0 || strlen(uri) == 0) return;
				if(checkMethod(method) == false || checkURI(uri) == false) return;

				reset();

				int ret;
				uint8_t *pos, *end;

				pos = (uint8_t *) method;
				end = (uint8_t *) (pos + strlen(method));

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';
				
				pos = (uint8_t *) uri;
				end = (uint8_t *) (pos + strlen(uri));

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';

				ret = snprintf((char *)cur, getFree(), "%s%.1f\r\n", "RTSP/", vernum);
				if(ret < 0) return;

				cur += ret;
		}

		void Request::setMethod(const char *method)
		{
			// Установка имени метода

				if(!isInit()) return;
				if(method == nullptr) return;
				if(!checkMethod(method)) return;

				uint8_t *pos = (uint8_t *) method;
				uint8_t *end = pos + strlen(method);

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';
		}

		void Request::setURI(const char *uri)
		{
			// Установка uri ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;

				if(uri == nullptr) return;
				if(!checkURI(uri)) return;		

				uint8_t *pos = (uint8_t *) uri;
				uint8_t *end = pos + strlen(uri);

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';						
		}

		void Request::setLogin(const char *login)
		{
			// Установка логина пользователя

				if(login == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с URI

				pos = (uint8_t *) searchURI((const char *) buf);
				first = pos;

				temp = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				end = pos;
				next = end;

				pos = temp + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

			// Копирование строки с именнем метода из временного буфера в основной

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Поиск в строке с URI подстроки с логином

				pos  = temp + strlen("rtsp://");
				temp = pos;

				while(*pos != '@' && pos < end) pos++;

				if(pos == end)
				{
					/* Если подстрока с логином и паролем отсутствует в 
						строке URI, то создаём её и записываем в основной 
						буфер вместе с основным сообщением. После этого
						выходим из метода */

						pos = temp;
						ret = snprintf((char *)cur, getFree(), "rtsp://%s:@", login);
						cur += ret;

						while(cur < m_end && pos < buf_end)
						{	
							*cur++ = *pos++;
						}

						*cur = '\0';

						return;
				}

				/* Если подстрока с логином в строке URI присутсвует,
					то меняем в ней только логин и записываем изменённую
					строку URI в основной буфер вместе с основным 
					сообщением. */

					pos = temp;
					while(*pos != ':' && pos < end) pos++;

					ret = snprintf((char *)cur, getFree(), "rtsp://%s", login);
					cur += ret;

					while(cur < m_end && pos < buf_end)
					{
						*cur++ = *pos++;
					}

					*cur = '\0';
		}

		void Request::setPassword(const char *password)
		{
			// Установка пароля пользователя

				if(password == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				uint8_t buf[getLen() + 1], *buf_end;
				uint8_t *pos, *end, *temp, *first, *next;

			// Копирование данных из текущего во временный и его очистка

				ret = Byte::copyBytes(m_pos, buf, getLen());
				buf[ret] = '\0';
				buf_end = buf + ret;
				zero();
				reset();

			// Поиск во временном буфере начала и конца строки с URI

				pos = (uint8_t *) searchURI((const char *) buf);
				first = pos;

				temp = pos;
				while(*pos != ' ' && pos < buf_end) pos++;
				end = pos;
				next = end;

				pos = temp + strlen("rtsp://");
				while(*pos != '/' && pos < end) pos++;
				end = pos;

			// Копирование строки с именнем метода из временного буфера в основной

				pos = buf;
				while(pos < first && cur < m_end)
				{
					*cur++ = *pos++;
				}

			// Поиск в строке с URI подстроки с логином и паролем

				pos  = temp + strlen("rtsp://");
				temp = pos;

				while(*pos != '@' && pos < end) pos++;

				if(pos == end)
				{
					/* Если подстрока с логином и паролем отсутствует в 
						строке URI, то создаём её и записываем в основной 
						буфер вместе с основным сообщением. После этого
						выходим из метода */

						pos = temp;
						ret = snprintf((char *)cur, getFree(), "rtsp://:%s@", password);
						cur += ret;

						while(cur < m_end && pos < buf_end)
						{	
							*cur++ = *pos++;
						}

						*cur = '\0';

						return;
				}

				/* Если подстрока с логином  и паролем в строке URI ,
					присутсвует то меняем в ней только пароль и записываем
					изменённую строку URI в основной буфер вместе с основным 
					сообщением. */

					pos = temp;
					while(*pos != ':' && pos < end) pos++;
					end = pos + 1;

					pos = first;
					while(cur < m_end && pos < end)
					{
						*cur++ = *pos++;
					}
					
					temp = pos;
					pos = (uint8_t *) password;

					while(cur < m_end && *pos != '\0')
					{
						*cur++ = *pos++;
					}

					pos = temp;
					while(cur < m_end && pos < buf_end)
					{
						*cur++ = *pos++;
					}

					*cur = '\0';
		}

		void Request::setPort(int port)
		{
			// Установка порта сервера
		}

		void Request::setName(const char *name)
		{
			// Установка имени ресурса
		}

		void Request::setVernum(double vernum)
		{
			// Установка версии rtsp-протокола

				if(!isInit()) return; else *cur = '\0';
				if(!searchMethod((const char *)m_pos)) return;
				if(!searchURI((const char *)m_pos)) return;
				
				int ret = snprintf((char *)cur, getFree(), "RTSP/%.1f\r\n", vernum);
				if(ret < 0) return;

				cur += ret;
		}

		void Request::setCseq(int cseq)
		{
			if(!isInit()) return; else *cur = '\0';
			if(!searchMethod((const char *)m_pos)) return;
			if(!searchURI((const char *)m_pos)) return;
			if(!searchVernum((const char *)m_pos)) return;

			int ret = snprintf((char *)cur, getFree(), "Cseq: %d\r\n", cseq);
			if(ret < 0) return;

			cur += ret;
		}

		void Request::setDate()
		{
			if(!isInit()) return;
				else *cur = '\0';
			if(!searchMethod((const char *)m_pos)) return;
			if(!searchURI((const char *)m_pos)) return;
			if(!searchVernum((const char *)m_pos)) return;

			int ret;
			time_t tm = time(NULL);
			struct tm *gm = gmtime(&tm);		

			ret = strftime((char *)cur, getFree(), "Date: %a, %b %d %Y %T GMT\r\n", gm);
			if(ret < 0) return;

			cur += ret;
		}

		void Request::setPublicMethods(const char *methods)
		{
			if(!isInit()) return; else *cur = '\0';
			if(!searchMethod((const char *) m_pos)) return;
			if(!searchURI((const char *) m_pos))    return;
			if(!searchVernum((const char *) m_pos)) return;

			int ret = snprintf((char *)cur, getFree(), "Public: %s\r\n", methods);
			if(ret < 0) return;

			cur += ret;
		}

		void Request::setMessageEnd()
		{
			if(!isInit()) return; else *cur = '\0';
			if(!searchMethod((const char *) m_pos)) return;
			if(!searchURI((const char *) m_pos))    return;
			if(!searchVernum((const char *) m_pos)) return;

			*cur++ = '\r';
			*cur++ = '\n';
		}

		void Request::getMethod(char *buf, int bufsize)
		{
			// Получение имени метода

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;

				int i = 0;
				char *pos = (char *) searchMethod((const char *) m_pos);

				bzero(buf, bufsize);

				if(pos == nullptr) return;

				while(i < bufsize && *pos != '\0' && *pos != ' ')
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getURI(char *buf, int bufsize)
		{
			// Получение uri ресурса

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;

				int i = 0;
				char *pos = (char *) searchURI((const char *) m_pos);

				bzero(buf, bufsize);

				if(pos == nullptr) return;
	
				while(i < bufsize && *pos != '\0' && *pos != ' ')
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getLogin(char *buf, int bufsize)
		{
			if(buf == nullptr) return;
			if(isInit()) *cur = '\0'; else return;
			if(!searchMethod((const char *) m_pos)) return;

			int i = 0;
			char *pos, *end, *temp;

			bzero(buf, bufsize);

			pos = (char *) searchURI((const char *) m_pos);
			if(pos == nullptr || *pos == '*') return;
			end = (char *) cur;

			temp = pos;

			while(*pos != ' ' && pos < end) pos++;
			if(pos == end) return;
			end = pos;

			pos = temp;
			while(*pos !=  '/' && pos < end) pos++;

			pos = temp;
			while(*pos !=  '@' && pos < end) pos++;
			if(pos == end) return;

			end = pos;
			pos = temp + strlen("rtsp://");

			while(*end != ':' && end > pos) end--;
			if(end == pos) return;

			while(i < bufsize && pos < end)
			{
				*buf++ = *pos++;
				 i++;
			}

			*buf = '\0';
		}

		void Request::getPassword(char *buf, int bufsize)
		{
			if(buf == nullptr) return;
			if(isInit()) *cur = '\0'; else return;
			if(!searchMethod((const char *) m_pos)) return;

			int i = 0;
			char *pos, *end, *temp;

			bzero(buf, bufsize);

			pos = (char *) searchURI((const char *) m_pos);
			if(pos == nullptr || *pos == '*') return;
			end = (char *) cur;

			temp = pos;
			while(*pos != ' ' && pos < end) pos++;
			if(pos == end) return;
			end = pos;

			pos = temp;
			while(*pos !=  '/' && pos < end) pos++;

			pos = temp;
			while(*pos !=  '@' && pos < end) pos++;
			if(pos == end) return;

			end = pos;
			pos = temp + strlen("rtsp://");

			while(*pos != ':' && pos < end) pos++;
			if(pos == end) return;
			pos++;

			while(i < bufsize && pos < end)
			{
				*buf++ = *pos++;
				 i++;
			}

			*buf = '\0';			
		}

		void Request::getHost(char *buf, int bufsize)
		{
			// Получение хоста

				if(buf == nullptr) return;
				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;
				
				int i = 0;
				char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = (char *) searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				if(pos == end) return;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;
				
				pos = temp + strlen("rtsp://");
				while(*pos !=  '@' && pos < end) pos++;
				if(*pos == '@') pos++;

				temp = pos;
				while(*pos !=  ':' && pos < end) pos++;
				if(*pos == ':') end = pos;

				pos = temp;
				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		int Request::getPort()
		{
			// Получение номера порта

				if(isInit()) *cur = '\0'; else return -1;
				if(!searchMethod((const char *) m_pos)) return -1;
				if(!searchURI((const char *) m_pos)) return -1;

				int i = 0, bufsize = 8;
				char buf[bufsize], *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = (char *) searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return 0;
				end = (char *) m_end;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;
				
				pos = temp + strlen("rtsp://");
				while(*pos !=  '@' && pos < end) pos++;
				if(*pos == '@') pos++;

				temp = pos;
				while(*pos !=  ':' && pos < end) pos++;
				if(pos == end) return 0;
				
				pos++;
				temp = buf;
				while(i < bufsize && pos < end)
				{
					*temp++ = *pos++;
					 i++;
				}

				*temp = '\0';

				return strtol(buf, NULL, 10);
		}

		void Request::getName(char *buf, int bufsize)
		{
			// Получение имени ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int i = 0;
				char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = (char *) searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;
				end = (char *) m_end;

				temp = pos;
				while(*pos != ' ' && pos < end) pos++;
				end = pos;

				pos = temp + strlen("rtsp://");
				while(*pos !=  '/' && pos < end) pos++;
				if(pos == end) return;

				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';
		}

		void Request::getURL(char *buf, int bufsize)
		{
			// Получение url ресурса

				if(isInit()) *cur = '\0'; else return;
				if(!searchMethod((const char *) m_pos)) return;
				if(!searchURI((const char *) m_pos)) return;

				int ret, i = 0;
				char *pos, *end, *temp;

				bzero(buf, bufsize);

				pos = (char *) searchURI((const char *) m_pos);
				if(pos == nullptr || *pos == '*') return;
				end = (char *) m_end;

				ret = snprintf(buf, bufsize, "%s", "rtsp://");
				if(ret < 0) return;
				pos += ret;
				buf += ret;
				bufsize -= ret;

				temp = pos;
				while(*pos !=  '/' && pos < end) pos++;
				end = pos;

				pos = temp;
				while(*pos !=  '@' && pos < end) pos++;
				pos = (pos == end) ? temp : pos + 1;

				while(i < bufsize && *pos != ':' && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				if(*pos == ':') while(pos < end) pos++;

				temp = pos;
				end = (char *) m_end;
				while(*pos != ' ' && pos < end) pos++;			
				end = pos;
				pos = temp;

				while(i < bufsize && pos < end)
				{
					*buf++ = *pos++;
					 i++;
				}

				*buf = '\0';																												
		}

		void Request::getVersion(const char *message)
		{
			
		}

		double Request::getVernum()
		{
			return 0;
		}

		void Request::getAccept(char *message)
		{
			
		}

		void Request::getAcceptEncoding(char *message)
		{
			
		}

		void Request::getAcceptLanguage(char *message)
		{
			
		}

		void Request::getAuthorization(char *message)
		{
			
		}

		int Request::getBandwidth(char *message)
		{
			return 0;
		}

		int Request::getBlocksize(char *message)
		{
			return 0;
		}

		void Request::getConference(char *message)
		{
			
		}

		void Request::getFrom(char *message)
		{
			
		}

		void Request::getIfModifiedSince(char *message)
		{
			
		}

		void Request::getProxyRequire(char *message)
		{
			
		}

		void Request::getReferer(char *message)
		{
			
		}

		void Request::getReuire(char *message)
		{
			
		}

		void Request::getUserAgent(char *message)
		{
			
		}

		void Request::addCSeq(int cseq)
		{
			
		}

		Response::Response() : Byte::Buffer()
		{
			// Конструктор по умолчанию
		}

		Response::Response(char *buf, int bufsize) : Buffer((uint8_t *) buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Response::Response(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		void Response::getStatusLine(const char *message)
		{
			
		}

		int Response::getStatusCode(const char *message)
		{
			return 0;
		}

		int Response::getReasonPhrase(const char *message)
		{
			return 0;
		}

		void Response::getAllow(const char *message)
		{

		}

		void Response::getPublic(const char *message)
		{

		}

		void Response::getRetryAfter(const char *message)
		{

		}

		void Response::getRtpInfo(const char *message)
		{
			
		}

		void Response::getUnsupported(const char *message)
		{
			
		}

		void Response::getWWWAuthenticate(const char *message)
		{
			
		}

	}; // namespace Rtsp
