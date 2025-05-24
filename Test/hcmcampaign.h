/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;

    virtual ~Army();
    int getLF() const;
    void setLF(int v);
    int getEXP() const;
    void setEXP(int v);

    UnitList* getUnitList() const;
};

class LiberationArmy : public Army 
{
public:
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
};

class ARVN : public Army
{
public:
    ARVN(Unit** unitArray, int size, string name, BattleField* battleField);
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
};

class Position
{
private:
    int r, c; // r: chỉ số hàng, c: chỉ số cột

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"

    double distance(const Position& other);
};

class Unit
{
protected:
    int quantity; // số lượng của loại đơn vị quân sự này trong quân lực
    int weight; // trọng số đóng góp vào các loại chỉ số năng lực của quân đội khác nhau
    Position pos; // vị trí của loại đơn vị này trên chiến trận

public:
    Unit(int quantity, int weight, Position pos);
    ~Unit(); // virtual destructor
    virtual int getAttackScore() = 0; //trả về lượng đóng góp của loại đơn vị quân sự cho một loại chỉ số năng lực nhất định (chỉ số
                                    // năng lực là chỉ số LF và EXP của quân quân đội đó).
    Position getCurrentPosition() const; // trả về Position hiện tại của đối tượng di chuyển.
    virtual string str() const = 0; // trả về chuỗi biểu diễn thông tin của đối tượng.

    int getQuantity() const;
    void setQuantity(int q);
    void changeQuantity(int delta);
    int getWeight() const;
    void setWeight(int w);
};

class Vehicle : public Unit {
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight
    , Position pos
    , VehicleType vehicleType);

    int getAttackScore() override;
    string str() const override;

    VehicleType getVehicleType() const;
};

class Infantry : public Unit {
    InfantryType infantryType;

public:
    Infantry(int quantity, int weight
    , Position pos
    , InfantryType infantryType);

    int getAttackScore() override;
    string str() const override;

    bool isSquare(int num);
    int sumDigits(int num);
    int calcDigitalRoot(int num);
    InfantryType getInfantryType() const;
};

class UnitNode {
public:
    Unit* unit;
    UnitNode* next;

    UnitNode(Unit* unit) : unit(unit), next(nullptr) {}
};

class UnitList
{
private:
    int capacity;
    // TODO
    UnitNode* head;
    UnitNode* tail;
    int vehicleCount;
    int infantryCount;

public:
    UnitList(int capacity);
    ~UnitList(); // Huỷ danh sách (không delete Unit*, vì Unit do nơi khác quản lý)
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
    // Kiểm tra xem num có phải số đặc biệt trong cơ số k hay không
    static bool isSpecialNumber(int num, int k);
    // Kiểm tra S = LF + EXP có phải số đặc biệt trong ít nhất một & S: capacity
    // cơ số nguyên tố lẻ < 10 (3, 5, hoặc 7) hay không
    static bool isSpecialS(int S);
    // Tính sức chứa tối đa dựa vào LF và EXP
    static int calcMaxCapacity(int LF, int EXP);
    
    UnitNode* getHead() const;

    bool removeUnit(Unit* unit) {
    UnitNode* prev = nullptr;
    UnitNode* cur  = head;

    while (cur != nullptr) {
        if (cur->unit == unit) {
            // Cập nhật vehicleCount hoặc infantryCount
            if (dynamic_cast<Vehicle*>(unit) != nullptr) {
                --vehicleCount;
            } else if (dynamic_cast<Infantry*>(unit) != nullptr) {
                --infantryCount;
            }

            // Bỏ node ra khỏi list
            if (prev == nullptr) {
                // Xóa head
                head = cur->next;
            } else {
                prev->next = cur->next;
            }

            if (cur == tail) {
                // Nếu xóa tail thì cập nhật lại
                tail = prev;
            }

            delete cur;  // giải phóng node, không delete unit!
            return true;
        }
            prev = cur;
            cur  = cur->next;
        }

        return false;  // không tìm thấy unit
    }
};

class TerrainElement
{
protected:
    Position pos;
public:
    TerrainElement(const Position &pos);
    virtual ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class Road : public TerrainElement {
public:
    Road(const Position &p);
    void getEffect(Army* army) override;
};

class Mountain : public TerrainElement {
public:
    Mountain(const Position &pos);
    void getEffect(Army* army) override;
};

class River : public TerrainElement {
public:
    River(const Position &pos);
    void getEffect(Army* army) override;
};

class Urban : public TerrainElement {
public:
    Urban(const Position &pos);
    void getEffect(Army* army) override;
};

class Fortification : public TerrainElement {
public:
    Fortification(const Position &pos);
    void getEffect(Army* army) override;
};

class SpecialZone : public TerrainElement {
public:
    SpecialZone(const Position &pos);
    void getEffect(Army* army) override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
    // mảng 2 chiều lưu con trỏ các TerrainElement
    TerrainElement*** terrain;
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str() const;

    void getTerrainEffects(Army* army);
};

class Configuration {
private:
    int n_rows;
    int n_cols;
    vector<Position*> arrayForest;
    vector<Position*> arrayRiver;
    vector<Position*> arrayFortification;
    vector<Position*> arrayUrban;
    vector<Position*> arraySpecialZone;
    vector<Unit*> liberationUnits;
    vector<Unit*> ARVNUnits;

    int eventCode;
    vector<Unit*> unitList;

    void readConfigFile(const string& filepath);
    void parsePositions(string arrayStr, vector<Position*>& positions);
    void parseUnitList(const string& value);
public:
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;

    int getRows() const;
    int getCols() const;
    vector<Position*> getArrayForest() const;
    vector<Position*> getArrayRiver() const;
    vector<Position*> getArrayFortification() const;
    vector<Position*> getArrayUrban() const;
    vector<Position*> getArraySpecialZone() const;
    vector<Unit*> getUnitList() const;
    const vector<Unit*>& getLiberationUnits() const;
    const vector<Unit*>& getARVNUnits() const;
    int getEventCode() const;
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
    ~HCMCampaign();
};

#endif