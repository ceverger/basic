#include "byte.hpp"

#ifndef RTSP_HPP
#define RTSP_HPP

	namespace Rtsp 
	{
		/* Пространство имён модуля для работы с протоколом RTSP */
		
			class Request : public Byte::Buffer
			{
				/* Класс для работы с rtsp-запросам */

				public:
		
					/* Конструкторы и деструкторы класса */

						Request();
						Request(char *buf, int bufsize);
						Request(const Byte::Array & array);
						virtual ~Request() {}

					/* Методы для парсинга полей rtsp-запроса */

						void getMethod(char *buf, int bufsize);

						void getURI(char *buf, int bufsize);
						void getLogin(char *buf, int bufsize);
						void getPassword(char *buf, int bufsize);
						void getHost(char *buf, int bufsize);
						 int getPort();
						void getName(char *buf, int bufsize);
						void getURL(char *buf, int bufsize);

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

						void setCseq(int cseq);
						void setDate();
						void setPublicMethods(const char *methods);

						void setMessageEnd();

					/* Методы для добавления полей в текст запроса */

						void addAccept(char *accept);
						void addAcceptEncoding(char *acceptEncoding);
						void addAcceptLanguage(char *acceptLanguage);

						void addAuthorization(char *authorization);
						void addBandwidth(int bandwidth);
						void addBlocksize(int blocksize);
						void addConference(char *conference);	
						void addCSeq(int cseq);	

			}; // class Request


			class Response : public Byte::Buffer
			{
				/* Класс предназначен для парсинга rtsp-запросов */

				public:

					/* Конструкторы и деструкторы класса */

						Response();
						Response(char *buf, int bufsize);
						Response(const Byte::Array & array);
						virtual ~Response() {}

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

			}; // class Response


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
