#ifndef TKVSDATABASEPOOL_H
#define TKVSDATABASEPOOL_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QMutex>
#include <QBasicTimer>
#include <TKvsDatabase>
#include <TGlobal>

class QSettings;


class T_CORE_EXPORT TKvsDatabasePool : public QObject
{
    Q_OBJECT
public:
    ~TKvsDatabasePool();
    TKvsDatabase database(TKvsDatabase::Type type);
    void pool(TKvsDatabase &database);

    static void instantiate();
    static TKvsDatabasePool *instance();

protected:
    void init();
    QSettings &kvsSettings(TKvsDatabase::Type type) const;
    bool openDatabase(TKvsDatabase &database, TKvsDatabase::Type type, const QString &env) const;
    void timerEvent(QTimerEvent *event);

    static QString driverName(TKvsDatabase::Type type);
    static int maxConnectionsPerProcess();

private:
    Q_DISABLE_COPY(TKvsDatabasePool)
    TKvsDatabasePool(const QString &environment);

    QVector<QMap<QString, QDateTime> > pooledConnections;
    QMutex mutex;
    QString dbEnvironment;
    QBasicTimer timer;
};

#endif // TKVSDATABASEPOOL_H
