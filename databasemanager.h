#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

class DatabaseManager
{
public:
    static DatabaseManager* getInstance(); //singleton
    DatabaseManager();
};

#endif // DATABASEMANAGER_H
