#include <iostream>
using namespace std;

enum Direction {North, South, East, West};

class MapSite {
public:
    virtual void Enter() = 0;
};

class Room : public MapSite {
public:
    explicit Room (int num) {
        std::cout << "Room number " << num << " constructed" << std::endl;
    }
    MapSite* GetSide(Direction) const;
    void SetSide(Direction, MapSite*) {}

    virtual void Enter() {
        std::cout << "Enter th Room number " << num << std::endl;
    }

private:
    MapSite *_sides[4];
    int num;
};

class Wall : public MapSite {
public:
    Wall(){
        std::cout << "Wall constructed" << std::endl;
    }
    virtual void Enter() {
        std::cout << "Attemp to enter the Wall" << std::endl;
    }
};

class Door : public MapSite {
public:
    explicit Door(Room* = 0, Room* = 0) {
        std::cout << "Door constructed" << std::endl;
    }

    virtual void Enter() {
        std::cout << "Enter the Door" << std::endl;
    }
    Room* otherSideFrom(Room*);

private:
    Room* room1;
    Room* room2;
    bool isOpen;
};

// a class for providing a set of rooms
class Maze {
public:
    Maze() {
        std::cout << "Maze constructed" << std::endl;
    }
    void addRoom(Room*) {}
    Room* RoomNo(int) const; // to find a room by its number
};

// Class for creating maze components. Builds rooms, walls and doors between rooms. For example, you can create using this class
// mazes by reading the maze plan from file
// The maze builder takes a MazeFactory as a parameter -> the programmer can specify the classes of rooms, door walls
class MazeFactory {
public:
    MazeFactory() {
        std::cout << "MazeFactory constructed" << std::endl;
    }

   virtual Maze *makeMaze() const {
       return new Maze;
   }
   virtual Wall *makeWall() const {
       return new Wall;
   }
   virtual Room *makeRoom(int num) const {
       return new Room(num);
   }
   virtual Door *makeDoor(Room *room1, Room *room2) const {
       return new Door(room1, room2);
   }
};

class MazeGame {
public:
    //builds a small maze consisting of only 2 rooms connected by 1 door
    Maze* CreateMaze(const MazeFactory& factory) {
        Maze *maze = factory.makeMaze();
        Room *room1 = factory.makeRoom(0);
        Room *room2 = factory.makeRoom(1);
        Door *door  = factory.makeDoor(room1, room2);

        maze->addRoom(room1);
        maze->addRoom(room2);

        room1->SetSide(North, factory.makeWall());
        room1->SetSide(East, door);
        room1->SetSide(South, factory.makeWall());
        room1->SetSide(West, factory.makeWall());

        room2->SetSide(North, factory.makeWall());
        room2->SetSide(East, factory.makeWall());
        room2->SetSide(South, factory.makeWall());
        room2->SetSide(West, door);

        return maze;
    }
};

class BombedWall : public Wall {
public:
    BombedWall() {
        std::cout << "BombedWall constructed" << std::endl;
    }
};

class RoomWithABomb : public Room {
public:
    explicit RoomWithABomb(int num) : Room(num) {
        std::cout << "RoomWithABomb number " << num << " constructed" << std::endl;
    }
};

class BombedMazeFactory : public MazeFactory {
public:
    BombedMazeFactory() {
        std::cout << "BombedMazeFactory constructed" << std::endl;
    }

    virtual Wall *makeWall() const override {
        return new BombedWall;
    }

    virtual Room* makeRoom(int n) const override{
        return new RoomWithABomb(n);
    }
};

int main() {
    MazeGame game;
    BombedMazeFactory factory;
    game.CreateMaze(factory);

    return 0;
}
