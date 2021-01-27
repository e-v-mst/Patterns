#include <iostream>
#include <utility>
#include <unordered_map>
#include <map>
#include <list>

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
    void addRoom(Room*) {
    }
    Room* RoomNo(int){ // to find a room by its number
        return nullptr;
    }
};


// define an interface for building mazes
class MazeBuilder {
public:
    virtual void BuildMaze(){}
    virtual void BuildRoom(int){}
    virtual void BuildDoor(int, int){}
    virtual Maze* GetMaze() {return nullptr;}
};

class MazeGame {
public:
    Maze* CreateMaze(MazeBuilder& builder) {
        builder.BuildRoom(1);
        builder.BuildRoom(2);
        builder.BuildDoor(1, 2);

        return builder.GetMaze();
    }

    Maze* CreateComplexMaze(MazeBuilder& builder) {
        for (int i = 1; i < 1002; i++)
            builder.BuildRoom(i);

        return builder.GetMaze();
    }
};


// Example 1. StandartMazeBuilder. Create simple Maze
class StandartMazeBuilder : public MazeBuilder{
public:
    StandartMazeBuilder(){
        currentMaze = nullptr;
    }
    void BuildMaze() override{

    }
    void BuildRoom(int num) override{
        if (currentMaze->RoomNo(num) == nullptr) {
            Room* room = new Room(num);
            currentMaze->addRoom(room);

            room->SetSide(North, new Wall);
            room->SetSide(South, new Wall);
            room->SetSide(East, new Wall);
            room->SetSide(West, new Wall);
        }
    }
    void BuildDoor(int num1, int num2) override{
        Room* room1 = currentMaze->RoomNo(num1);
        Room* room2 = currentMaze->RoomNo(num2);
        Door* door = new Door(room1, room2);

        room1->SetSide(commonWall(room1, room2), door);
        room2->SetSide(commonWall(room2, room1), door);
    }
    Maze* GetMaze() override{
        return currentMaze;
    }
private:
    Direction commonWall(Room*, Room*){
    }
    Maze* currentMaze;
};

// Example 2. CountingMazeBuilder. Doesn't create something, just count components which could be created
class CountingMazeBuilder : public MazeBuilder{
public:
    CountingMazeBuilder(){
        doorCount = 0;
        roomCount = 0;
    }
    void BuildMaze() override{

    }
    void BuildRoom(int) override{
        ++roomCount;
    }
    void BuildDoor(int, int) override{
        ++doorCount;
    }

    virtual void AddWall(int, Direction) {

    }

    void GetCounts(int& doors, int& rooms) const{
        doors = doorCount;
        rooms = roomCount;
    }
private:
    int doorCount;
    int roomCount;
};



int main()
{
    cout << "--- StandartMazeBuilder ---" << endl;
    Maze *maze;
    MazeGame standartMazeGame;
    StandartMazeBuilder standartBuilder;

    standartMazeGame.CreateMaze(standartBuilder);
    maze = standartBuilder.GetMaze();

    cout << endl;
    cout << "--- CountingMazeBuilder ---" << endl;

    int doors{0};
    int rooms{0};

    MazeGame countingMazeGame;
    CountingMazeBuilder countingBuilder;

    countingMazeGame.CreateMaze(countingBuilder);
    countingBuilder.GetCounts(doors, rooms);

    cout << "There are " <<  rooms << " rooms and " << doors << " doors." << endl;
}
