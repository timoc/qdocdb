
#include <QDataStream>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSettings>
#include <QJsonArray>

#include "qdocdbclient.h"

int QDocdbClient::find(QString url, QVariantMap query, QVariantList &reply, QVariantMap queryOptions, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeFind, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);
    dbQuery->set("query", query);
    dbQuery->set("queryOptions", queryOptions);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        reply = this->replies[id]->get("reply").toList();
    }
    return r;
}

int QDocdbClient::findOne(QString url, QVariantMap query, QVariantMap &reply, QVariantMap queryOptions, QString snapshot) {
    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeFindOne, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);
    dbQuery->set("query", query);
    dbQuery->set("queryOptions", queryOptions);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        reply = this->replies[id]->get("reply").toMap();
    }
    return r;
}

int QDocdbClient::count(QString url, QVariantMap query, int &reply, QVariantMap queryOptions, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeCount, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);
    dbQuery->set("query", query);
    dbQuery->set("queryOptions", queryOptions);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        reply = this->replies[id]->get("count").toInt();
    }
    return r;
}

int QDocdbClient::createIndex(QString url, QString fieldName, QString indexName) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeCreateIndex, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("fieldName", fieldName);
    dbQuery->set("indexName", indexName);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::newSnapshot(QString url, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeNewSnapshot, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::revertToSnapshot(QString url, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeRevertToSnapshot, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::removeSnapshot(QString url, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeRemoveSnapshot, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::getSnapshotList(QString url, QStringList& snapshotList) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeGetSnapshotList, &dbQuery);
    dbQuery->set("url", url);
    int r = this->sendAndWaitReply(id, dbQuery);
    snapshotList.clear();
    if (r == QDocdbClient::success) {
        snapshotList = this->replies[id]->get("snapshotList").toStringList();
    }
    delete dbQuery;
    return r;
}

int QDocdbClient::getModified(QString url, QVariantList& docIds, QString snapshot) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeGetModified, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("snapshot", snapshot);
    int r = this->sendAndWaitReply(id, dbQuery);
    docIds.clear();
    if (r == QDocdbClient::success) {
        docIds = this->replies[id]->get("documentIds").toList();
    }
    delete dbQuery;
    return r;
}

int QDocdbClient::newTransaction(QString url) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeNewTransaction, &dbQuery);
    dbQuery->set("url", url);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        return this->replies[id]->get("transactionId").toInt();
    }
    return -1;
}

int QDocdbClient::writeTransaction(int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeWriteTransaction, &dbQuery);
    dbQuery->set("transactionId", transactionId);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::discardTransaction(int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeDiscardTransaction, &dbQuery);
    dbQuery->set("transactionId", transactionId);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::insert(QString url, QVariantMap doc, QByteArray& docId, bool overwrite, bool ignoreReadOnlyValue, int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeInsert, &dbQuery);

    dbQuery->set("url", url);
    dbQuery->set("transactionId", transactionId);
    dbQuery->set("document", doc);
    dbQuery->set("overwrite", overwrite);
    dbQuery->set("ignoreReadOnlyValue", ignoreReadOnlyValue);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        docId = this->replies[id]->get("documentId").toByteArray();
    }
    return r;
}

int QDocdbClient::remove(QString url, QVariantMap query, int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeRemove, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("query", query);
    dbQuery->set("transactionId", transactionId);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::removeId(QString url, QByteArray docId, bool ignoreReadOnlyValue, int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeRemoveId, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("documentId", docId);
    dbQuery->set("transactionId", transactionId);
    dbQuery->set("ignoreReadOnlyValue", ignoreReadOnlyValue);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::set(QString url, QVariantMap query, QVariantList docs, int transactionId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeSet, &dbQuery);
    dbQuery->set("url", url);
    dbQuery->set("query", query);
    dbQuery->set("documents", docs);
    dbQuery->set("transactionId", transactionId);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::exclusiveLock(QString url) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeExclusiveLock, &dbQuery);
    dbQuery->set("url", url);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::unlock(QString url) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeUnlock, &dbQuery);
    dbQuery->set("url", url);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    return r;
}

