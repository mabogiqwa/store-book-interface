#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mTransactionView(nullptr), mLogTextEdit(nullptr), mSplitter(nullptr), mTransactionModel(nullptr), mBroadcaster(nullptr), mReceiverWindow(nullptr)
{
    mAddCustomerAction = nullptr;
    mAddItemAction = nullptr;
    mCreateTransactionAction = nullptr;
    mRestoreItemsAction = nullptr;
    mStartBroadcastAction = nullptr;
    mStopBroadcastAction = nullptr;
    mExitAction = nullptr;
    mAboutAction = nullptr;
    mHelpAction = nullptr;

    //qDebug() << "MainWindow constructor done";
    setupUI();
    setupMenus();
    setupToolBar();
    setupStatusBar();
    setupConnections();

    setWindowTitle("Store Management System");
    resize(800,600);

    //logMessage("App started");
    updateActions();
}

MainWindow::~MainWindow()
{
    if (mBroadcaster) {
        mBroadcaster->stopBroadcasting();
        delete mBroadcaster;
    }
    if (mReceiverWindow) {
        delete mReceiverWindow;
    }
}

void MainWindow::onAddCustomer()
{
    CustomerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getCustomerName();
        CustomerManager *manager = CustomerManager::getInstance();
        if (!manager) {
            QMessageBox::critical(this, "Error", "CustomerManager::getInstance() returned nullptr");
            return;
        }
        try {
            manager->addCustomer(name);
        } catch (...) {
            QMessageBox::critical(this, "Error", "Exception in addCustomer()");
        }
        //logMessage(QString("Customer added: %1").arg(name));
        mStatusBar->showMessage(QString("Customer '%1' added").arg(name), 3000);
    }
}

void MainWindow::onAddItem()
{
    ItemDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getItemName();
        Itemtype type = dialog.getItemType();

        ItemManager *manager = ItemManager::getInstance();
        if (!manager) {
            QMessageBox::critical(this, "Error", "ItemManager::getInstance() returned nullptr");
            return;
        }

        try {
            manager->addItem(name, type);
            //qDebug() << "Item added successfully";
        } catch (...) {
            QMessageBox::critical(this, "Error", "Exception in addItem()");
        }
        //logMessage(QString("Item added: %1 (%2)").arg(name, type == Itemtype::Book ? "Book" : "Magazine"));
        mStatusBar->showMessage(QString("Item '%1' added").arg(name), 3000);
    }
}

void MainWindow::onCreateTransaction()
{
    TransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Customer *customer = dialog.getSelectedCustomer();
        QList<SelectedItem> selectedItems = dialog.getSelectedItems();

        if (customer && !selectedItems.isEmpty()) {
            Transaction *transaction = new Transaction(customer, QDateTime::currentDateTime());

            for (const SelectedItem &selectedItem : selectedItems) {
                transaction->addItem(selectedItem.item, selectedItem.quantity);
            }

            TransactionManager *manager = TransactionManager::getInstance();
            if (!manager) {
                QMessageBox::critical(this, "Error", "TransactionManager::getInstance() returned nullptr");
                return;
            }
            try {
                manager->addTransaction(transaction);
                //mStatusBar->showMessage(QString("Item '%1' added").arg(name), 3000);
            } catch (...) {
                QMessageBox::critical(this, "Error", "Exception in addTransaction()");
            }
            //logMessage(QString("Transaction created for customer: %1").arg(customer->getName()));
            mStatusBar->showMessage("Transaction created", 3000);
        }
    }
}

void MainWindow::onRestoreItems()
{
    int ret = QMessageBox::question(this,"Restore Items","Are you sure you want to restore items from backup? This will replace the current item list.", QMessageBox::Yes, QMessageBox::No);
    //qDebug() << "the question was asked";
    if (ret == QMessageBox::Yes) {
        ItemManager::getInstance()->restoreFromBackup();
        //logMessage("Items restored from backup");
        //mStatusBar->showMessage("Items restored from backup", 3000);
        //mBroadcaster = new UdpBroadcaster(this);

        if (!mBroadcaster) {
            mBroadcaster = new UdpBroadcaster(this);
            connect(mBroadcaster, &UdpBroadcaster::broadcastSent, this, &MainWindow::onBroadcastSent);
        }

    }
}

void MainWindow::onStartBroadcast()
{
    qDebug() << "onStartBroadcast() executed.";
    if (!mBroadcaster) {
        mBroadcaster = new UdpBroadcaster(this);
        connect(mBroadcaster, &UdpBroadcaster::broadcastSent, this, &MainWindow::onBroadcastSent);
    }

    mBroadcaster->startBroadcasting();

    if (mStartBroadcastAction) {
        mStartBroadcastAction->setEnabled(false);
        qDebug() << "mStartBroadcastAction->setEnabled was set to false";
    }

    if (mStopBroadcastAction) {
        mStopBroadcastAction->setEnabled(true);
        qDebug() << "mStopBroadcastAction->setEnabled was set to true";
    }

    //logMessage("Broadcasting started");
    //mStatusBar->showMessage("Broadcasting started");
}

