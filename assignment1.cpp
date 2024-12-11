#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Item
class Item
{
protected:
    string title;
    double price;
    int stock;

public:
    Item(string t, double p, int s) : title(t), price(p), stock(s) {}

    virtual void displayDetails()
    {
        cout << "Item: " << title << ", Price: " << price << ", Stock: " << stock << endl;
    }

    // Function to sell items
    bool sellItem(int quantity)
    {
        if (stock >= quantity)
        {
            stock -= quantity;
            return true;
        }
        else
        {
            cout << "Sorry, We are Out Of Stock !!" << endl;
            return false;
        }
    }

    string getTitle() { return title; }

    double getPrice() { return price; }

    int getStock() { return stock; }

    void restock(int quantity) { stock += quantity; }
};

// Book
class Book : public Item
{
private:
    string author;

public:
    Book(string t, double p, int s, string a) : Item(t, p, s), author(a) {}

    void displayDetails()
    {
        cout << "Book: " << title << ", Author: " << author
             << ", Price: " << price << ", Stock: " << stock << endl;
    }
};

// Magazine
class Magazine : public Item
{
public:
    Magazine(string t, double p, int s) : Item(t, p, s) {}

    void displayDetails()
    {
        cout << "Magazine: " << title << ", Price: " << price
             << ", Stock: " << stock << endl;
    }
};

// Inventory
class Inventory
{
private:
    vector<Item *> items;

public:
    // Adding item
    void addItem(Item &item)
    {
        items.push_back(&item);
    }

    // Displaying all items
    void displayInventory()
    {
        for (int i = 0; i < items.size(); i++)
        {
            items[i]->displayDetails();
        }
    }

    // Finding Item
    Item *findItem(string title)
    {
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i]->getTitle() == title)
            {
                return items[i];
            }
        }
        return nullptr;
    }
};

// StoreManager
class StoreManager
{
public:
    // Adding item to the inventory
    void addItemToInventory(Inventory &inventory, Item &item)
    {
        inventory.addItem(item);
    }

    // Viewing items in the inventory
    void viewInventory(Inventory &inventory)
    {
        cout << "\nManager is viewing the inventory:" << endl;
        ;
        inventory.displayInventory();
    }

    // Greetings
    void thankYou()
    {
        cout << "Thanks For the purchase" << endl;
    }
};

// Cashier
class Cashier
{
public:
    // Processing a sale
    void processSale(Inventory &inventory, string itemName, int quantity, string paymentMethod)
    {
        Item *item = inventory.findItem(itemName);
        if (item != nullptr)
        {
            if (item->sellItem(quantity))
            {
                double totalPrice = quantity * item->getPrice();
                cout << "Sold " << quantity << " of " << itemName
                     << " for Rs." << totalPrice << " using " << paymentMethod << " payment." << endl;
            }
        }
        else
        {
            cout << "Item not found!" << endl;
        }
    }
};

// Customer
class Customer
{
private:
    string name;

public:
    Customer(string n) : name(n) {}

    // Interact with the inventory
    void interactWithInventory(Inventory &inventory)
    {
        cout << "Hello " << name << "! Here are the available items:" << endl;
        inventory.displayInventory();
    }

    // Buy an item
    void buyItem(Cashier &cashier, Inventory &inventory, string itemName, int quantity, string paymentMethod)
    {
        cout << name << " wants to buy " << quantity << " of " << itemName << endl;
        cashier.processSale(inventory, itemName, quantity, paymentMethod);
    }
};

// Main Function
int main()
{
    Inventory storeInventory;
    StoreManager manager;
    Cashier cashier;

    // Creating items
    Book cppBook("ANSI C++ Programming", 500, 10, "E. Balagurusamy");
    Book algoBook("Harry Potter", 1000, 5, "J.K Rowling");
    Magazine techToday("Tech Today", 100, 20);

    // Adding items to inventory
    manager.addItemToInventory(storeInventory, cppBook);
    manager.addItemToInventory(storeInventory, algoBook);
    manager.addItemToInventory(storeInventory, techToday);

    // Manager views inventory
    manager.viewInventory(storeInventory);

    // Customer interaction
    Customer customer("ABC");
    customer.interactWithInventory(storeInventory);

    // Customer buying items
    customer.buyItem(cashier, storeInventory, "ANSI C++ Programming", 1, "Cash");
    customer.buyItem(cashier, storeInventory, "Tech Today", 2, "Online");

    // Display updated inventory
    cout << "Updated Inventory:" << endl;
    storeInventory.displayInventory();

    manager.thankYou();

    return 0;
}
