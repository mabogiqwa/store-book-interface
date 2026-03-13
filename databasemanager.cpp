#include "databasemanager.h"

DatabaseManager* DatabaseManager::sInstance = nullptr;

DatabaseManager *DatabaseManager::getInstance()
{
    if (!sInstance) {
        sInstance = new DatabaseManager();
    }

    return sInstance;
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
