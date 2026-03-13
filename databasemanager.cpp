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
    QSqlQuery query(mDatabase);
    query.prepare("SELECT COUNT(*) FROM customers WHERE name = :name");
    query.bindValue(":name",name);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::itemExists(const QString &name)
{
    QSqlQuery query(mDatabase);

    query.prepare("SELECT COUNT(*) FROM items WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

int DatabaseManager::getCustomerID(const QString &name)
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT id FROM customers WHERE name = :name");
    query.bindValue(":name",name);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

int DatabaseManager::getItemID(const QString &name)
{
    QSqlQuery query(mDatabase);
    query.prepare("SELECT id FROM items WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool DatabaseManager::saveTransaction(Transaction *transaction)
{
    if (!transaction || !isConnected())
        return false;

    Customer *customer = transaction->getCustomer();
    if (!customer)
        return false;

    if (!saveCustomer(customer)) //ensures customer record exists in database
        return false;

    int customerId = getCustomerID(customer->getName());
    if (customerId == -1) {
        qDebug() << "Customer id for " << customer->getName() << " could not be found!";

        return false;
    }

}

bool DatabaseManager::saveAllTransactions()
{

}

bool DatabaseManager::loadTransactions()
{

}

bool DatabaseManager::saveCustomer(Customer *customer)
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

    QSqlQuery query(mDatabase);

    // Customers table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS customers ("
            "  id   INT AUTO_INCREMENT PRIMARY KEY,"
            "  name VARCHAR(255) NOT NULL UNIQUE"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"))
    {
        qDebug() << "Failed to create customers table:"
                 << query.lastError().text();
        return false;
    }

    // Items table
    // type stored as INT: 0 = Book, 1 = Magazine (mirrors the Itemtype enum)
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS items ("
            "  id   INT AUTO_INCREMENT PRIMARY KEY,"
            "  name VARCHAR(255) NOT NULL UNIQUE,"
            "  type INT          NOT NULL"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"))
    {
        qDebug() << "Failed to create items table:"
                 << query.lastError().text();
        return false;
    }

    // Transactions table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS transactions ("
            "  id          INT AUTO_INCREMENT PRIMARY KEY,"
            "  customer_id INT      NOT NULL,"
            "  datetime    DATETIME NOT NULL,"
            "  FOREIGN KEY (customer_id) REFERENCES customers(id)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"))
    {
        qDebug() << "Failed to create transactions table:"
                 << query.lastError().text();
        return false;
    }

    // Transaction items (line items) table
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS transaction_items ("
            "  id             INT AUTO_INCREMENT PRIMARY KEY,"
            "  transaction_id INT NOT NULL,"
            "  item_id        INT NOT NULL,"
            "  quantity       INT NOT NULL DEFAULT 1,"
            "  FOREIGN KEY (transaction_id) REFERENCES transactions(id),"
            "  FOREIGN KEY (item_id)        REFERENCES items(id)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"))
    {
        qDebug() << "Failed to create transaction_items table:"
                 << query.lastError().text();
        return false;
    }

    qDebug() << "Schema initialised successfully.";
    return true;
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
