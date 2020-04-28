#include "servercore.h"

#include <QString>
#include <QDebug>
#include <QTcpSocket>

namespace  {const int PORT_NUMBER = 1272;}

ServerCore::ServerCore()
{
	m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &ServerCore::socketReady);
    connect(m_socket, &QTcpSocket::disconnected, this, &ServerCore::socketDisconnect);
}

void ServerCore::startServer()
{
    if(this->listen(QHostAddress::Any, PORT_NUMBER))
		qDebug() << "Listening in  process...";
	else
		qDebug() << "Error listening!";
}

void ServerCore::incomingConnection(qintptr socketDescriptor)
{
	addPendingConnection(m_socket);
	m_socket->setSocketDescriptor(socketDescriptor);

	qDebug() << "Socket number " << socketDescriptor << endl
             << "Client connected.";
	qDebug() << "Send client status CONNECTED.";

	m_socket->write("You are connected.");
}

void ServerCore::socketReady()
{
	if (m_socket->waitForConnected(1000)) {
		m_socket->waitForReadyRead(1000);
		/*
		 * Читаем сообщение от клиента здесь
		 */
    }
}

void ServerCore::socketDisconnect()
{
    qDebug() << "disconnected";
	socketReady(); //Пока что костыль подпирающий то, что сервер после одного подключения прекращает слушать порт
}
