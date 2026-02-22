# Store Management System

A Qt6/C++ desktop application for managing store inventory, customers, and transactions with UDP network broadcasting capabilities.

## Overview

The Store Management System allows store operators to manage customers, catalog items (books and magazines), and create purchase transactions. Transaction data can be broadcast over a local network via UDP, and a built-in receiver window can monitor incoming broadcasts.

## Features

- **Customer Management** – Add and track customers
- **Item Management** – Add books and magazines to the catalog; restore items from backup
- **Transaction Management** – Create transactions linking customers to purchased items with quantities
- **Transaction Tree View** – Visualize all transactions in a hierarchical tree (Customer → Transaction → Items)
- **UDP Broadcasting** – Broadcast transaction data as XML over the local network (port 12345) on a 5-second interval
- **UDP Receiver** – Built-in receiver window to monitor and display incoming XML broadcasts in both raw and formatted views

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Application entry point |
| `mainwindow.cpp` | Main application window, menus, toolbar, and coordination |
| `customer.cpp` | Customer data model |
| `customermanager.cpp` | Singleton manager for customer collection |
| `customerdialog.cpp` | Dialog for adding new customers |
| `item.cpp` | Item data model (Book/Magazine) |
| `transaction.cpp` | Transaction data model (customer + purchased items) |
| `transactionmanager.cpp` | Singleton manager for transaction collection |
| `transactiondialog.cpp` | Dialog for creating new transactions |
| `transactionmodel.cpp` | Qt item model for the transaction tree view |
| `udpbroadcaster.cpp` | UDP broadcast sender (XML over port 12345) |
| `udpreceiver.cpp` | UDP broadcast receiver |
| `receiverwindow.cpp` | Window for displaying received UDP data |

## Tree View Example
![Alt Image](assets/customer-tree-view.png)

## Usage

### Adding Customers
Go to **Data → Add Customer** (or `Ctrl+U`) and enter the customer's name.

### Adding Items
Click **Add Item** in the toolbar and enter a name and type (Book or Magazine).

### Creating Transactions
Click **Create Transaction** in the toolbar. Select a customer, choose items and quantities, then click **Create Transaction**. This option is only enabled once at least one customer and one item exist.
![Alt Image](assets/transaction-dialog.png)

### Restoring Items
Go to **Data → Restore Items** to restore the item catalog from backup.

### Broadcasting Transactions
Click **Start Broadcast** in the toolbar (or go to **Network → Start Broadcasting**). The application will broadcast all transaction data as XML every 5 seconds to the local network on UDP port 12345. Click **Stop Broadcast** to stop.

### Receiving Broadcasts
Go to **Network → Open UDP Receiver** to open the receiver window. Click **Network → Start Listening** inside that window to begin monitoring broadcasts. Received data is displayed in both raw XML and a formatted, readable view.
![Alt Image](assets/udp-receiver.png)
## Network Details

- **Protocol:** UDP
- **Port:** 12345
- **Broadcast address:** 255.255.255.255 (local network broadcast)
- **Interval:** Every 5 seconds
- **Format:** XML

### XML Broadcast Format

```xml
<?xml version="1.0" encoding="UTF-8"?>
<transactions>
  <transaction>
    <customer>John Smith</customer>
    <datetime>2025-04-01T10:30:00</datetime>
    <items>
      <item>
        <name>The Great Gatsby</name>
        <quantity>2</quantity>
      </item>
    </items>
  </transaction>
</transactions>
```

## Architecture

The application uses the **Singleton** pattern for its three manager classes (`CustomerManager`, `ItemManager`, `TransactionManager`), ensuring a single shared data source across all UI components. Qt signals and slots are used throughout for loose coupling between the managers and the UI.

## License

COS3711 Assignment 2 — © 2025
