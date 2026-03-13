#include "databasemanager.h"

DatabaseManager* DatabaseManager::sInstance = nullptr;

DatabaseManager *DatabaseManager::getInstance()
{
    if (!sInstance) {
        sInstance = new DatabaseManager();
    }

    return sInstance;
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

bool DatabaseManager::saveTransaction(Transaction *transaction)
{
    mDatabase = QSqlDatabase::addDatabase("QMYSQL");
}

bool DatabaseManager::saveAllTransactions()
{

}

bool DatabaseManager::loadTransactions()
{

}

bool DatabaseManager::saveItem(Item *item)
{

}

bool DatabaseManager::loadItems()
{

}

bool DatabaseManager::connect(const QString &host, const QString &databaseName, const QString &username, const QString &password, int port)
{

}

void DatabaseManager::disconnect()
{

}

DatabaseManager::DatabaseManager()
{

}
