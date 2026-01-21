#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace CmakeProject1 {

class InventorySystem {
public:
    static InventorySystem& getInstance();
    
    struct Item {
        std::string id;
        std::string name;
        std::string description;
        int quantity;
        int maxStack;
    };
    
    bool addItem(const std::string& itemId, int quantity = 1);
    bool removeItem(const std::string& itemId, int quantity = 1);
    int getItemCount(const std::string& itemId) const;
    const std::vector<Item>& getItems() const;
    void clearInventory();

private:
    InventorySystem() = default;
    ~InventorySystem() = default;
    InventorySystem(const InventorySystem&) = delete;
    InventorySystem& operator=(const InventorySystem&) = delete;
    
    std::vector<Item> items;
    std::unordered_map<std::string, int> itemQuantities;
};

} // namespace CmakeProject1