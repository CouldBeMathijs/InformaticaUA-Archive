#include <iostream>
#include "World.h"
#include "Level.h"
#include "TerrainRoom.h"
#include "DungeonRoom.h"
#include "WeaponItem.h"
#include "PotionItem.h"
#include "MonsterCharacter.h"
#include "HeroCharacter.h"

using namespace std;
int oef2() {
    World* world = new World("Shire");
    Level* level = new Level("Brandy Hall");
    Level* level2 = new Level("Entrance");
    world->addLevel(level);
    world->addLevel(level2);
    delete world;
    return 0;
}
int oef3(){
    World* world = new World("Shire");
    Level* level = new Level("Brandy Hall");
    world->addLevel(level);

    TerrainRoom* tR = new TerrainRoom{"The Garden", "sunny"};
    level->addRoom(0, tR);
    DungeonRoom* dR = new DungeonRoom{"Bilbo's bedroom"};
    level->addRoom(2, dR);

    cout<<level->getRoom(2)->toString()<<endl;

    cout<<level->getRoom(0)->toString()<<endl;

    delete world;
    return 0;
}
int oef4() {
    vector<Item*> items = {new PotionItem("An appel", 1), new WeaponItem("The ring of fire", 10, -3)};
    for (auto item: items) {
        cout<<item->toString();
        cout<<endl;
    }
    return 0;
}
int oef5() {
    vector<WeaponItem*> weapon_items = {new WeaponItem("The ring of fire", 10, -3)};
    vector<PotionItem*> potion_items = {new PotionItem("An appel", 1)};

    Character* orc = new MonsterCharacter("Orc", 7, 3, 0);
    Character* bilbo = new HeroCharacter("Bilbo", 6, 4, 2, potion_items, {});
    Character* frodo = new HeroCharacter("Frodo", 7, 3, 0, {}, weapon_items);
    cout << bilbo->toString() << endl;
    cout<<endl;
    cout<<orc->toString()<<endl;
    cout<<endl;
    cout<<frodo->toString()<<endl;

    cout << frodo->getHealthScore() << endl;
    cout << frodo->getDefensePower() << endl;
    cout << frodo->getAttackPower() << endl;

    cout << bilbo->getHealthScore() << endl;
    cout << bilbo->getDefensePower() << endl;
    cout << bilbo->getAttackPower() << endl;
    return 0;
}
int oef6() {
    World *world = new World("Shire (Middle-earth)");
    Level *level = new Level("Brandy Hall");
    world->addLevel(level);
    Room *roomBilbo = new DungeonRoom("The sleeping room of Bilbo");
    level->addRoom(0, roomBilbo);

    PotionItem *apple = new PotionItem("apple", 1);
    WeaponItem *ring = new WeaponItem("The ring of fire", 12, -3);
    vector<Item *> items = {ring, apple};
    roomBilbo->addItems(items);

    HeroCharacter *bilbo = new HeroCharacter("Bilbo Baggings", 6, 4, 2, {}, {});
    MonsterCharacter *orc = new MonsterCharacter("Orc", 7, 3, 2);
    vector<Character *> characters = {bilbo, orc};
    roomBilbo->addCharacters(characters);

    cout << ">>> We enter a room:" << endl;
    cout << roomBilbo->toString() << endl << endl;

    cout << ">>> " << bilbo->getName() << " picks up a ring" << endl << endl;
    roomBilbo->pickUpWeapon(bilbo, ring);
    cout << bilbo->toString() << endl;
    cout << endl;

    cout << ">>> " << bilbo->getName() << " picks up an apple" << endl << endl;
    roomBilbo->usePotion(bilbo, apple);
    cout << ">>> " << "His health is now " << bilbo->getHealthScore() << endl << endl;

    cout << ">>> The room now looks like this:" << endl;
    cout << roomBilbo->toString() << endl;

    cout << ">>> In the room he encounters a vicious creature! He starts fighting it and ..." << endl;
    roomBilbo->fight(bilbo, orc);

    cout << ">>> The room now looks like this:" << endl;
    cout << roomBilbo->toString() << endl;

    delete world;
    return 0;
}
int main() {
    cout << "==Oefening 01: Zie W12Vrij==" << endl;
    std::cout << "==Oefening 02==" << std::endl;
    oef2();
    std::cout << endl << "==Oefening 03==" << std::endl;
    oef3();
    std::cout << endl << "==Oefening 04==" << std::endl;
    oef4();
    std::cout << endl << "==Oefening 05==" << std::endl;
    oef5();
    std::cout << endl << "==Oefening 06==" << std::endl;
    oef6();
    return 0;
}
