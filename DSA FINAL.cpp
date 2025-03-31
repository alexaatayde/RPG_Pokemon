#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
#define DELAY_MS 50  

// Struct for HeroClass
typedef struct {
    char name[20];
    float hpMultiplier;
    float attackMultiplier;
    float defenseMultiplier;
} HeroClass;

// constants
const HeroClass technomancer = {"Technomancer", 0.8, 1.5, 0.7};
const HeroClass rogue = {"Rogue", 1.0, 1.3, 1.0};
const HeroClass barbarian = {"Barbarian", 1.2, 1.1, 1.3};

// Struct for Inventory Item
typedef struct InventoryItem {
    char name[20];
    struct InventoryItem* next;
} InventoryItem;

// Struct for Character
typedef struct {
    char name[30];
    int level;
    int hp;
    int attack;
    int defence;
    int exp;
    HeroClass classType;
    InventoryItem* inventory;
    // Skills related flags
    int skillActive;  
    float skillMultiplier;  
    int turnsRemaining; 
} Character;

// Struct for Enemy
typedef struct {
    char name[30];
    int hp;
    int attack;
    int defence;
    int hasPoison;
    int itemDropChance; 
} Enemy;

// Function for text animation delay
void textDelay(const char *text, int delay_ms) {
    while (*text) {
        putchar(*text++);
        fflush(stdout);
        usleep(delay_ms * 100);
    }
}

// Function to add item to inventory
void addItem(Character* player, const char* itemName) {
    InventoryItem* newItem = (InventoryItem*)malloc(sizeof(InventoryItem));
    if (!newItem) return;
    strcpy(newItem->name, itemName);
    newItem->next = player->inventory;
    player->inventory = newItem;
    textDelay("You found an item: ", DELAY_MS);
    textDelay(itemName, DELAY_MS);
    textDelay("!\n", DELAY_MS);
}

// Function to simulate rolling dice
int roll_dice(int num, int sides) {
    int total = 0;
    for (int i = 0; i < num; i++) {
        total += (rand() % sides) + 1;
    }
    return total;
}

// Function to create character
Character createCharacter(const char *name, int level, HeroClass classType) {
    Character c;
    strcpy(c.name, name);
    c.level = level;
    c.hp = (100 + (level * 10)) * classType.hpMultiplier;
    c.attack =  (roll_dice(2, 6) + 5) * classType.attackMultiplier;
   c.defence = (roll_dice(1, 5) + 2) * classType.defenseMultiplier;

    c.exp = 0;
    c.classType = classType;
    c.inventory = NULL;
    c.skillActive = 0;
    c.skillMultiplier = 1.0;
    c.turnsRemaining = 0;
    return c;
}

// Function to generate a random enemy
Enemy generateEnemy() {
    const char *enemyNames[] = {"Goblin", "Dark Mage", "Skeleton", "Venomous Spider"};
    int index = rand() % 4;
    Enemy e;
    strcpy(e.name, enemyNames[index]);
    e.hp = roll_dice(3, 10) + 20;
    e.attack = roll_dice(2, 6) + 5;
    e.defence = roll_dice(1, 5) + 2;
    e.hasPoison = (index == 3); // Only the spider poisons
    e.itemDropChance = 30; // 30% chance to drop an item
    return e;
}

// Function to use an item from inventory
void useItem(Character* player) {
    if (player->inventory == NULL) {
        textDelay("You have no items to use!\n", DELAY_MS);
        return;
    }

    textDelay("Choose an item to use:\n", DELAY_MS);
    InventoryItem* current = player->inventory;
    int itemIndex = 1;
    while (current != NULL) {
        printf("%d. %s\n", itemIndex, current->name);
        current = current->next;
        itemIndex++;
    }

    int choice;
    scanf("%d", &choice);

    
    current = player->inventory;
    for (int i = 1; i < choice; i++) {
        current = current->next;
        if (current == NULL) return;
    }

    if (strcmp(current->name, "Health Potion") == 0) {
        player->hp += 20; // Heal 20
        textDelay("You used a Health Potion! Your HP is now ", DELAY_MS);
        printf("%d.\n", player->hp);
    }

    // Remove item from inventory
    if (current == player->inventory) {
        player->inventory = current->next;
    } else {
        InventoryItem* prev = player->inventory;
        while (prev->next != current) {
            prev = prev->next;
        }
        prev->next = current->next;
    }
    free(current);
}

