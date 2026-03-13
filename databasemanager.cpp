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

bool DatabaseManager::customerExists(const QString &name)
{

}

bool DatabaseManager::itemExists(const QString &name)
{

}

int DatabaseManager::getCustomerID(const QString &name)
{

}

int DatabaseManager::getItemID(const QString &name)
{

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

bool DatabaseManager::initializeSchema()
{
    if (!isConnected()) {
        return false;
    }
}

bool DatabaseManager::connect(const QString &host, const QString &databaseName, const QString &username, const QString &password, int port)
{
    mDatabase.setHostName(host);
    mDatabase.setDatabaseName(databaseName);
    mDatabase.setUserName(username);
    mDatabase.setPort(port);

    if (!mDatabase.open()) {
        qDebug() << "Connection failed: " << mDatabase.lastError().text();
        return false;
    }

    qDebug() << "Connected to MySQL database: " << databaseName;

    return true;
}

void DatabaseManager::disconnect()
{
    if (mDatabase.isOpen()) {
        mDatabase.close();
    }
}

bool DatabaseManager::isConnected() const
{

}

DatabaseManager::DatabaseManager()
{

}
