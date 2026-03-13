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

private:
    static DatabaseManager *sInstance;
    QSqlDatabase mDatabase;
};

#endif // DATABASEMANAGER_H
