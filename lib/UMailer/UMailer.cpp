// SPDX-FileCopyrightText: 2021 Samuel Cabrero <samuel@orica.es>
//
// SPDX-License-Identifier: MIT

#include "UMailer.h"

#ifdef UMAILER_DEBUG
#define DBG_IN(...) { Serial.print("<- "); Serial.println(__VA_ARGS__); }
#define DBG_OUT(...) { Serial.print("-> "); Serial.println(__VA_ARGS__); }
#else
#define DBG_IN(...) {}
#define DBG_OUT(...) {}
#endif

TUMailer::Message::Message() {

}

void TUMailer::Message::setFromEmail(const String &fromEmail) {
        m_fromEmail = fromEmail;
}

void TUMailer::Message::setFromName(const String &fromName) {
        m_fromName = fromName;
}

void TUMailer::Message::addTo(const String &to) {
        m_to.push_back(to);
}

void TUMailer::Message::addCc(const String &cc) {
        m_cc.push_back(cc);
}

void TUMailer::Message::addBcc(const String &bcc) {
        m_bcc.push_back(bcc);
}

void TUMailer::Message::setSubject(const String &subject) {
        m_subject = subject;
}

void TUMailer::Message::setBody(const String &body) {
        m_body = body;
}

void TUMailer::Message::appendBody(const String &body) {
        m_body.concat(body);
}

TUMailer::TUMailer() : m_server(""), m_port(25), m_clientName(""), m_timeout(5) {

}

void TUMailer::begin(const String &server) {
        m_server = server;
}

void TUMailer::setSmtpPort(uint16_t port) {
        m_port = port;
}

void TUMailer::setClientName(const String &clientName) {
        m_clientName = clientName;
}

void TUMailer::setTimeout(uint32_t ms) {
	m_timeout = ms;
}

bool TUMailer::read(WiFiClient &client, const String &expectedCode) {
	String response;
	int idx;

	response = client.readStringUntil('\n');
	if (response.length() == 0) {
		m_errCode = -1;
		m_errDesc = "SMTP response timeout. No delimiter char.";
		return false;
	}

	DBG_IN(response);

	idx = response.indexOf(' ');
	if (idx == -1) {
		idx = response.indexOf('-');
		if (idx == -1) {
			m_errCode = -1;
			m_errDesc = "SMTP response format error (" + response + ")";
			return false;
		}
	}

	m_errCode = response.substring(0, idx);
	m_errDesc = response.substring(idx);
	if (m_errCode.compareTo(expectedCode) != 0) {
		m_errDesc = "Unexpected SMTP response (" + m_errDesc + ")";
		return false;
	}

	return true;
}

bool TUMailer::write(WiFiClient &client, const String &cmd) {
	size_t n;

	DBG_OUT(cmd);

	n = client.println(cmd);
	if (n < cmd.length() + 2) {
		m_errCode = -1;
		m_errDesc = "Error sending command (" + cmd + ")";
		return false;
	}

	return true;
}

bool TUMailer::sendMessage(WiFiClient &client, const Message &message) {
	String cmd;

	cmd = "MAIL FROM: <" + message.m_fromEmail + ">";
	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "250")) {
		return false;
	}

	for (String s : message.m_to) {
		cmd = "RCPT TO: <" + s + ">";
		if (!write(client, cmd)) {
			return false;
		}
	  	if (!read(client, "250")) {
		        return false;
	        }
	}

	for (String s : message.m_cc) {
		cmd = "RCPT TO: <" + s + ">";
		if (!write(client, cmd)) {
			return false;
		}
	  	if (!read(client, "250")) {
		        return false;
	        }
	}

	for (String s : message.m_bcc) {
		cmd = "RCPT TO: <" + s + ">";
		if (!write(client, cmd)) {
			return false;
		}
	  	if (!read(client, "250")) {
		        return false;
	        }
	}

        cmd = "DATA";
        if (!write(client, cmd)) {
		return false;
	}

        if (!read(client, "354")) {
		return false;
	}

        cmd = "From: ";
        if (message.m_fromName.length() > 0) {
                cmd += message.m_fromName + " ";
        }
        cmd += "<" + message.m_fromEmail + ">";
        if (!write(client, cmd)) {
		return false;
	}

        cmd = "To: ";
        for (String s : message.m_to) {
		cmd += "<" + s + ">,";
	}
        if (!write(client, cmd)) {
		return false;
	}

        if (message.m_cc.size() > 0) {
		cmd = "Cc: ";
        	for (String s : message.m_cc) {
			cmd += "<" + s + ">,";
		}
        	if (!write(client, cmd)) {
			return false;
		}
        }

        if (message.m_bcc.size() > 0) {
		cmd = "Bcc: ";
        	for (String s : message.m_bcc) {
			cmd += "<" + s + ">,";
		}
        	if (!write(client, cmd)) {
			return false;
		}
        }

	cmd = "Subject: " + message.m_subject;
	if (!write(client, cmd)) {
		return false;
	}

	cmd = "\r\n" + message.m_body + "\r\n.";
	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "250")) {
	        return false;
	}

	return true;
}

bool TUMailer::send(const Message &message) {
        WiFiClient client;
	String cmd;

        if (!client.connect(m_server.c_str(), m_port, m_timeout)) {
                m_errDesc = "Connection Error";
                m_errCode = -1;
                return false;
        }

	client.setTimeout((int)(m_timeout / 1000));

	if (!read(client, "220")) {
		return false;
        }

	cmd = "HELO";
	if (m_clientName.length() > 0) {
		cmd.concat(" ");
		cmd.concat(m_clientName);
	}

	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "250")) {
		return false;
	}

	if (!sendMessage(client, message)) {
		return false;
	}

	cmd = "QUIT";
	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "221")) {
		return false;
	}

  	m_errCode = "0";
	m_errDesc = "";

  	return true;
}

bool TUMailer::send(std::deque<Message> &mailq) {
	WiFiClient client;
	String cmd;

        if (!client.connect(m_server.c_str(), m_port, m_timeout)) {
                m_errDesc = "Connection Error";
                m_errCode = -1;
                return false;
        }

	client.setTimeout((int)(m_timeout / 1000));

	if (!read(client, "220")) {
		return false;
        }

	cmd = "HELO";
	if (m_clientName.length() > 0) {
		cmd.concat(" ");
		cmd.concat(m_clientName);
	}

	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "250")) {
		return false;
	}

	for (auto it = mailq.begin(); it != mailq.end(); ) {
		if (sendMessage(client, *it)) {
			it = mailq.erase(it);
		} else {
			++it;
		}
	}

	cmd = "QUIT";
	if (!write(client, cmd)) {
		return false;
	}

	if (!read(client, "221")) {
		return false;
	}

  	m_errCode = "0";
	m_errDesc = "";

  	return true;
}

const char *TUMailer::errorCode() {
	return m_errCode.c_str();
}

const char *TUMailer::errorDescription() {
	return m_errDesc.c_str();
}

TUMailer UMailer;