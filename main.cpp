#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits>
#include <memory>
#include <map>

using namespace std;

 // ============================================================
// SHOESTRIDE: Brand-Affiliated Shoe Store Inventory System
// C++17 | Demonstrates vectors, linked lists, stacks, queues,
// BST, linear/binary search, and bubble/selection/insertion sort.
// ============================================================

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};

struct CartItem {
    int productId;
    int quantity;
};

struct Order {
    int orderId;
    vector<CartItem> items;
    double total;
};

struct Action {
    string description;
    int productId;
    int previousStock;
};

// -------------------- Binary Search Tree --------------------
class ProductBST {
private:
    struct Node {
        int id;
        Node* left;
        Node* right;
        explicit Node(int value) : id(value), left(nullptr), right(nullptr) {}
    };

      Node* root = nullptr;

    Node* insertNode(Node* node, int id) {
        if (!node) return new Node(id);
        if (id < node->id) node->left = insertNode(node->left, id);
        else if (id > node->id) node->right = insertNode(node->right, id);
        return node;
    }

      bool searchNode(Node* node, int id) const {
        if (!node) return false;
        if (node->id == id) return true;
        return id < node->id ? searchNode(node->left, id)
                             : searchNode(node->right, id);
    }

     void inorderNode(Node* node) const {
        if (!node) return;
        inorderNode(node->left);
        cout << node->id << " ";
        inorderNode(node->right);
    }

     void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    public:
    ProductBST() = default;
    ProductBST(const ProductBST&) = delete;
    ProductBST& operator=(const ProductBST&) = delete;
    ~ProductBST() { destroy(root); }

    void clear() {
        destroy(root);
        root = nullptr;
    }

     void insert(int id) { root = insertNode(root, id); }
    bool search(int id) const { return searchNode(root, id); }

    void displayInorder() const {
        cout << "BST product IDs (in-order): ";
        inorderNode(root);
        cout << '\n';
    }
};

// -------------------- Input Helpers --------------------
int readInt(const string& prompt, int minValue = numeric_limits<int>::min()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a valid whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& prompt, double minValue = 0.0) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a valid amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// -------------------- Main System --------------------
class ShoeStore {
private:
    vector<Product> inventory;
    list<CartItem> cart;                 // Linked-list shopping cart
    stack<Action> actionHistory;        // LIFO action history
    queue<Order> checkoutQueue;         // FIFO checkout queue
    ProductBST productTree;             // BST of product IDs
    int nextOrderId = 1001;

    const vector<string> partnerBrands = {
        "Nike", "Adidas", "Puma", "New Balance"
    };

    Product* findProductById(int id) {
        for (auto& product : inventory)
            if (product.id == id) return &product;
        return nullptr;
    }

    const Product* findProductById(int id) const {
        for (const auto& product : inventory)
            if (product.id == id) return &product;
        return nullptr;
    }

    void recordStockAction(const string& description, const Product& p) {
        actionHistory.push({description, p.id, p.stockQuantity});
    }

    void printProduct(const Product& p) const {
        cout << left << setw(7) << p.id
             << setw(25) << p.name
             << setw(17) << p.category
             << setw(16) << p.brand
             << right << setw(11) << fixed << setprecision(2) << p.price
             << setw(9) << p.stockQuantity << '\n';
    }

    void printTableHeader() const {
        cout << left << setw(7) << "ID"
             << setw(25) << "PRODUCT"
             << setw(17) << "CATEGORY"
             << setw(16) << "BRAND"
             << right << setw(11) << "PRICE"
             << setw(9) << "STOCK" << '\n';
        cout << string(85, '-') << '\n';
    }

 void loadSampleProducts() {
        inventory = {
            {101, "Air Force 1 '07", "Lifestyle", "Nike", 5495.00, 12},
            {102, "Air Max SC", "Running", "Nike", 4295.00, 8},
            {201, "Superstar", "Lifestyle", "Adidas", 5200.00, 10},
            {202, "Ultraboost Light", "Running", "Adidas", 9500.00, 6},
            {301, "Suede Classic", "Lifestyle", "Puma", 3900.00, 9},
            {302, "RS-X", "Casual", "Puma", 5600.00, 5},
            {401, "574 Core", "Lifestyle", "New Balance", 4995.00, 7},
            {402, "530", "Casual", "New Balance", 5795.00, 4}
        };
        rebuildBST();
    }
    
