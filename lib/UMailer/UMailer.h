// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#ifndef __UMAILER_H__
#define __UMAILER_H__

#include <Arduino.h>
#include <WiFi.h>
#include <deque>

class TUMailer {
public:
	TUMailer();

	class Message {
	public:
		Message();

		void setFromEmail(const String &fromEmail);
		void setFromName(const String &fromName);
		void addTo(const String &to);
		void addCc(const String &cc);
		void addBcc(const String &bcc);
		void setSubject(const String &subject);
		void setBody(const String &body);
		void appendBody(const String &body);
	private:
		friend class TUMailer;
		String m_fromEmail;
		String m_fromName;
		std::vector<String> m_to;
		std::vector<String> m_cc;
		std::vector<String> m_bcc;
		String m_subject;
		String m_body;
	};

	void begin(const String &server);

	void setSmtpPort(uint16_t port);
	void setClientName(const String &clientName);
	void setTimeout(uint32_t seconds);

	bool send(const Message &message);
	bool send(std::deque<Message> &mailq);

	const char *errorCode();
	const char *errorDescription();

private:
	bool read(WiFiClient &client, const String &expectedCode);
	bool write(WiFiClient &client, const String &cmd);
	bool sendMessage(WiFiClient &client, const Message &message);

	String m_server;
	uint16_t m_port;
	String m_clientName;

	String m_errDesc;
	String m_errCode;

	uint32_t m_timeout;
};

extern TUMailer UMailer;

#endif /* __UMAILER_H__ */