void combat(Character *player) {
    int round = 1; 
    while (player->hp > 0) {
        printf("Round %d\n", round++);
        Enemy enemy = generateEnemy();
        textDelay("A wild enemy appears!\n", DELAY_MS);

       
        printf("Enemy: %s | HP: %d | Attack: %d | Defense: %d\n", enemy.name, enemy.hp, enemy.attack, enemy.defence);
        printf("%s Stats: HP: %d | Attack: %d | Defense: %d\n", player->name, player->hp, player->attack, player->defence);

       
        while (player->hp > 0 && enemy.hp > 0) {
            textDelay("\nChoose an action:\n1. Attack\n2. Use Item\n3. Use Skill\n4. Run\n", DELAY_MS);
            int choice;
            scanf("%d", &choice);

            int damage = 0;

            if (choice == 1) { // Attack
                damage = (roll_dice(2,7)+ 5)* player->classType
				.attackMultiplier;

             
                if (player->skillActive) {
                    damage *= player->skillMultiplier;
                    printf("\nSkill active! Attack damage is increased!\n");
                    player->skillActive = 0; // Reset skill after use
                }

               
                int finalDamage = damage - enemy.defence;
                if (finalDamage < 0) finalDamage = 0; 

                enemy.hp -= finalDamage;  
                printf("\nYou attack dealing %d damage!\n", finalDamage);
            } else if (choice == 2) { // Use Item
                useItem(player);
            } else if (choice == 3) { // Use Skill
                if (strcmp(player->classType.name, "Barbarian") == 0) {
                 
                    player->skillActive = 1;
                    player->skillMultiplier = 1.5;
                    printf("\nBarbarian's skill activated! Your next attack will deal more damage and may skip the enemy's turn.\n");
                } else if (strcmp(player->classType.name, "Technomancer") == 0) {
                 
                    player->skillActive = 2;
                    player->turnsRemaining = 2;
                    printf("\nTechnomancer's Arcane Shield activated! Damage taken will be reduced for the next 2 turns.\n");
                } else if (strcmp(player->classType.name, "Rogue") == 0) {
                   
                    player->skillActive = 3;
                    player->turnsRemaining = 1;
                    printf("\nRogue's Stealth Strike activated! Increased critical hit chance and reduced damage taken for one turn.\n");
                }
            } else if (choice == 4) { // Run
                if (roll_dice(1, 20) >= 15) {
                    textDelay("You successfully escape!\n", DELAY_MS);
                    return;
                } else {
                    textDelay("You fail to escape! The fight continues.\n", DELAY_MS);
                }
            }

           
            Sleep(1000); 
            system("cls");

            
            printf("\nUpdated Stats:\n");
            printf("Enemy: %s | HP: %d | Attack: %d | Defense: %d\n", enemy.name, enemy.hp, enemy.attack, enemy.defence);
            printf("%s Stats: HP: %d | Attack: %d | Defense: %d\n", player->name,player->hp, player->attack, player->defence);

      
            if (player->skillActive == 2 && player->turnsRemaining > 0) {
                enemy.attack /= 2;  
                player->turnsRemaining--;
                printf("Technomancer's shield is active! Enemy damage is reduced.\n");
            }

         
            if (player->skillActive == 3 && player->turnsRemaining > 0) {
                enemy.attack /= 2;
                player->turnsRemaining--;
                printf("Rogue's stealth is active! Enemy damage is reduced.\n");
            }

         
            if (player->skillActive && rand() % 100 < 40) {  
                textDelay("\nThe enemy's turn is skipped!\n", DELAY_MS);
            } else {
               
                if (enemy.hp > 0) {
                   
                    int enemyDamage = enemy.attack - player->defence;
                    if (enemyDamage < 0) enemyDamage = 0; 

                    player->hp -= enemyDamage;
                    printf("Enemy attacks! dealing %d damage\n", enemyDamage);
                }
            }
        }

       
        if (player->hp > 0) {
            textDelay("\nYou won the battle!\n", DELAY_MS);
            system("cls");  
            player->exp += 10;
            if (player->exp >= 20) {
                player->level++;
                player->exp = 0;
                textDelay("Level up!\n", DELAY_MS);
            }
            if ((rand() % 100) < enemy.itemDropChance) {
                addItem(player, "Health Potion");
            }
        }
    }
}


int main() {
    srand(time(NULL));
    int classChoice;
    Character player;

    textDelay("helo mamili ikaw charaacter!\n\n", DELAY_MS);

    do {
        textDelay("\nChoose a class:\n1. Barbarian\n2. Technomancer\n3. Rogue\n4. Exit\n", DELAY_MS);
        printf("\nEnter your choice: ");
        scanf("%d", &classChoice);
        system("cls");
        
        if (classChoice == 1) 
            player = createCharacter("Hwei Grout", 1, barbarian);
        else if (classChoice == 2) 
            player = createCharacter("Aile Eiks", 1, technomancer);
        else if (classChoice == 3) 
            player = createCharacter("Drew Weigh", 1, rogue);
        else 
            return 0;

        combat(&player);
    } while (player.hp > 0);

    textDelay("Game Over!\n", DELAY_MS);
    return 0;
}          
