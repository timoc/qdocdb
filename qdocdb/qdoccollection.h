#ifndef QDOCCOLLECTION_H
#define QDOCCOLLECTION_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QHash>
#include <QList>
#include <QQueue>
#include <QByteArray>

#include "qdockvinterface.h"
#include "qdocdbcommonconfig.h"
#include "qdocutils.h"

typedef struct {
    int id;
    QJsonObject query;
    QJsonObject queryOptions;
    QJsonArray reply;
    bool triggered;
} td_s_observer;

class QDocCollectionTransaction;
class QDocCollectionSnapshot;

class QDocCollection : public QObject {
    Q_OBJECT

    bool isOpened;
    QString baseDir;

    int getLastSnapshotId(unsigned char &snapshotId);
    QList<QByteArray> findLinkKeys(QJsonObject query, QString curPath, int unionLogic);

    QDocCollection(QString collectionDir, QDocdbCommonConfig* commonConfig, bool inMemory = false);

public:
    enum classTypeEnum {
        typeCollection = 0x0,
        typeTransaction = 0x1,
        typeReadOnlySnapshot = 0x2
    };

protected:
    classTypeEnum classType;
    unsigned char snapshotId;

    QDocdbCommonConfig* commonConfig;
    QDocKVInterface* kvdb;
    QString lastError;

    QHash<QString, QString> getIndexes();

    QHash<int, td_s_observer> observers;

    inline QByteArray constructDocumentLinkKey(QByteArray id);
    static inline QByteArray constructDocumentLinkKey(QByteArray id, unsigned char snapshotId);
    inline QByteArray constructDocumentLinkKey(QString id);
    static inline QByteArray constructDocumentStartKey(QByteArray id);

    inline QByteArray constructIndexValueKey(QString indexName, QByteArray valueData);
    inline QByteArray constructIndexValueKey(QString indexName, QJsonValue jsonValue);
    static inline QByteArray constructIndexValueStartKey(QString indexName, unsigned char snapshotId);

    int getIndexValueLinkKeys(QString indexName, QJsonValue jsonValue, QList<QByteArray>& linkKeys);
    int getValueByLinkKey(QByteArray linkKey, QJsonValue& value);

    int find(QJsonObject query, QJsonArray* pReply, QList<QByteArray>& ids, QJsonObject options = QJsonObject());
    void applyOptions(QJsonArray* pReply, QList<QByteArray>* pIds, QJsonObject options);
    int checkValidR(QJsonValue queryPart, QJsonValue docPart, QString curPath, bool& valid);

    QJsonValue getJsonValue(QByteArray id);
    QJsonValue getJsonValue(QByteArray id, unsigned char& snapshotId, bool& isNotModifiedLater);
    QJsonValue getJsonValue(QByteArray id, unsigned char& snapshotId, bool& isNotModifiedLater, bool& isNaked);
    QJsonValue getJsonValueByLinkKey(QByteArray linkKey);

public:
    enum resultEnum {
        success = 0x0,
        errorQuery = 0x1,
        errorInvalidObject = 0x2,
        errorAlreadyExists = 0x3,
        errorDatabase = 0x4,
        errorSnapshotIsExists = 0x5,
        errorType = 0x6,
        errorObjectIsReadOnly = 0x7
    };

    // document iterator
    class iterator {
        QDocKVIterator *kvit;
        unsigned char snapshotId;
        QByteArray beginKey;
        QByteArray prefix;
        void setupPrefix();

    public:
        QByteArray key();
        QJsonValue value();
        QJsonValue value(unsigned char& snapshotId, bool& isNotModifiedLater);
        QJsonValue value(unsigned char& snapshotId, bool& isNotModifiedLater, bool &isNaked);
        bool isModifiedLater();
        void seekToFirst();
        void seek(QByteArray id);
        void next();
        bool isValid();
        iterator(QDocKVIterator* kvit, unsigned char snapshotId);
        ~iterator();
    };
    iterator* newIterator();