int QDocdbClient::observe(QObject* sub, QString url, QVariantMap query, QVariantMap queryOptions, int preferedId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeObserve, &dbQuery);

    dbQuery->set("url", url);
    dbQuery->set("query", query);
    dbQuery->set("queryOptions", queryOptions);
    dbQuery->set("preferedId", preferedId);

    this->observeDataLock = true;
    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        int observeId = this->replies[id]->get("observeId").toInt();
        if (!this->observerInfo.contains(observeId)) {
            QDocdbClient::ObserverInfo& info = this->observerInfo[observeId];
            info.sub = sub;
            info.url = url;
            info.query = query;
            info.queryOptions = queryOptions;
        }
        this->subs[observeId] = sub;
        this->observeDataLock = false;
        QMetaObject::invokeMethod(this, "observeDataProcess", Qt::QueuedConnection);
        return observeId;
    } else {
        this->observeDataLock = false;
        return -1;
    }
}

int QDocdbClient::unobserve(int observeId) {

    QDocdbLinkObject* dbQuery;
    int id = this->newLinkObject(QDocdbLinkObject::typeUnobserve, &dbQuery);
    dbQuery->set("observeId", observeId);

    int r = this->sendAndWaitReply(id, dbQuery);
    delete dbQuery;

    if (r == QDocdbClient::success) {
        this->subs.remove(observeId);
        if (this->observerInfo.contains(observeId)) {
            this->observerInfo.remove(observeId);
        }
    }
    return r;
}

void QDocdbClient::subscriberDestroyed(QObject* sub) {
    QList<int> observeIds;
    for (QMap<int, QObject*>::iterator it = this->subs.begin(); it != this->subs.end();) {
        if (it.value() == sub) {
            observeIds.append(it.key());
            it = this->subs.erase(it);
        } else {
            it++;
        }
    }
    foreach (int observeId, observeIds) {
        this->unobserve(observeId);
    }
}

int QDocdbClient::sendAndWaitReply(int id, QDocdbLinkObject* linkObject) {
    this->activeQueries[id] = linkObject;
    this->client->send(linkObject);
    bool ok = linkObject->waitForDone();
    this->activeQueries.remove(id);
    QMetaObject::invokeMethod(this, "destroyReply", Qt::QueuedConnection, Q_ARG(int, id));
    if (!ok) {
        this->lastError = "Error: Timeout waiting reply";
        return QDocdbClient::errorConnection;
    } else {
        QString errorString = this->replies[id]->get("error").toString();
        if (!errorString.isEmpty()) {
            this->lastError = errorString;
            return QDocdbClient::error;
        }
    }
    return QDocdbClient::success;
}

int QDocdbClient::newLinkObject(QDocdbLinkObject::LinkObjectType type, QDocdbLinkObject** linkObject) {
    int id = this->nextFId++;
    *linkObject = QDocdbLinkObject::newLinkObject(id, type);
    return id;
}

int QDocdbClient::connectToServer(QString serverName) {
    this->serverName = serverName;
    const QString key("QDocdbServer/" + serverName);
    QSettings settings(QDOCDB_ORGANIZATION, QDOCDB_APPLICATION);
    QString host = settings.value(key).toString();
    QHostAddress address(host.mid(0, host.indexOf(":")));
    int port = host.mid(host.indexOf(":") + 1).toInt();

    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(address, port, QIODevice::ReadWrite);
    if (!socket->waitForConnected(2000)) {
        delete socket;
        return QDocdbClient::errorConnection;
    }
    this->needConnected = true;
    this->client = new QDocdbLinkBase(socket);
    connect(this->client, SIGNAL(receive(QDocdbLinkObject*)), this, SLOT(receive(QDocdbLinkObject*)), Qt::DirectConnection);
    connect(this->client, SIGNAL(clientRemoved()), this, SLOT(clientRemoved()), Qt::DirectConnection);
    return QDocdbClient::success;
}

