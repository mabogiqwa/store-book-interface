#include "itemmanager.h"
#include "databasemanager.h"
#include <QDebug>

ItemManager* ItemManager::sInstance = nullptr;

ItemManager::ItemManager(QObject *parent)
    : QObject{parent}
{
}

ItemManager* ItemManager::getInstance() {
    if (!sInstance) {
        sInstance = new ItemManager();
    }

    return sInstance;
}

void ItemManager::addItem(const QString &name, Itemtype type)
{
    if (!findItem(name)) {
        Item *item = new Item(name, type);
        mItems.append(item);
        createBackup();
        emit itemAdded(item);
    }
}

QList<Item*> ItemManager::getItems() const
{
    return mItems;
}

Item* ItemManager::findItem(const QString &name) const
{
    for (Item *item : mItems) {
        if (item->getName() == name) {
            return item;
        }
    }
    return nullptr;
}

void ItemManager::createBackup() {
    qDeleteAll(mBackup);
    mBackup.clear();

    for (Item *item : mItems) {
        mBackup.append(new Item(item->getName(), item->getType()));
    }
}

void ItemManager::restoreFromBackup() {
    qDeleteAll(mItems);
    mItems.clear();

    for (Item *backUpItem : mBackup) {
        mItems.append(new Item(backUpItem->getName(), backUpItem->getType()));
    }

    emit itemsRestored();
}

ItemManager::~ItemManager() {
    qDeleteAll(mItems);
    mItems.clear();
    qDeleteAll(mBackup);
    mBackup.clear();
}
