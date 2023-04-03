#include <QCoreApplication>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTcpSocket socket;                                          // Connecting to the server
    socket.connectToHost("localhost", 1234);
    if (!socket.waitForConnected(5000)) {
        qWarning() << "Could not connect to server";
        return 1;
    }


    QByteArray fileData;                                        // Get the file
    qint64 fileSize = 0;
    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_5_0);
    if (socket.waitForReadyRead(5000)) {
        in >> fileSize;
        fileData = socket.read(fileSize);
    }


    QFile file("received_file.txt");                            // Save the file
    if (file.open(QIODevice::WriteOnly)) {
        file.write(fileData);
        file.close();
        qDebug() << "File saved to disk";
    } else {
        qWarning() << "Could not save file to disk";
    }


    socket.close();                                             // Close the connection and exit the program
    return 0;
}