bool QDocdbClient::isConnected() {
    return (this->client != NULL);
}

void QDocdbClient::receive(QDocdbLinkObject* linkObject) {
    if (linkObject->getType() == QDocdbLinkObject::typeObserveData) {
        int observeId = linkObject->get("observeId").toInt();
        QJsonArray reply = QJsonArray::fromVariantList(linkObject->get("reply").toList());
        this->observeData.enqueue(ObserveData({ observeId, reply }));
        QMetaObject::invokeMethod(this, "observeDataProcess", Qt::QueuedConnection);
        delete linkObject;
        return;
    }
    int id = linkObject->get("id").toInt();
    if (this->activeQueries.contains(id)) {
        this->replies[id] = linkObject;
        emit this->activeQueries[id]->done();
    } else {
        delete linkObject;
    }
}

void QDocdbClient::observeDataProcess() {
    if (this->observeDataLock) return;
    while (this->observeData.count()) {
        ObserveData data = this->observeData.dequeue();
        QMetaObject::invokeMethod(this->subs[data.observeId], "observeQueryChanged",
            Qt::DirectConnection,
            Q_ARG(int, data.observeId),
            Q_ARG(QJsonArray, data.reply));
    }
}

void QDocdbClient::destroyReply(int id) {
    if (this->replies.contains(id)) {
        delete this->replies[id];
        this->replies.remove(id);
    }
}

void QDocdbClient::disconnectFromServer() {
    this->needConnected = false;
    if (this->client != NULL) {
        this->client->close();
    }
}

void QDocdbClient::cancelAllActiveQueries() {
    foreach(int id, this->activeQueries.keys()) {
        if (!this->replies.contains(id)) {
            this->replies[id] = new QDocdbLinkObject();
        }
        this->replies[id]->set("error", "canceled");
        emit this->activeQueries[id]->done();
    }
}

void QDocdbClient::clientRemoved() {
    QDocdbLinkBase* client = (QDocdbLinkBase*)this->sender();
    if (this->client == client) {
        disconnect(this->client, SIGNAL(receive(QDocdbLinkObject*)), this, SLOT(receive(QDocdbLinkObject*)));
        disconnect(this->client, SIGNAL(clientRemoved()), this, SLOT(clientRemoved()));
        this->client = NULL;
        this->cancelAllActiveQueries();
    }
    this->subs.clear();

    if (this->needConnected) {
        this->connectTimer.setInterval(this->nextConnectWaitTimeout);
        this->connectTimer.start();
    }
}

void QDocdbClient::onConnectTimer() {
    this->connectTimer.stop();
    if (this->connectToServer(this->serverName) == QDocdbClient::success) {
        this->nextConnectWaitTimeout = 1000;
        for (QMap<int, ObserverInfo>::iterator it = this->observerInfo.begin(); it != this->observerInfo.end(); it++) {
            ObserverInfo& info = it.value();
            this->observe(info.sub, info.url, info.query, info.queryOptions, it.key());
        }
    } else {
        this->connectTimer.setInterval(this->nextConnectWaitTimeout);
        if (this->nextConnectWaitTimeout < 60000) {
            this->nextConnectWaitTimeout += 5000;
        }
        this->connectTimer.start();
    }
}

QDocdbClient::QDocdbClient() {
    qRegisterMetaType<QJsonArray>("QJsonArray&");
    this->client = NULL;
    this->nextFId = 1;
    this->needConnected = false;
    this->nextConnectWaitTimeout = 1000;
    this->observeDataLock;
    connect(&this->connectTimer, SIGNAL(timeout()), this, SLOT(onConnectTimer()));
}

QDocdbClient::~QDocdbClient() {
    if (this->client != NULL) {
        disconnect(this->client, SIGNAL(receive(QDocdbLinkObject*)), this, SLOT(receive(QDocdbLinkObject*)));
        disconnect(this->client, SIGNAL(clientRemoved()), this, SLOT(clientRemoved()));
        this->client = NULL;
        this->cancelAllActiveQueries();
    }
}