void MainWindow::onStopBroadcast()
{
    qDebug() << "Clicked on onStopBroadcast()";
    if (mBroadcaster) {
        mBroadcaster->stopBroadcasting();
    }

    if (mStartBroadcastAction) {
        mStartBroadcastAction->setEnabled(true);
        qDebug() << "Start broadcast action enabled";
    }

    if (mStopBroadcastAction) {
        mStopBroadcastAction->setEnabled(false);
        qDebug() << "Stop broadcast action stopped";
    }


    //logMessage("Broadcasting stopped");
    //mStatusBar->showMessage("Broadcasting stopped", 3000);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About Store Management System",
                       "Store Management System v1.0\n\n"
                       "A Qt application for managing store transactions.\n"
                       "Built with Qt6 and C++.\n\n"
                       "COS3711 Assignment 2\n"
                       "© 2025");
}

void MainWindow::onHelp()
{
    QMessageBox::information(this, "Help",
                             "Store Management System Help\n\n"
                             "1. Add customers using Data -> Add Customer\n"
                             "2. Add items using Data -> Add Item\n"
                             "3. Create transactions using Data -> Create Transaction\n"
                             "4. Start broadcasting using Network -> Start Broadcasting\n"
                             "5. Use the UDP Receiver application to receive broadcasts\n\n"
                             "Keyboard Shortcuts:\n"
                             "Ctrl+U - Add Customers\n"
                             "Ctrl+I - Add Item\n"
                             "Ctrl+T - Create Transaction\n"
                             "Ctrl+Q - Exit");
}

void MainWindow::onExit()
{
    close();
}

void MainWindow::onTransactionAdded()
{
    mTransactionModel->refreshModel();
    //mTransactionModel->expandAll();
    //mTransactionView->resizeColumnToContents(0);
    updateActions();
}

void MainWindow::onBroadcastSent(const QString &data)
{
    qDebug() << "onBroadcastSent() was executed?";
    Q_UNUSED(data);
    //logMessage("Transaction data broadcast sent");
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mSplitter = new QSplitter(Qt::Vertical, this);

    //Transaction view
    mTransactionView = new QTreeView(this);
    mTransactionModel = new TransactionModel(this);
    mTransactionView->setModel(mTransactionModel);
    mTransactionView->setRootIsDecorated(true);
    mTransactionView->setAlternatingRowColors(true);
    mTransactionView->header()->setStretchLastSection(true);
    mTransactionView->expandAll();
    mTransactionView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mSplitter->addWidget(mTransactionView);

    mSplitter->setSizes({400,150});

    mainLayout->addWidget(mSplitter);
}

void MainWindow::setupMenus()
{
    mFileMenu = menuBar()->addMenu("&File");

    mExitAction = new QAction("&Exit", this);
    mExitAction->setShortcut(QKeySequence::Quit);
    mExitAction->setStatusTip("Exit the application");
    mFileMenu->addAction(mExitAction);

    mDataMenu = menuBar()->addMenu("&Data");

    mAddCustomerAction = new QAction("&Add Customer", this);
    mAddCustomerAction->setShortcut(QKeySequence("Ctrl+U"));
    mAddCustomerAction->setStatusTip("Add a new customer.");
    mDataMenu->addAction(mAddCustomerAction);

    mDataMenu->addSeparator();

    /*
    mCreateTransactionAction = new QAction("Create &Transaction", this);
    mCreateTransactionAction->setShortcut(QKeySequence("Ctrl+T"));
    mCreateTransactionAction->setStatusTip("Create a new transaction");
    mDataMenu->addAction(mCreateTransactionAction);
    */

    //mDataMenu->addSeparator();

    mRestoreItemsAction = new QAction("&Restore Items", this);
    mRestoreItemsAction->setStatusTip("Restore items from backup");
    mDataMenu->addAction(mRestoreItemsAction);

    //Network Menu
    mNetworkMenu = menuBar()->addMenu("&Network");

    mStartBroadcastAction = new QAction("&Start Broadcasting", this);
    mStartBroadcastAction->setStatusTip("Start broadcasting transaction data");
    mNetworkMenu->addAction(mStartBroadcastAction);

    mStopBroadcastAction = new QAction("&Stop Broadcasting", this);
    mStopBroadcastAction->setStatusTip("Stop broadcasting transaction data");
    mStopBroadcastAction->setEnabled(false);
    mNetworkMenu->addAction(mStopBroadcastAction);

    mNetworkMenu->addSeparator();

    mOpenReceiverAction = new QAction("&Open UDP Receiver", this);
    mOpenReceiverAction->setStatusTip("Open UDP Receiver window");
    mNetworkMenu->addAction(mOpenReceiverAction);
    //Add help menu later
    mHelpMenu = menuBar()->addMenu("&Help");

    mHelpAction = new QAction("&Help", this);
    mHelpAction->setShortcut(QKeySequence::HelpContents);
    mHelpAction->setStatusTip("Show help information");
    mHelpMenu->addAction(mHelpAction);

    mHelpMenu->addSeparator();

    mAboutAction = new QAction("&About", this);
    mAboutAction->setStatusTip("Show info about application");
    mHelpMenu->addAction(mAboutAction);
}

