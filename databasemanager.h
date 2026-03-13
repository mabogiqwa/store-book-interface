#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include "transaction.h"
#include "item.h"

class DatabaseManager
{
public:
    static DatabaseManager* getInstance();

    //Transactions
    bool saveTransaction(Transaction *transaction);
    bool saveAllTransactions();
    bool loadTransactions();

    //Items
    bool saveItem(Item *item);
    bool saveAllItems();
    bool loadItems();

    //Schema
    bool initializeSchema();

    //Connections
    bool connect(const QString &host, const QString &databaseName, const QString &username, const QString &password, int port = 3306);
    void disconnect();
    bool isConnected() const;

    bool saveAll();
    bool loadAll();

private:
    explicit DatabaseManager();
    static DatabaseManager *sInstance;
    QSqlDatabase mDatabase;
    ~DatabaseManager();

    //Helper functions
    bool customerExists(const QString &name);
    bool itemExists(const QString &name);
    int getCustomerID(const QString &name);
    int getItemID(const QString &name);
};

#endif // DATABASEMANAGER_H
