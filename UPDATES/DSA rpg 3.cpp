#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define DELAY_MS 50  

//protoype functions


// Struct for HeroClass
typedef struct {
    char name[20];
    float hpMultiplier;
    float attackMultiplier;
    float defenseMultiplier;
} HeroClass;

// Define constant hero classes
const HeroClass technomancer = {"Alex", 0.8, 1.5, 0.7};
const HeroClass rogue = {"Dewey", 1.0, 1.3, 1.0};
const HeroClass barbarian = {"Ygot", 1.2, 1.1, 1.3};

// Inventory using linked list
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

// Function for text animation delay without borders
void textDelay(const char *text, int delay) {
    int len = strlen(text);
    

    // Print the text with delay
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        usleep(delay * 500);  // Adjust delay as needed
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
    c.hp = (20 + (level * 10)) * classType.hpMultiplier;
    c.attack =  (roll_dice(2, 6) + 5) * classType.attackMultiplier;
   c.defence = (roll_dice(3, 10) + 2 )* classType.defenseMultiplier;

    c.exp = 0;
    c.classType = classType;
    c.inventory = NULL;
    c.skillActive = 0;
    c.skillMultiplier = 1.0;
    c.turnsRemaining = 0;
    return c;
}

// Function to generate a random enemy
Enemy generateEnemy(int specificEnemyIndex) {
    const char *enemyNames[] = {"bruno", "Librong James", "Mang kief mcjobil akiskis III", "Aling Jema", "Mang puto"};
    int index;
	if (specificEnemyIndex >= 0 && specificEnemyIndex <= 4)
	{
	 index = specificEnemyIndex;
	 } 
	 else
	 {
	 	index = rand() % 5;
	 }
    Enemy e;
    strcpy(e.name, enemyNames[index]);
    e.hp = roll_dice(3, 10) + 20;
    e.attack = roll_dice(2, 6) + 5;
    e.defence = roll_dice(1, 5) + 2;
    e.hasPoison = (index == 3); 
    e.itemDropChance = 30; 
    return e;
}
void ManongPuto (Character *player)
{
    Enemy enemy = generateEnemy(4); 
    textDelay("Manong Puto has appeared...\n", DELAY_MS);
	
}
void items (Character* player)
{
	 InventoryItem* current = player->inventory;
	  if (strcmp(current->name, "Puto") == 0) {
        player->hp += 50; 
        textDelay("You ate the Puto! Your HP is now ", DELAY_MS);
        printf("%d.\n", player->hp);
    }
   else if(strcmp(current->name,"Book Shard")==0)
    {
   	 player->skillActive = 99;            
        player->turnsRemaining = 3;         
        textDelay("You used the Book Shard! Incoming damage will be reduced for 3 turns.\n", DELAY_MS);
    
	}
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

  
	items(player);
	
    // Remove item logic 
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

void levelup(Character* player)
{
	if (player->hp > 0) {
            textDelay("\nYou won the battle!\n", DELAY_MS);
            system("cls");  
            player->exp += 10;
            if (player->exp >= 20) {
                player->level++;
                player->exp = 0;
                textDelay("Level up!\n", DELAY_MS);
            }
        }
}
void combat(Character *player, int specificEnemyIndex) {
    int round = 1; 
    int choice;
    	Enemy enemy = generateEnemy(specificEnemyIndex);
        printf("Round %d\n", round++);
        
        textDelay("A wild enemy appears!\n", DELAY_MS);

        
        printf("Enemy: %s | HP: %d | Attack: %d | Defense: %d\n", enemy.name, enemy.hp, enemy.attack, enemy.defence);
        printf("%s Stats: HP: %d | Attack: %d | Defense: %d\n", player->name, player->hp, player->attack, player->defence);

      
        while (player->hp > 0 && enemy.hp > 0) {
            textDelay("\nChoose an action:\n1. Attack\n2. Use Item\n3. Use Skill\n4. Run\n", DELAY_MS);
            int choice;
            scanf("%d", &choice);

            int damage = 0;

            if (choice == 1) { 
                damage = (roll_dice(3, 7) + 5) * player->classType.attackMultiplier;

                
                if (player->skillActive) {
                    damage *= player->skillMultiplier;
                    printf("\nSkill active! Attack damage is increased!\n");
                    player->skillActive = 0; 
                }

               
                int finalDamage = damage - enemy.defence;
                if (finalDamage < 0) finalDamage = 0; 

                enemy.hp -= finalDamage;  
                printf("\nYou attack dealing %d damage!\n", finalDamage);
            } else if (choice == 2) { // Use Item
                useItem(player);
            } else if (choice == 3) { // Use Skill
                if (strcmp(player->classType.name, "Ygot") == 0) {
                   printf("choose a skill\n");
                   printf("1. cutting \n\n(skill description: 40% chance to stagger enemy attack while increasing dmg in the next turn)");
                   scanf("%d",&choice);
                   if (choice == 1)
                   {
                   	player->skillActive = 1;
                    player->skillMultiplier = 1.5;
                    printf("\nYgot's skill activated! Your next attack will deal more damage and may skip the enemy's turn.\n");
				   }
                    
                } else if (strcmp(player->classType.name, "Alex") == 0) {
                    
                    player->skillActive = 2;
                    player->turnsRemaining = 2;
                    printf("\nAlex's Arcane Shield activated! Damage taken will be reduced for the next 2 turns.\n");
                } else if (strcmp(player->classType.name, "Dewey") == 0) {
                    
                    player->skillActive = 3;
                    player->turnsRemaining = 1;
                    printf("\nDewey's Stealth Strike activated! Increased critical hit chance and reduced damage taken for one turn.\n");
                }
            } else if (choice == 4) { //run
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

          
            if ((player->skillActive == 2 || player->skillActive == 99) && player->turnsRemaining > 0)
 {
                enemy.attack /= 2; 
                player->turnsRemaining--;
                printf("Alex's shield is active! Enemy damage is reduced.\n");
            }

      
            if (player->skillActive == 3 && player->turnsRemaining > 0) {
                enemy.attack /= 2;  
                player->turnsRemaining--;
                printf("Dewey's stealth is active! Enemy damage is reduced.\n");
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

        levelup(player);
        
            
        
    
}


void die(Character *player) {
    textDelay("\nYou succumb to your fate...\n", DELAY_MS);
    textDelay("The journey ends here.\n", DELAY_MS);
    player->hp = 0;
	
}

void story(Character *player, Enemy* enemy) {
    int choice;
    
    //encounter 1
    textDelay("You reach for the door with contempt to finish your task...", DELAY_MS);
    textDelay("\nYou bid your family goodbye as they send you off\nthey watch you walk outside with coins clinking each step\n        this is your journey and yours alone\n", DELAY_MS);
    printf("\npress any key to continue...");
    getch();
    system("cls");
    textDelay("As you step out, you are greeted with the fresh aroma eminating from someone.\n", DELAY_MS);
    textDelay("a guy dressed in a casual attire walks up with a basket full of freshly baked puto, he offers you one\n", DELAY_MS);
    printf("-------------------------");
    textDelay("\n|	Do you accept?	|\n", DELAY_MS);
    textDelay("\n|	1.Yes	    	|\n", DELAY_MS);
    textDelay("\n|	2.No	    	|\n", DELAY_MS);
	printf("-------------------------\n");
    scanf("%d", &choice);
    system("cls");
    if (choice == 1) {
    	Enemy enemy = generateEnemy(4);
        textDelay("\nManong Puto reaches for his icepick to harvest your organs as currency... He lunges.\n\n", DELAY_MS);
        combat(player,4);
         addItem(player, "Puto");
        if(player->hp == 0) 
		{
		return die(player);	
		}
    } else {
    	textDelay("you refused his offer, you noticed his expression dejected and hurt\nbut still thanked you for time..he leaves\n", DELAY_MS);
    	printf("\npress any key to continue...");
    getch();
    system("cls");
    }
    
    	textDelay("You continue onwards trekking towards your task\n", DELAY_MS);
    	Sleep(1000); 
    	system("cls");
    //scene 2 
    	textDelay("you arrive at your barangay,greeted by the townsfolk\n but one particular individual caught your attention", DELAY_MS);
    	textDelay("\nthe notorious book peddler james, no one knows how he eraned that\ntitle but the townfolks seemed to be weary near the guy", DELAY_MS);
    	textDelay("\nHe looks at you scanning you from top to bottom seemingly frustrated at your CIT uniform", DELAY_MS);
    	printf("\npress any key to continue");
    	getch();
    	system("cls");
 		textDelay("I hate bus attired outfits.. he mumbled.\n", DELAY_MS);
 		combat(player,2);
 		addItem(player,"Book Shard");
 		
        	
}


int main() {
    srand(time(NULL));
    int classChoice;
    Character player;
	Enemy enemy;
	printf("------------------------------");
    textDelay("\n|	Darkthru ponti!	     |\n", DELAY_MS);
    printf("------------------------------");

    do {
        textDelay("\nChoose a class:\n1. Ygot \n2. Alex\n3. Dewey\n4. Exit\n", DELAY_MS);
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
		
		story(&player,&enemy);
      	break;
    } while (player.hp > 0);

    textDelay("Game Over!\n", DELAY_MS);
    return 0;
}
