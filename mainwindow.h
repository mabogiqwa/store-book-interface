#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>
#include <QApplication>
#include "customer.h"
#include "customermanager.h"
#include "customerdialog.h"
#include "item.h"
#include "itemdialog.h"
#include "itemmanager.h"
#include "transaction.h"
#include "transactiondialog.h"
#include "transactionmanager.h"
#include "transactionmodel.h"
#include "udpbroadcaster.h"
#include "receiverwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onAddCustomer();
    void onAddItem();
    void onCreateTransaction();
    void onRestoreItems()
    {
        int ret = QMessageBox::question(this,"Restore Items","Are you sure you want to restore items from backup? This will replace the current item list.", QMessageBox::Yes, QMessageBox::No);
        //qDebug() << "the question was asked";
        if (ret == QMessageBox::Yes) {
            ItemManager::getInstance()->restoreFromBackup();

            if (!mBroadcaster) {
                mBroadcaster = new UdpBroadcaster(this);
                connect(mBroadcaster, &UdpBroadcaster::broadcastSent, this, &MainWindow::onBroadcastSent);
            }

        }
    }
    void onCreateBackup();
    void onStartBroadcast();
    void onStopBroadcast();
    void onAbout();
    void onHelp();
    void onExit();
    void onOpenReceiver();

    void onTransactionAdded();
    void onBroadcastSent(const QString &data);
private:
    //UI comps
    QTreeView *mTransactionView;
    QTextEdit *mLogTextEdit;
    QSplitter *mSplitter;

    //Model
    TransactionModel *mTransactionModel;

    //Menus
    QMenu *mFileMenu;
    QMenu *mDataMenu;
    QMenu *mNetworkMenu;
    QMenu *mHelpMenu;

    //Actions
    QAction *mAddCustomerAction;
    QAction *mAddItemAction;
    QAction *mCreateTransactionAction;
    QAction *mCreateBackupItemsAction;
    QAction *mRestoreItemsAction;
    QAction *mStartBroadcastAction;
    QAction *mStopBroadcastAction;
    QAction *mExitAction;
    QAction *mAboutAction;
    QAction *mHelpAction;
    QAction *mOpenReceiverAction;
    ReceiverWindow *mReceiverWindow;

    //Toolbar
    QToolBar *mToolBar;

    //Status bar
    QStatusBar *mStatusBar;

    //Network
    UdpBroadcaster *mBroadcaster;

    QString generateXMLData();

    //Setting up UI
    void setupUI();
    void setupMenus();
    void setupToolBar();
    void setupStatusBar();
    void setupConnections();

    void updateActions();
    void logMessage(const QString &message);
};

inline void MainWindow::onCreateBackup()
{

}
#endif // MAINWINDOW_H
