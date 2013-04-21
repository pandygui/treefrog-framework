#ifndef TACTIONCONTEXT_H
#define TACTIONCONTEXT_H

#include <QStringList>
#include <QMap>
#include <QSqlDatabase>
#include <TGlobal>
#include <TSqlTransaction>
#include <TKvsDatabase>

class QHostAddress;
class THttpResponseHeader;
class THttpSocket;
class THttpResponse;
class TApplicationServer;
class TTemporaryFile;
class TActionController;


class T_CORE_EXPORT TActionContext
{
public:
    TActionContext(int socket);
    virtual ~TActionContext();

    QSqlDatabase &getSqlDatabase(int id);
    void releaseSqlDatabases();
    TKvsDatabase &getKvsDatabase(TKvsDatabase::Type type);
    void releaseKvsDatabases();
    TTemporaryFile &createTemporaryFile();
    void stop() { stopped = true; }
    QHostAddress clientAddress() const;
    const TActionController *currentController() const { return currController; }
    static TActionContext *current();

protected:
    void execute();
    virtual void emitError(int socketError);
    bool beginTransaction(QSqlDatabase &database);
    void commitTransactions();
    void rollbackTransactions();

    int socketDescriptor() const { return socketDesc; }
    qint64 writeResponse(int statusCode, THttpResponseHeader &header);
    qint64 writeResponse(int statusCode, THttpResponseHeader &header, const QByteArray &contentType, QIODevice *body, qint64 length);
    qint64 writeResponse(THttpResponseHeader &header, QIODevice *body, qint64 length);

    QMap<int, QSqlDatabase> sqlDatabases;
    TSqlTransaction transactions;
    QMap<int, TKvsDatabase> kvsDatabases;
    volatile bool stopped;

private:
    Q_DISABLE_COPY(TActionContext)

    int socketDesc;
    THttpSocket *httpSocket;
    TActionController *currController;
    QList<TTemporaryFile *> tempFiles;
    QStringList autoRemoveFiles;
};

#endif // TACTIONCONTEXT_H