    void reloadObservers();
    void emitObservers();

    QString getLastError();
    QString getBaseDir();
    QDocdbCommonConfig* getCommonConfig();

    QDocKVInterface* getKVDB();
    unsigned char getSnapshotId();
    QDocCollection::classTypeEnum getClassType();
    QHash<int, td_s_observer>* getObservers();

    // API
    int find(QJsonObject query, QJsonArray* pReply, QJsonObject options = QJsonObject());
    int findOne(QJsonObject query, QJsonObject* pReply, QJsonObject options = QJsonObject());
    int count(QJsonObject query, int& replyCount, QJsonObject options = QJsonObject());
    int observe(QJsonObject query, QJsonObject queryOptions, int preferedId = -1);
    int unobserve(int);
    QDocCollectionTransaction* newTransaction();
    int newSnapshot(QString snapshotName);
    QDocCollectionSnapshot* getSnapshot(QString shapshotName);
    int getSnapshotId(QString snapshotName, unsigned char& snapshotId);
    int revertToSnapshot(QString snapshotName);
    int removeSnapshot(QString snapshotName);
    int getModified(QList<QByteArray>& ids);

    int getSnapshotsValue(QList<QString>& snapshots);
    // ---
    // debug
    int printAll();

    static QDocCollection* open(QString collectionDir, QDocdbCommonConfig* commonConfig, bool inMemory = false);

    QDocCollection(QDocKVInterface* kvdb, QString collectionDir, QDocdbCommonConfig* commonConfig, classTypeEnum classType);
    ~QDocCollection();

signals:
    void observeQueryChanged(int, QJsonArray&);

public slots:
    void emitObserver(int observerId);
};

class QDocCollectionTransaction : public QDocCollection {

    QDocCollection* parentColl;
    QHash<int, td_s_observer>* baseObservers;

    int addIndexValue(QString indexName, QJsonValue jsonValue, QByteArray id);
    int addIndexValue(QString indexName, QJsonValue jsonValue, QList<QByteArray> ids);
    int removeIndexValue(QString indexName, QJsonValue jsonValue, QByteArray linkKey);
    int putJsonValue(QByteArray id, QJsonValue jsonValue);

public:
    enum resultTransactionEnum {
        errorWriteTransaction = 0x101
    };

    int putIndexes(QHash<QString, QString>&);
    int putSnapshotsValue(QList<QString>& snapshots);
    int copyIndexValues(unsigned char dstSnapshotId, unsigned char srcSnapshotId);

    // API
    int set(QJsonObject& query, QJsonArray& docs);
    int insert(QJsonObject doc, QByteArray& id, bool overwrite = false, bool ignoreReadOnlyValue = false);
    int remove(QJsonObject& query);
    int removeById(QByteArray id, bool ignoreReadOnlyValue = false);
    int createIndex(QString fieldName, QString indexName);
    int writeTransaction();
    // ---

    QDocCollectionTransaction(QDocCollection* pColl);
};

class QDocCollectionSnapshot : public QDocCollection {
    QDocCollection* parentColl;

public:
    int getModified(QList<QByteArray>& ids);
    QDocCollectionSnapshot(QDocCollection* pColl, unsigned char);
};

/*
 *  --- Documents (HH - snapshot number, min \x00, max \xFF):
 *  d:DOCUMENT_ID:             -> empty
 *  d:DOCUMENT_ID:\xHH         -> serialized QJsonObject
 *  d:DOCUMENT_ID:\xHH         -> serialized QJsonObject
 *  --- Indexes
 *  in:                        -> serialized QHash<QString, QString> [fieldName] = indexName
 *
 *  iv:INDEX_NAME:\xHH:        -> empty
 *  iv:INDEX_NAME:\xHH:VALUE   -> links to keys in serialized QList<QByteArray>[ "d:DOCUMENT_ID:\xHH", ... ]
 *  --- Snapshots
 *  s:                         -> list of snapshots in serialized QList<QString>
*/


#endif // QDOCCOLLECTION_H
