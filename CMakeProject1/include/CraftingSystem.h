#pragma once
#include <string>
#include <vector>

namespace CmakeProject1 {

class CraftingSystem {
public:
    static CraftingSystem& getInstance();
    
    struct Recipe {
        std::string resultItemId;
        std::vector<std::pair<std::string, int>> ingredients; // itemId, quantity
        int resultQuantity;
    };
    
    bool registerRecipe(const Recipe& recipe);
    bool canCraft(const std::string& recipeId) const;
    bool craftItem(const std::string& recipeId);
    const std::vector<Recipe>& getRecipes() const;
    void clearRecipes();

private:
    CraftingSystem() = default;
    ~CraftingSystem() = default;
    CraftingSystem(const CraftingSystem&) = delete;
    CraftingSystem& operator=(const CraftingSystem&) = delete;
    
    std::vector<Recipe> recipes;
};

} // namespace CmakeProject1