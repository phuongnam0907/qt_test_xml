#include <QCoreApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QByteArray>
#include <QList>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile *xmlFile;
    quint8 array[3242];
    QXmlStreamReader *xmlReader;
    QByteArray b;
    int length = 0;
    bool isOK;
    QList<bool> listBool;
//    QList< address;
//    QList<quint>value;
    for (int i = 0; i < 13; i++)
        listBool.append(true);
    for (int i = 0; i < 3242; i++) array[i] = 0xFF;

    b.clear();
    xmlFile = new QFile("D:\default_PN5190_R1.xml");
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Cannot open file";
        return 0;
    }
    xmlReader = new QXmlStreamReader(xmlFile);

    // Parse the XML until we reach end of it
    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        // If token is StartElement - read it
        if (token == QXmlStreamReader::StartElement) {
            if ((xmlReader->name() == "Region") && (xmlReader->attributes().value("RegionOffset").isEmpty())) {
                continue;
            }

            if (xmlReader->name() == "Parameter") {
                QString tmpStr = xmlReader->attributes().value("Offset").toString().replace("0x","");
                QString tStr = xmlReader->attributes().value("Value").toString().replace("0x","");
                length = tmpStr.toInt(&isOK, 16);
                QByteArray tb = QByteArray::fromHex(tStr.toLatin1());
                int tbS = tb.size();
                while (tbS > 0) {
                    array[length + tb.size() - tbS] = (quint8)tb.at(tb.size() - tbS);
                    tbS--;
                }
//                qDebug() << "++ Value: " << tmpStr << " - " << QByteArray::fromHex(tmpStr.toLatin1()).toHex();

//                qDebug() << "++ Name: " << xmlReader->attributes().value("Name")
//                         << " - Offset: " << xmlReader->attributes().value("Offset")
//                         << " - Value: " << xmlReader->attributes().value("Value");

//                qDebug() << "Address: " << length << xmlReader->attributes().value("Offset") << " - " << xmlReader->attributes().value("Value") << " - " << b.size();;
            }
        }
    }

    b.resize(3242);
    memcpy(b.data(), &array, 3242);

    int kLen = 254;
    for (int j = 0; j < 13; j++) {
        int m = 0;
        if (j == 12) m = (j)*(254) + 194;
        else m = (j+1)*(254);
        for (int i = j*kLen; i < m; i++)
            if (array[i] != 0xFF) {
                listBool.insert(j,false);
                break;
            }
    }

    for (int i = 0; i < 13; i++) qDebug() << i << ": " << listBool.at(i);
    for (int j = 0; j < 3242; j++)
        qDebug() << QString("%1").arg((quint16)j, 4, 16, QChar('0')).toUpper() << " - " << QString("%1").arg((quint8)b.at(j), 2, 16, QChar('0')).toUpper();
    // Close reader and flush file
    xmlReader->clear();
    xmlFile->close();

//    qDebug() << "//======================================================================//";

//    xmlFile = new QFile("D:\default_PN5190_R1.xml");
//    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)){
//        qDebug() << "Cannot open file";
//        return 0;
//    }
//    xmlReader = new QXmlStreamReader(xmlFile);

//    // Parse the XML until we reach end of it
//    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
//        // Read next element
//        QXmlStreamReader::TokenType token = xmlReader->readNext();
//        // If token is StartElement - read it
//        if (token == QXmlStreamReader::StartElement) {
//            if ((xmlReader->name() == "Region") && (!xmlReader->attributes().value("RegionOffset").isEmpty())) {
//                qDebug() << "Region Name: " << xmlReader->attributes().value("RegionName")
//                         << "Region Offset: " << xmlReader->attributes().value("RegionOffset")
//                         << " - Region Access: " << xmlReader->attributes().value("RegionAccess")
//                         << " - Region Type: " << xmlReader->attributes().value("RegionType");
//                continue;
//            }

//            if (xmlReader->name() == "Protocol") {
//                qDebug() << "++ Protocol Name: " << xmlReader->attributes().value("ProtocolName")
//                         << " - Index: " << xmlReader->attributes().value("ProtocolIndex")
//                         << " - Offset: " << xmlReader->attributes().value("ProtocolOffset");
//                continue;
//            }

//            if (xmlReader->name() == "Register") {
//                qDebug() << "++++ RegisterName: " << xmlReader->attributes().value("RegisterName")
//                         << " - Address: " << xmlReader->attributes().value("RegisterLogicalAddress")
//                         << " - Value: " << xmlReader->attributes().value("RegisterValue");
//            }
//        }
//    }

//    if (xmlReader->hasError()) qDebug() << "Somethings error";

//    // Close reader and flush file
//    xmlReader->clear();
//    xmlFile->close();


    return a.exec();
}
