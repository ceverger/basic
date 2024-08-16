#include "byte.hpp"

#ifndef RTSP_HPP
#define RTSP_HPP

	namespace Rtsp 
	{
		/* Пространство имён модуля для работы с протоколом RTSP */
		
			namespace Request
			{
				/* Пространство имён модуля для работы с rtsp-запросам */
				
					class Generator : public Byte::Buffer
					{
						/* Класс предназначен для генерации rtsp-зпросов */

							public:
		
							/* Конструкторы и деструкторы класса */

								Generator();
								Generator(char *buf, int bufsize);
								Generator(const Byte::Array & array);
								virtual ~Generator() {}

							/* Методы для генерации rtsp-запросов */

								void generate(
													const char *method,
													const char *uri,
													double vernum = 0
												 );

								void setMethod(const char *method);
								void setURI(const char *uri);
								void setLogin(const char *login);
								void setPassword(const char *password);
								void setPort(int port);
								void setName(const char *name);
								void setVernum(double vernum);

							/* Методы для добавления полей в текст запроса */

								void addAccept(char *accept);
								void addAcceptEncoding(char *acceptEncoding);
								void addAcceptLanguage(char *acceptLanguage);

								void addAuthorization(char *authorization);
								void addBandwidth(int bandwidth);
								void addBlocksize(int blocksize);
								void addConference(char *conference);

					}; // class Generator


					class Parser : public Byte::Buffer
					{
						/* Класс предназначен для парсинга rtsp-запросов */

						public:

							/* Конструкторы и деструкторы класса */

								Parser();
								Parser(char *buf, int bufsize);
								Parser(const Byte::Array & array);
								virtual ~Parser() {}
	
							/* Методы для парсинга rtsp-запроса */

								void getMethod(const char *message);

								void getURI(const char *message);
								void getLogin(char *buf, int bufsize);
								void getPassword(char *buf, int bufsize);
								void getHost(char *buf, int bufsize);
								 int getPort();
								void getURL(char *buf, int bufsize);
								void getName(char *buf, int bufsize);

								void getVersion(const char *message);
								double getVernum();

								void getAccept(char *message);
								void getAcceptEncoding(char *message);
								void getAcceptLanguage(char *message);

								void getAuthorization(char *message);
								 int getBandwidth(char *message);
								 int getBlocksize(char *message);
								void getConference(char *message);
								void getFrom(char *message);
								void getIfModifiedSince(char *message);
								void getProxyRequire(char *message);
								void getReferer(char *message);
								void getReuire(char *message);
								void getUserAgent(char *message);					

					}; // class Parser

			}; // namespace Request

			namespace Response
			{
				/* Пространство имён модуля для работы с rtsp-ответами */

				class Generator : public Byte::Buffer
				{
					/* Класс предназначен для генерации rtsp-ответов */

					public:

					/* Конструкторы и деструкторы класса */

						Generator();
						Generator(char *buf, int bufsize);
						Generator(const Byte::Array & array);
						virtual ~Generator() {}			

				}; // class Generator

				class Parser : public Byte::Buffer
				{
					/* Класс предназначен для парсинга rtsp-ответов */

					public:

					/* Конструкторы и деструкторы класса */

						Parser();
						Parser(char *buf, int bufsize);
						Parser(const Byte::Array & array);
						virtual ~Parser() {}

					/* Методы для парсинга rtsp-запроса */

						void getStatusLine(const char *message);
						 int getStatusCode(const char *message);
						 int getReasonPhrase(const char *message);
						void getAllow(const char *message);
						void getPublic(const char *message);
						void getRetryAfter(const char *message);
						void getRtpInfo(const char *message);
						void getUnsupported(const char *message);
						void getWWWAuthenticate(const char *message);

				}; // class Generator 
				
			}; // namespace Response


		class Server
		{
			private:

				

			public:

								

		}; // class Server


		class Client
		{
			private:

				

			public:
				
			
		}; // class Client
			
	}; // namespace Rtsp

#endif // RTSP_HPP
