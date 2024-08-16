#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "rtsp.hpp"

	static bool checkMethod1(const char *method)
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

	static const char *checkMethod2(const char *str)
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

	static bool checkURI(const char *uri)
	{
		if(uri == nullptr) return false;
		if(strlen(uri) == 0) return false;

		if(*uri == '*') return true;
		if(strstr(uri, "rtsp://") != nullptr) return true;

		return false;
	}

	namespace Rtsp
	{
		Request::Generator::Generator() : Buffer() 
		{
			// Конструктор по умолчанию
		}

		Request::Generator::Generator(char *buf, int bufsize) : Buffer((uint8_t *)buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Request::Generator::Generator(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		void Request::Generator::generate(const char *method, const char *uri, double vernum)
		{
			// Генерация основной строки rtsp-запроса

				if(!isInit()) return;
				if(method == nullptr || uri == nullptr) return;
				if(strlen(method) == 0 || strlen(uri) == 0) return;
				if(checkMethod1(method) == false || checkURI(uri) == false) return;

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

				ret = snprintf((char *)cur, getFree(), "%s%.1f", "RTSP/", vernum);
				if(ret < 0) return;

				cur += ret;

			  *cur++ = '\r';
			  *cur++ = '\n';
		}

		void Request::Generator::setMethod(const char *method)
		{
			// Установка имени метода

				if(!isInit()) return;
				if(method == nullptr) return;
				if(!checkMethod1(method)) return;

				uint8_t buf[getLen()];
				uint8_t *pos = (uint8_t *) method;
				uint8_t *end = pos + strlen(method);

				if(getLen() > 0)
				{
					for(int i = 0; i < getLen(); ++i)
					{
						
					}
				}
				reset();
				zero();

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';
		}

		void Request::Generator::setURI(const char *uri)
		{
			// Установка uri ресурса

				if(!isInit()) return;
				if(uri == nullptr) return;
				if(!checkURI(uri)) return;
				
				*cur = '\0';
				
				if(!checkMethod2((char *) m_pos)) return;

				uint8_t *pos = (uint8_t *) uri;
				uint8_t *end = pos + strlen(uri);

				while(cur < m_end && pos < end)
				{
					*cur++ = *pos++;
				}

				*cur++ = ' ';							
		}

		void Request::Generator::setLogin(const char *login)
		{
			// Установка логина пользователя
		}

		void Request::Generator::setPassword(const char *password)	
		{
			// Установка пароля пользователя
		}

		void Request::Generator::setPort(int port)
		{
			// Установка порта сервера
		}

		void Request::Generator::setName(const char *name)
		{
			// Установка имени ресурса
		}

		void Request::Generator::setVernum(double vernum)
		{
			// Установка версии rtsp-протокола

				if(!isInit()) return;
				if(uri == nullptr) return;
				if(!checkURI(uri)) return;
				
				*cur = '\0';
		}

		Request::Parser::Parser() : Buffer()
		{
			// Конструктор по умолчанию		
		}

		Request::Parser::Parser(char *buf, int bufsize) : Buffer((uint8_t *) buf, bufsize)
		{
			// Пользовательский конструктор		
		}

		Request::Parser::Parser(const Array & array) : Buffer(array)
		{
			// Констуктор копирования		
		}

		void Request::Parser::getMethod(const char *message)
		{
			// Получение имени метода
		}

		void Request::Parser::getURI(const char *message)
		{
			// Получение uri ресурса
		}

		void Request::Parser::getLogin(char *buf, int bufsize)
		{
			
		}

		void Request::Parser::getPassword(char *buf, int bufsize)
		{
			
		}

		void Request::Parser::getHost(char *buf, int bufsize)
		{
			
		}

		int Request::Parser::getPort()
		{
			return 0;
		}

		void Request::Parser::getURL(char *buf, int bufsize)
		{
			
		}

		void Request::Parser::getName(char *buf, int bufsize)
		{
			
		}

		void Request::Parser::getVersion(const char *message)
		{
			
		}

		double Request::Parser::getVernum()
		{
			return 0;
		}

		void Request::Parser::getAccept(char *message)
		{
			
		}

		void Request::Parser::getAcceptEncoding(char *message)
		{
			
		}

		void Request::Parser::getAcceptLanguage(char *message)
		{
			
		}

		void Request::Parser::getAuthorization(char *message)
		{
			
		}

		int Request::Parser::getBandwidth(char *message)
		{
			return 0;
		}

		int Request::Parser::getBlocksize(char *message)
		{
			return 0;
		}

		void Request::Parser::getConference(char *message)
		{
			
		}

		void Request::Parser::getFrom(char *message)
		{
			
		}

		void Request::Parser::getIfModifiedSince(char *message)
		{
			
		}

		void Request::Parser::getProxyRequire(char *message)
		{
			
		}

		void Request::Parser::getReferer(char *message)
		{
			
		}

		void Request::Parser::getReuire(char *message)
		{
			
		}

		void Request::Parser::getUserAgent(char *message)
		{
			
		}

		Response::Generator::Generator() : Byte::Buffer()
		{
			// Конструктор по умолчанию
		}

		Response::Generator::Generator(char *buf, int bufsize) : Buffer((uint8_t *) buf, bufsize)
		{
			// Пользовательский конструктор
		}

		Response::Generator::Generator(const Array & array) : Buffer(array)
		{
			// Констуктор копирования
		}

		Response::Parser::Parser() : Buffer()
		{
			// Конструктор по умолчанию		
		}

		Response::Parser::Parser(char *buf, int bufsize) : Buffer((uint8_t *) buf, bufsize)
		{
			// Пользовательский конструктор		
		}

		Response::Parser::Parser(const Array & array) : Buffer(array)
		{
			// Констуктор копирования		
		}

		void Response::Parser::getStatusLine(const char *message)
		{
			
		}

		int Response::Parser::getStatusCode(const char *message)
		{
			return 0;
		}

		int Response::Parser::getReasonPhrase(const char *message)
		{
			return 0;
		}

		void Response::Parser::getAllow(const char *message)
		{

		}

		void Response::Parser::getPublic(const char *message)
		{

		}

		void Response::Parser::getRetryAfter(const char *message)
		{

		}

		void Response::Parser::getRtpInfo(const char *message)
		{
			
		}

		void Response::Parser::getUnsupported(const char *message)
		{
			
		}

		void Response::Parser::getWWWAuthenticate(const char *message)
		{
			
		}

	}; // namespace Rtsp