void MainWindow::setupToolBar()
{
    mToolBar = addToolBar("Main Toolbar");

    mAddItemAction = new QAction("Add item", this);
    mToolBar->addAction(mAddItemAction);
    mToolBar->addSeparator();
    mCreateTransactionAction = new QAction("Create transaction", this);
    mToolBar->addAction(mCreateTransactionAction);
    mToolBar->addSeparator();
    mStartBroadcastAction = new QAction("Start broadcast", this);
    mToolBar->addAction(mStartBroadcastAction);
    mStopBroadcastAction = new QAction("Stop broadcast", this);
    mToolBar->addAction(mStopBroadcastAction);
}

void MainWindow::setupStatusBar()
{
    mStatusBar = statusBar();
    mStatusBar->showMessage("ready");
}

void MainWindow::setupConnections()
{
    //menu actions
    if (mAddCustomerAction) {
        connect(mAddCustomerAction, &QAction::triggered, this, &MainWindow::onAddCustomer);
    }
    if (mAddItemAction) {
        connect(mAddItemAction, &QAction::triggered, this, &MainWindow::onAddItem);
    }
    if (mCreateTransactionAction) {
        connect(mCreateTransactionAction, &QAction::triggered, this, &MainWindow::onCreateTransaction);
    }
    if (mRestoreItemsAction) {
        connect(mRestoreItemsAction, &QAction::triggered, this, &MainWindow::onRestoreItems);
    }
    if (mStartBroadcastAction) {
        connect(mStartBroadcastAction, &QAction::triggered, this, &MainWindow::onStartBroadcast);
    }
    if (mStopBroadcastAction) {
        connect(mStopBroadcastAction, &QAction::triggered, this, &MainWindow::onStopBroadcast);
    }
    if (mExitAction) {
        connect(mExitAction, &QAction::triggered, this, &MainWindow::onExit);
    }
    if (mAboutAction) {
        connect(mAboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    }
    if (mHelpAction) {
        connect(mHelpAction, &QAction::triggered, this, &MainWindow::onHelp);
    }
    if (mOpenReceiverAction) {
        connect(mOpenReceiverAction, &QAction::triggered, this, &MainWindow::onOpenReceiver);
    }

    //Manager connections
    TransactionManager *transactionManager = TransactionManager::getInstance();
    if (transactionManager) {
        connect(transactionManager, &TransactionManager::transactionAdded, this, &MainWindow::onTransactionAdded);
    }

    CustomerManager *customerManager = CustomerManager::getInstance();
    if (customerManager) {
        connect(customerManager, &CustomerManager::customerAdded, this, &MainWindow::updateActions);
    }

    ItemManager *itemManager = ItemManager::getInstance();
    if (itemManager) {
        connect(itemManager, &ItemManager::itemAdded, this, &MainWindow::updateActions);
        connect(itemManager, &ItemManager::itemsRestored, this, &MainWindow::updateActions);
    }
}

void MainWindow::onOpenReceiver()
{
    if (!mReceiverWindow) {
        mReceiverWindow = new ReceiverWindow(this);
    }

    mReceiverWindow->show();
    mReceiverWindow->raise();
    mReceiverWindow->activateWindow();
}

void MainWindow::updateActions()
{
    CustomerManager *customerManager = CustomerManager::getInstance();
    ItemManager *itemManager = ItemManager::getInstance();

    if (!customerManager || !itemManager) {
        return;
    }
    bool hasCustomers = !customerManager->getCustomers().isEmpty();
    bool hasItems = !itemManager->getItems().isEmpty();

    if (mCreateTransactionAction) {
        mCreateTransactionAction->setEnabled(hasCustomers && hasItems);
    }

}

void MainWindow::logMessage(const QString &message)
{
    QString timeStamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    mLogTextEdit->append(QString("[%1] %2").arg(timeStamp, message));
}