     void rebuildBST() {
        productTree.clear();
        for (const auto& p : inventory) productTree.insert(p.id);
    }

   
    void displayInventory() const {
        cout << "\n==================== SHOE INVENTORY ====================\n";
        if (inventory.empty()) {
            cout << "No products in inventory.\n";
            return;
        }
        printTableHeader();
        for (const auto& p : inventory) printProduct(p);
    }
// Linear Search: checks each product until a name match is found.
    void linearSearchByName() const {
        string key = readLine("\nEnter product name or keyword: ");
        bool found = false;
        for (const auto& p : inventory) {
            if (p.name.find(key) != string::npos) {
                if (!found) printTableHeader();
                printProduct(p);
                found = true;
            }
        }
        if (!found) cout << "No product matched that keyword.\n";
    }
 
    // Binary Search: inventory is copied and sorted by ID first.
     void binarySearchById() const {
        int id = readInt("\nEnter product ID: ", 0);
        vector<Product> sorted = inventory;
        sort(sorted.begin(), sorted.end(),
             [](const Product& a, const Product& b) { return a.id < b.id; });

        int low = 0, high = static_cast<int>(sorted.size()) - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (sorted[mid].id == id) {
                printTableHeader();
                printProduct(sorted[mid]);
                return;
            }
             if (sorted[mid].id < id) low = mid + 1;
            else high = mid - 1;
        }
        cout << "Product ID not found.\n";
    }

    void filterByBrand() const {
        cout << "\nPartner brands:\n";
        for (size_t i = 0; i < partnerBrands.size(); ++i)
            cout << i + 1 << ". " << partnerBrands[i] << '\n';
        int choice = readInt("Choose brand (1-4): ", 1);
        if (choice > static_cast<int>(partnerBrands.size())) {
            cout << "Invalid brand choice.\n";
            return;
        }

        const string selected = partnerBrands[choice - 1];
        bool found = false;
        for (const auto& p : inventory) {
            if (p.brand == selected) {
                if (!found) printTableHeader();
                printProduct(p);
                found = true;
            }
        }
        if (!found) cout << "No products currently listed for " << selected << ".\n";
    }
    
     // Bubble Sort: ascending price.
    void bubbleSortByPrice() {
        vector<Product> sorted = inventory;
        for (size_t i = 0; i < sorted.size(); ++i) {
            for (size_t j = 0; j + 1 < sorted.size() - i; ++j) {
                if (sorted[j].price > sorted[j + 1].price)
                    swap(sorted[j], sorted[j + 1]);
            }
        }
        cout << "\nProducts sorted by price (Bubble Sort):\n";
        printTableHeader();
        for (const auto& p : sorted) printProduct(p);
    }

    // Selection Sort: descending stock quantity.
    void selectionSortByStock() {
        vector<Product> sorted = inventory;
        for (size_t i = 0; i < sorted.size(); ++i) {
            size_t best = i;
            for (size_t j = i + 1; j < sorted.size(); ++j)
                if (sorted[j].stockQuantity > sorted[best].stockQuantity)
                    best = j;
            swap(sorted[i], sorted[best]);
        }
        cout << "\nProducts sorted by stock (Selection Sort):\n";
        printTableHeader();
        for (const auto& p : sorted) printProduct(p);
    }

    // Insertion Sort: alphabetical product name.
    void insertionSortByName() {
        vector<Product> sorted = inventory;
        for (size_t i = 1; i < sorted.size(); ++i) {
            Product current = sorted[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && sorted[j].name > current.name) {
                sorted[j + 1] = sorted[j];
                --j;
            }
            sorted[j + 1] = current;
        }
         cout << "\nProducts sorted alphabetically (Insertion Sort):\n";
        printTableHeader();
        for (const auto& p : sorted) printProduct(p);
    }

    void addProduct() {
        cout << "\n=========== ADD NEW SHOE PRODUCT ===========\n";
        int id = readInt("Product ID: ", 1);
        if (findProductById(id)) {
            cout << "That product ID already exists.\n";
            return;
        }

        string name = readLine("Product name: ");
        string category = readLine("Category (Running/Lifestyle/Casual): ");
        cout << "Partner brands: ";
        for (const auto& b : partnerBrands) cout << b << "  ";
        cout << '\n';
        string brand = readLine("Brand (enter exact name): ");
        if (find(partnerBrands.begin(), partnerBrands.end(), brand) == partnerBrands.end()) {
            cout << "Brand is not in the official partner list. Product not added.\n";
            return;
        }
        double price = readDouble("Price (PHP): ");
        int stock = readInt("Stock quantity: ", 0);

        inventory.push_back({id, name, category, brand, price, stock});
        productTree.insert(id);
        cout << "Product successfully added.\n";
    }

    void restockProduct() {
        int id = readInt("\nEnter product ID to restock: ", 1);
        Product* p = findProductById(id);
        if (!p) {
            cout << "Product not found.\n";
            return;
        }
        int oldStock = p->stockQuantity;
        int amount = readInt("Quantity to add: ", 1);
        recordStockAction("Restock", *p);
        p->stockQuantity += amount;
        cout << "Stock updated: " << oldStock << " -> " << p->stockQuantity << '\n';
    }

    void undoLastStockAction() {
        if (actionHistory.empty()) {
            cout << "There is no stock action to undo.\n";
            return;
        }
        Action last = actionHistory.top();
        actionHistory.pop();
        Product* p = findProductById(last.productId);
        if (!p) {
            cout << "The product no longer exists; cannot undo this action.\n";
            return;
        }
        p->stockQuantity = last.previousStock;
        cout << "Undid " << last.description << " for product ID "
             << last.productId << ". Stock restored to "
             << p->stockQuantity << ".\n";
    }

    void addToCart() {
        displayInventory();
        int id = readInt("\nEnter product ID to add to cart: ", 1);
        Product* p = findProductById(id);
        if (!p) {
            cout << "Product not found.\n";
            return;
        }
        int quantity = readInt("Quantity: ", 1);

        int alreadyInCart = 0;
        for (const auto& item : cart)
            if (item.productId == id) alreadyInCart += item.quantity;

        if (alreadyInCart + quantity > p->stockQuantity) {
            cout << "Not enough stock. Available: " << p->stockQuantity - alreadyInCart << '\n';
            return;
        }

        auto it = find_if(cart.begin(), cart.end(),
                          [id](const CartItem& item) { return item.productId == id; });
        if (it != cart.end()) it->quantity += quantity;
        else cart.push_back({id, quantity});

        cout << quantity << " x " << p->name << " added to cart.\n";
    }

    void viewCart() const {
        cout << "\n==================== SHOPPING CART ====================\n";
        if (cart.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }
        double total = 0;
        for (const auto& item : cart) {
            const Product* p = findProductById(item.productId);
            if (!p) continue;
            double subtotal = p->price * item.quantity;
            cout << p->name << " (" << p->brand << ") x " << item.quantity
                 << " = PHP " << fixed << setprecision(2) << subtotal << '\n';
            total += subtotal;
        }
        cout << "TOTAL: PHP " << fixed << setprecision(2) << total << '\n';
    }

    void removeFromCart() {
        if (cart.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }

        int id = readInt("Enter product ID to remove from cart: ", 1);
        auto it = find_if(cart.begin(), cart.end(),
                          [id](const CartItem& item) { return item.productId == id; });
        if (it == cart.end()) {
            cout << "That product is not in your cart.\n";
            return;
        }
        cart.erase(it);
        cout << "Item removed from cart.\n";
    }

    void placeOrder() {
        if (cart.empty()) {
            cout << "Your cart is empty. Add products first.\n";
            return;
        }

        Order order;
        order.orderId = nextOrderId++;
        order.items.assign(cart.begin(), cart.end());
        order.total = 0;
         // Validate stock again at checkout time.
        for (const auto& item : order.items) {
            const Product* p = findProductById(item.productId);
            if (!p || p->stockQuantity < item.quantity) {
                cout << "Checkout failed: insufficient stock for product ID "
                     << item.productId << ".\n";
                return;
            }
            order.total += p->price * item.quantity;
        }

         checkoutQueue.push(order);
        cart.clear();
        cout << "Order #" << order.orderId << " placed in checkout queue.\n"
             << "It will be processed in FIFO order.\n"
             << "Order total: PHP " << fixed << setprecision(2) << order.total << '\n';
    }

    void processNextOrder() {
        if (checkoutQueue.empty()) {
            cout << "No pending orders in the checkout queue.\n";
            return;
        }

        Order order = checkoutQueue.front();
        // Revalidate stock because other queued orders may use the same items.
        for (const auto& item : order.items) {
            const Product* p = findProductById(item.productId);
            if (!p || p->stockQuantity < item.quantity) {
                cout << "Order #" << order.orderId
                     << " cannot be fulfilled due to insufficient stock. Removed from queue.\n";
                checkoutQueue.pop();
                return;
            }
        }

       for (const auto& item : order.items) {
            Product* p = findProductById(item.productId);
            recordStockAction("Order processing", *p);
            p->stockQuantity -= item.quantity;
        }

        checkoutQueue.pop();
        cout << "Order #" << order.orderId << " processed successfully.\n"
             << "Payment total: PHP " << fixed << setprecision(2) << order.total << '\n'
             << "Inventory has been updated.\n";
    }

    void showQueueStatus() const {
        if (checkoutQueue.empty()) {
            cout << "Checkout queue is empty.\n";
            return;
        }
        queue<Order> copy = checkoutQueue;
        cout << "\nPending orders (front to back):\n";
        while (!copy.empty()) {
            cout << "Order #" << copy.front().orderId
                 << " | PHP " << fixed << setprecision(2) << copy.front().total << '\n';
            copy.pop();
        }
    }
    
   void showBST() const {
        productTree.displayInorder();
        int id = readInt("Enter an ID to search in the BST: ", 1);
        cout << (productTree.search(id) ? "ID exists in the BST.\n"
                                        : "ID not found in the BST.\n");
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\n========== ADMIN MENU ==========\n"
                 << "1. Display inventory\n"
                 << "2. Add new shoe product\n"
                 << "3. Restock product\n"
                 << "4. Undo last stock action (Stack)\n"
                 << "5. Process next customer order (Queue)\n"
                 << "6. View pending checkout queue\n"
                 << "7. Display/search Product BST\n"
                 << "0. Back to main menu\n";
            choice = readInt("Enter choice: ", 0);
            switch (choice) {
                case 1: displayInventory(); break;
                case 2: addProduct(); break;
                case 3: restockProduct(); break;
                case 4: undoLastStockAction(); break;
                case 5: processNextOrder(); break;
                case 6: showQueueStatus(); break;
                case 7: showBST(); break;
                case 0: break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }

    void customerMenu() {
        int choice;
        do {
            cout << "\n========== CUSTOMER MENU ==========\n"
                 << "1. Browse all shoes\n"
                 << "2. Filter by partner brand\n"
                 << "3. Search by product name (Linear Search)\n"
                 << "4. Search by product ID (Binary Search)\n"
                 << "5. Sort by price (Bubble Sort)\n"
                 << "6. Sort by stock (Selection Sort)\n"
                 << "7. Sort by name (Insertion Sort)\n"
                 << "8. Add shoe to cart\n"
                 << "9. View cart\n"
                 << "10. Remove item from cart\n"
                 << "11. Place order / checkout\n"
                 << "0. Back to main menu\n";
            choice = readInt("Enter choice: ", 0);
            switch (choice) {
                case 1: displayInventory(); break;
                case 2: filterByBrand(); break;
                case 3: linearSearchByName(); break;
                case 4: binarySearchById(); break;
                case 5: bubbleSortByPrice(); break;
                case 6: selectionSortByStock(); break;
                case 7: insertionSortByName(); break;
                case 8: addToCart(); break;
                case 9: viewCart(); break;
                case 10: removeFromCart(); break;
                case 11: placeOrder(); break;
                case 0: break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }

public:
    ShoeStore() { loadSampleProducts(); }

    void run() {
        cout << "====================================================\n"
             << "                 WELCOME TO SHOESTRIDE              \n"
             << "       Brand-Affiliated Shoe Store & E-Commerce     \n"
             << "====================================================\n";
        int choice;
        do {
            cout << "\n=============== MAIN MENU ===============\n"
                 << "1. Customer / Shopping\n"
                 << "2. Administrator / Inventory Management\n"
                 << "3. View current inventory\n"
                 << "0. Exit program\n";
            choice = readInt("Enter choice: ", 0);
            switch (choice) {
                case 1: customerMenu(); break;
                case 2: adminMenu(); break;
                case 3: displayInventory(); break;
                case 0: cout << "Thank you for visiting SHOESTRIDE!\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    ShoeStore store;
    store.run();
    return 0;
}
