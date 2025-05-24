#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Position class implementation
Position::Position(int r, int c) 
    : r(r)
    , c(c) {}

static string removeSpace(const string &s) {
    string out;
    for (char c : s) {
        if (c != ' ') {
            out.push_back(c);
        }
    }

    return out;
}

Position::Position(const string &str_pos) {
    string t = removeSpace(str_pos);

    size_t firstParent = t.find('(');
    size_t comma = t.find(',');
    size_t secondParent = t.find(')');

    if (firstParent != string::npos
    && comma != string::npos
    && secondParent != string::npos
    && firstParent < comma
    && comma < secondParent) {
        string rowStr = t.substr(firstParent + 1, comma - firstParent - 1);
        string colStr = t.substr(comma + 1, secondParent - comma - 1);

        r = stoi(rowStr);
        c = stoi(colStr);
    }
    // Sai format 
    else {
        r = 0;
        c = 0;
    }
}

int Position::getRow() const {
    return r;
}

int Position::getCol() const {
    return c;
}

void Position::setRow(int r) {
    this->r = r; // bên trái là member, bên phải là tham số
}

void Position::setCol(int c) {
    this->c = c;
} 

string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

double Position::distance(const Position& other) {
    int dr = abs(r - other.r);
    int dc = abs(c - other.c);
    return sqrt(dr * dr + dc * dc);
}

// Unit class implementation
Unit::Unit(int quantity, int weight, Position pos)
    : quantity(quantity)
    , weight(weight)
    , pos(pos) {}

Unit::~Unit() {}

Position Unit::getCurrentPosition() const {
    return pos;
}

int Unit::getQuantity() const {
    return quantity;
}

void Unit::changeQuantity(int delta) {
    quantity += delta;
}

int Unit::getWeight() const {
    return weight;
}

void Unit::setWeight(int w) {
    this->weight = w;
}

void Unit::setQuantity(int q) {
    this->quantity = q;
}

// Vehicle class implementation
Vehicle::Vehicle(int quantity , int weight , Position pos , VehicleType vehicleType)
    : Unit(quantity, weight, pos)
    , vehicleType(vehicleType) {}

int Vehicle::getAttackScore() {
    int typeValue = static_cast<int>(vehicleType);
    int numerator = typeValue * 304 + quantity * weight;
    return (numerator + 30 - 1) / 30;
}

string Vehicle::str() const {
    string vehType = "";
    switch(vehicleType) {
        case TANK: vehType = "TANK"; break;
        case ARTILLERY: vehType = "ARTILLERY"; break;
        case ARMOREDCAR: vehType = "ARMOREDCAR"; break;
        case APC: vehType = "APC"; break;
        case TRUCK: vehType = "TRUCK"; break;
        case MORTAR: vehType = "MORTAR"; break;
        case ANTIAIRCRAFT: vehType = "ANTIAIRCRAFT"; break;
    }

    return "Vehicle[vehicleType=" + vehType 
    + ",quantity=" + to_string(quantity) 
    + ",weight=" + to_string(weight) 
    + ",position=" + pos.str() + "]";
}

VehicleType Vehicle::getVehicleType() const {
    return vehicleType;
}

// Infantry class implementation
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos)
    , infantryType(infantryType) {}

bool Infantry::isSquare(int num) {
    if (num < 0) return false;

    for (int i = 1; i <= num / i; i++) { // avoid overflow
        if (i * i == num) return true;
    }
    return false;
}

int Infantry::sumDigits(int num) {
    if (num <= 0) {
        return 0;
    }

    return num % 10 + sumDigits(num / 10);
}

int Infantry::calcDigitalRoot(int num) {
    int sum = sumDigits(num) + (1 + 9 + 7 + 5);

    while (sum >= 10) {
        sum = sumDigits(sum);
    }
    return sum;
}

int Infantry::getAttackScore() {
    int typeValue = static_cast<int>(infantryType);
    int score = typeValue * 56 + quantity * weight;

    if (isSquare(weight) && infantryType == SPECIALFORCES) {
        score += 75;

        // int dr = calcDigitalRoot(score);

        // if (dr > 7) {
        //     int inc = (quantity * 20 + 99) / 100;
        //     const_cast<Infantry*>(this)->quantity += inc;
        // } else if (dr < 3) {
        //     int dec = (quantity * 10 + 99) / 100;
        //     const_cast<Infantry*>(this)->quantity -= dec;
        // }

        // score = typeValue * 56 + quantity * weight + 75;
    }

    return score;
}

string Infantry::str() const {
    string infType = "";
    switch(infantryType) {
        case SNIPER: "SNIPER"; break;
        case ANTIAIRCRAFTSQUAD: "ANTIAIRCRAFTSQUAD"; break;
        case MORTARSQUAD: "MORTARSQUAD"; break;
        case ENGINEER: "ENGINEER"; break;
        case SPECIALFORCES: "SPECIALFORCES"; break;
        case REGULARINFANTRY: "REGULARINFANTRY"; break;
    }

    return "Infantry[infantryType=" + infType
    + ",quantity=" + to_string(quantity)
    + ",weight=" + to_string(weight)
    + ",position=" + pos.str() + "]";
}

InfantryType Infantry::getInfantryType() const {
    return infantryType;
}

// UnitList class implementation
UnitList::UnitList(int capacity)
    : capacity(capacity)
    , head(nullptr)
    , tail(nullptr)
    , vehicleCount(0)
    , infantryCount(0) {}

UnitList::~UnitList() {
    UnitNode* cur = head;
    while (cur != nullptr) {
        UnitNode* temp = cur->next;
        delete cur;
        cur = temp;
    }
}

bool UnitList::isSpecialNumber(int num, int k) {
    if (num < 0 || k < 2) return false;

    while (num > 0) {
        int remainder = num % k;

        if (remainder > 1) return false;

        num /= k;
    }

    return true;
}

bool UnitList::isSpecialS(int S) {
    return isSpecialNumber(S, 3)
    || isSpecialNumber(S, 5)
    || isSpecialNumber(S, 7);
}

int UnitList::calcMaxCapacity(int LF, int EXP) {
    return isSpecialS(LF + EXP) ? 12 : 8;
}

bool UnitList::isContain(VehicleType vt) {
    for (UnitNode* p = head; p != nullptr; p = p->next) {
        Vehicle* v = dynamic_cast<Vehicle*>(p->unit); // ép kiểu về Vehicle
        
        if (v != nullptr) {
            if (v->getVehicleType() == vt) {
                return true;
            }
        }
    }

    return false;
}

bool UnitList::isContain(InfantryType it) {
    for (UnitNode* p = head; p != nullptr; p = p->next) {
        Infantry* i = dynamic_cast<Infantry*>(p->unit);

        if (i != nullptr) {
            if (i->getInfantryType() == it) {
                return true;
            }
        }
    }

    return false;
}

bool UnitList::insert(Unit* unit) {
    // Nếu là vehicle
    Vehicle* v = dynamic_cast<Vehicle*>(unit);
    if (v != nullptr) {
        for (UnitNode* p = head; p != nullptr; p = p->next) {
            Vehicle* v2 = dynamic_cast<Vehicle*>(p->unit);

            // Nếu có cùng type -> cộng quantity
            if (v2 != nullptr 
            && v2->getVehicleType() == v->getVehicleType()) {
                v2->changeQuantity(v->getQuantity());
                return true;
            }
        }

        // Nếu đã đầy thì không chèn được
        if (vehicleCount + infantryCount >= capacity) {
            return false;
        } 

        // Chèn cuối 
        UnitNode* node = new UnitNode(unit);
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++vehicleCount;
        return true;
    }

    // Nếu là infantry
    Infantry* inf = dynamic_cast<Infantry*>(unit);
    if (inf != nullptr) {
        for (UnitNode* p = head; p != nullptr; p = p->next) {
            Infantry* inf2 = dynamic_cast<Infantry*>(p->unit);

            if (inf2 != nullptr 
            && inf2->getInfantryType() == inf->getInfantryType()) {
                inf2->changeQuantity(inf->getQuantity());
                return true;
            }
        }

        if (vehicleCount + infantryCount >= capacity) {
            return false;
        }

        UnitNode* node = new UnitNode(unit);
        node->next = head;
        head = node;
        if (tail == nullptr) {
            tail = node;
        }

        ++infantryCount;
        return true;
    }

    return false;
}

string UnitList::str() const {
    std::ostringstream oss;
    oss << "UnitList[count_vehicle=" << vehicleCount
        << ";count_infantry=" << infantryCount << ";";

    bool first = true;
    for (UnitNode* p = head; p != nullptr; p = p->next) {
        if (! first) oss << ",";
        oss << p->unit->str();
        first = false;
    }

    oss << "]";
    return oss.str();
}

UnitNode* UnitList::getHead() const {
    return head;
}

// Army class implementation
int Army::getEXP() const {
    return EXP;
}

void Army::setEXP(int v) {
    EXP = v < 0 ? 0 : (v > 500 ? 500 : v);
}

int Army::getLF() const {
    return LF;
}

void Army::setLF(int v) {
    LF = v < 0 ? 0 : (v > 1000 ? 1000 : v);
}

Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField), LF(0), EXP(0) {

        // Calc original LF/EXP 
        for (int i = 0; i < size; i++) {
            Vehicle* v = dynamic_cast<Vehicle*>(unitArray[i]);
            Infantry* inf = dynamic_cast<Infantry*>(unitArray[i]);

            if (v != nullptr) {
                LF += v->getAttackScore();
            }
            else if (inf != nullptr) {
                EXP += inf->getAttackScore();
            }
        }

        setLF(LF);
        setEXP(EXP);

        // Khỏi tạo UnitList
        int cap = UnitList::calcMaxCapacity(getLF(), getEXP());
        unitList = new UnitList(cap);

        // Copy từng Unit vào List
        for (int i = 0; i < size; i++) {
            Unit* u = const_cast<Unit*>(unitArray[i]);
            Vehicle* v = dynamic_cast<Vehicle*>(u);
            Infantry* inf = dynamic_cast<Infantry*>(u);
            Unit* copy = nullptr;

            if (v != nullptr) {
                copy = new Vehicle(v->getQuantity(),
                                v->getWeight(),
                                v->getCurrentPosition(),
                                v->getVehicleType());       
            }
            else if (inf != nullptr) {
                copy = new Infantry(inf->getQuantity(),
                                inf->getWeight(),
                                inf->getCurrentPosition(),
                                inf->getInfantryType());
            }

            if (copy != nullptr) {
                unitList->insert(copy);
            }
        }

        // Điều chỉnh tất cả Infantry với chỉ số cá nhân 
        UnitNode* cur = unitList->getHead();
        while (cur != nullptr) {
            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);

            if (inf != nullptr) {
                int q0 = inf->getQuantity();
                int personal = inf->calcDigitalRoot(inf->getAttackScore());
                if (personal > 7) {
                    int delta = (q0 * 20 + 99) / 100;
                    inf->changeQuantity(delta);
                }
                else if (personal < 3) {
                    int delta = (q0 * 10 + 99) / 100;
                    inf->changeQuantity(-delta);
                }
            }

            cur = cur->next;
        }

        // Tính lại LF/EXP sau mọi điều chỉnh
        int newLF = 0;
        int newEXP = 0;
        cur = unitList->getHead();
        while (cur != nullptr) {
            Vehicle* v = dynamic_cast<Vehicle*>(cur->unit);
            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);

            if (v != nullptr) {
                newLF += v->getAttackScore();
            }
            else if (inf != nullptr) {
                newEXP += inf->getAttackScore();
            }

            cur = cur->next;
        }

        setLF(newLF);
        setEXP(newEXP);
}

Army::~Army() {
    UnitNode* cur = unitList->getHead();
    while (cur != nullptr) {
        Vehicle* v = dynamic_cast<Vehicle*>(cur->unit);
        Infantry* inf = dynamic_cast<Infantry*>(cur->unit);

        if (v != nullptr) {
            delete v;
        } 
        else if (inf != nullptr) {
            delete inf;
        } 

        cur = cur->next;
    }
    delete unitList;
}

UnitList* Army::getUnitList() const {
    return unitList;
}

// LiberationArmy class implementation
LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name, BattleField* battleField)
    : Army(unitArray, size, name, battleField) {}

// Trả về true nếu tìm thấy, outIdx chứa chỉ số units[] tham gia combo
bool findMinCombo(const vector<Unit*>& arr, int target, vector<int>& outIdx) {
    int n = arr.size();
    vector<int> vals(n);
    int sumMax = 0;
    for (int i = 0; i < n; ++i) {
        vals[i] = arr[i]->getAttackScore();
        sumMax += vals[i];
    }

    const int INF = 1e9;
    vector<int> dp(sumMax + 1, INF), prev(sumMax + 1,-1), prevIdx(sumMax + 1,-1);
    dp[0] = 0;
    for (int i = 0; i < n; ++i) {
        int w = vals[i];

        for (int s = sumMax; s >= w; --s) {
            if (dp[s - w] + 1 < dp[s]) {
                dp[s] = dp[s - w] + 1;
                prev[s] = s - w;
                prevIdx[s] = i;
            }
        }
    }
    // tìm s ≥ target nhỏ nhất có dp[s] != INF
    int best = -1;
    for (int s = target; s <= sumMax; ++s) {
        if (dp[s] != INF) {best = s; break;}
    }

    if (best < 0) return false;
    // hồi ngược lại lấy indices
    for (int s = best; s > 0; s = prev[s]) {
        outIdx.push_back(prevIdx[s]);
    }

    return true;
}

void LiberationArmy::fight(Army *enemy, bool defense) {
    // 1) Nhân LF/EXP lên 1.5 (tấn công) hoặc 1.3 (phòng thủ)
    double factor = defense ? 1.3 : 1.5;
    setLF(static_cast<int>(ceil(getLF() * factor)));
    setEXP(static_cast<int>(ceil(getEXP() * factor)));

    // 2) Chuẩn bị vectors
    vector<Unit*> infs, vehs;
    for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
        if (auto inf = dynamic_cast<Infantry*>(cur->unit)) infs.push_back(inf);

        else if (auto vh = dynamic_cast<Vehicle*>(cur->unit)) vehs.push_back(vh);
    }

    int enemyEXP = enemy->getEXP();
    int enemyLF  = enemy->getLF();

    // 3) Tìm tổ hợp A (infantry) và B (vehicle)
    vector<int> idxA, idxB;
    bool okA = findMinCombo(infs, enemyEXP, idxA);
    bool okB = findMinCombo(vehs, enemyLF, idxB);

    // 4) Xử lý kết quả
    if (okA && okB) {
        // -- thắng: xóa các unit của mình theo idxA, idxB --
        for (int i : idxA) unitList->removeUnit(infs[i]);
        for (int i : idxB) unitList->removeUnit(vehs[i]);
        // -- tịch thu các unit của đối phương cùng loại --
        // tương tự: xóa trên enemy rồi insert vào this->unitList
        vector<Unit*> eInf, eVeh;
        for (UnitNode* cur = enemy->getUnitList()->getHead(); cur; cur = cur->next) {
            if (auto inf = dynamic_cast<Infantry*>(cur->unit)) eInf.push_back(inf);

            else if (auto vh = dynamic_cast<Vehicle*>(cur->unit)) eVeh.push_back(vh);
        }
        // dùng same idxA (vì loại và số lượng giống) để tịch thu eInf[idxA] và eVeh[idxB]
        for (int i : idxA) {
            Unit* u = eInf[i];
            enemy->getUnitList()->removeUnit(u);
            unitList->insert(u);
        }

        for (Unit* u : eVeh) {
            enemy->getUnitList()->removeUnit(u);
            unitList->insert(u);
        }

    } else if (okA || okB) {
        // -- hòa: vẫn thắng nếu chỉ số còn lại (LF hoặc EXP) của mình > của đối phương --
        int remMy = okA ? getEXP() : getLF();
        int remEn = okA ? enemyEXP : enemyLF;

        if (remMy > remEn) {
            // xóa tổ hợp thỏa như trên, và xóa *tất cả* đơn vị không thỏa
            auto &keep = okA ? idxA : idxB;
            auto &eras = okA ? infs : vehs;
            // xóa thỏa
            for (int i : keep) unitList->removeUnit(eras[i]);
            // xóa tất cả k-thỏa
            for (int i = 0; i < (int)eras.size(); ++i) {
                bool isKept = false;

                for (int k = 0; k < (int)keep.size(); ++k) {
                    if (keep[k] == i) {isKept = true; break;}
                }

                if (!isKept) {
                    unitList->removeUnit(eras[i]);
                }
            }
        } else {
            // không giao tranh
            for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
                int w = cur->unit->getWeight();
                cur->unit->setWeight( static_cast<int>(ceil(w * 0.9)));
            }
        }
    } else {
        // -- không có tổ hợp nào: giảm 10% weight tất cả đơn vị --
        for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
            int w = cur->unit->getWeight();
            cur->unit->setWeight( static_cast<int>(ceil(w * 0.9)));
        }
    }

    // 5) Cập nhật lại LF/EXP từ unitList (duyệt lại toàn bộ unitList)
    int sumLF = 0, sumEXP = 0;
    for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
        if (dynamic_cast<Vehicle*>(cur->unit)) sumLF += cur->unit->getAttackScore();
        else                                    sumEXP += cur->unit->getAttackScore();
    }

    setLF(sumLF);
    setEXP(sumEXP);
}

string LiberationArmy::str() const {
    string result = name + "[LF=" + to_string(LF) + 
    ",EXP=" + to_string(EXP) +
    ",unitList=" + unitList->str() +
    ",battleField=";

    if (battleField) {
        result += battleField->str();
    }

    result += "]";
    return result;
}

// ARVN class implementation
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}

void ARVN::fight(Army *enemy, bool defense) {
    // ARVN chỉ số không thay đổi (factor = 1.0)

    // 1) Nếu ARVN tấn công: ARVN luôn thất bại
    if (! defense) {
        // Giảm 20% quantity mỗi đơn vị
        vector<Unit*> toDelete;
        for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
            int newQty = static_cast<int>(ceil(cur->unit->getQuantity() * 0.8));
            cur->unit->setQuantity(newQty);
            // Nếu còn 1 hoặc ít hơn thì xóa
            if (cur->unit->getQuantity() <= 1) {
                toDelete.push_back(cur->unit);
            }
        }
        // Xóa khỏi danh sách
        for (Unit* u : toDelete) {
            unitList->removeUnit(u);
        }
    }
    // 2) Nếu ARVN phòng thủ (defense=true): Liberation tấn công
    else {
        // Tịch thu toàn bộ đơn vị của ARVN sang LiberationArmy
        LiberationArmy* la = dynamic_cast<LiberationArmy*>(enemy);
        vector<Unit*> transfer;
        for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
            transfer.push_back(cur->unit);
        }

        for (Unit* u : transfer) {
            unitList->removeUnit(u);
            la->getUnitList()->insert(u);
        }

        // Phần còn lại (nhưng thực ra không còn unit nào) giảm 20% weight
        for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
            int newW = static_cast<int>(ceil(cur->unit->getWeight() * 0.8));
            cur->unit->setWeight(newW);
        }
    }

    // 3) Cập nhật lại LF và EXP từ unitList
    int sumLF = 0, sumEXP = 0;
    for (UnitNode* cur = unitList->getHead(); cur != nullptr; cur = cur->next) {
        int atk = cur->unit->getAttackScore();

        if (dynamic_cast<Vehicle*>(cur->unit)) sumLF  += atk;
        else                                   sumEXP += atk;
    }

    setLF(sumLF);
    setEXP(sumEXP);
}

string ARVN::str() const {
    string result = name + "[LF=" + to_string(LF) + 
    ",EXP=" + to_string(EXP) + 
    ",unitList=" + unitList->str() + 
    ",battleField=";

    if(battleField){
        result += battleField->str();
    }

    result += "]";
    return result;
}

// Road class implementation
Road::Road(const Position &pos)
    : TerrainElement(pos) {}

void Road::getEffect(Army* army) {
    // không ảnh hưởng gì
}

// Mountain class implementation
Mountain::Mountain(const Position &pos)
    : TerrainElement(pos) {}

void Mountain::getEffect(Army* army) {
    Position center = this->pos;

    // Ktra loại quân đội
    LiberationArmy* liberationArmy = dynamic_cast<LiberationArmy*>(army);
    if (liberationArmy != nullptr) {
        // Quân giải phóng: Bán kính 2 đơn vị
        UnitNode* cur = liberationArmy->getUnitList()->getHead();
        int totalBonusLF = 0;
        int totalBonusEXP = 0;

        while (cur != nullptr) {
            // Khoảng cách từ đơn vị đến địa hình
            double distance = center.distance(cur->unit->getCurrentPosition());

            if (distance <= 2.0) {
                // Đơn vị nằm trong bán kính ảnh hưởng
                Vehicle* vehicle = dynamic_cast<Vehicle*>(cur->unit);
                if (vehicle != nullptr) {
                    // Phương tiện chiến đấu bị trừ 10% attackScore
                    int attackScore = vehicle->getAttackScore();
                    int loss = static_cast<int>(ceil(attackScore * 0.1));
                    totalBonusLF -= loss;
                } else {
                    Infantry* infantry = dynamic_cast<Infantry*>(cur->unit);
                    if (infantry != nullptr) {
                        // Bộ bình được cộng 30% attackScore
                        int attackScore = infantry->getAttackScore();
                        int gain = static_cast<int>(ceil(attackScore * 0.3));
                        totalBonusEXP += gain;
                    }
                }
            }

            cur = cur->next;
        }

        // Cập nhật chỉ số sức mạnh
        liberationArmy->setLF(liberationArmy->getLF() + totalBonusLF);
        liberationArmy->setEXP(liberationArmy->getEXP() + totalBonusEXP);
    } else {
        // Quân đội Sài Gòn : bán kính 4 đơn vị
        ARVN* arvn = dynamic_cast<ARVN*>(army);
        if (arvn != nullptr) {
            UnitNode* cur = arvn->getUnitList()->getHead();
            int totalBonusLF = 0;
            int totalBonusEXP = 0;

            while (cur != nullptr) {
                double distance = center.distance(cur->unit->getCurrentPosition());

                if (distance <= 4.0) {
                    Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);
                    if (veh != nullptr) {
                        int attackScore = veh->getAttackScore();
                        int loss = static_cast<int>(ceil(attackScore * 0.05));
                        totalBonusLF -= loss;
                    } else {
                        Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
                        if (inf != nullptr) {
                            int attackScore = inf->getAttackScore();
                            int gain = static_cast<int>(ceil(attackScore * 0.2));
                            totalBonusEXP += gain;
                        }
                    }
                }

                cur = cur->next;
            }

            arvn->setLF(arvn->getLF() + totalBonusLF);
            arvn->setEXP(arvn->getEXP() + totalBonusEXP);
        }
    }
}

// River class implementation
River::River(const Position& pos) 
    : TerrainElement(pos) {}

void River::getEffect(Army* army) {
    Position center = this->pos;
    // LiverationArmy
    LiberationArmy* li = dynamic_cast<LiberationArmy*>(army);
    if (li != nullptr) {
        UnitNode* cur = li->getUnitList()->getHead();
        while (cur != nullptr) {
            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
            if (inf != nullptr) {
                double d = center.distance(inf->getCurrentPosition());
                if (d <= 2.0) {
                    int attackScore = inf->getAttackScore();
                    int loss = static_cast<int>(ceil(0.1 * attackScore));
                    int newEXP = li->getEXP() - loss;
                    li->setEXP(newEXP);
                }
            }

            cur = cur->next;
        }
        return;
    }

    ARVN* ar = dynamic_cast<ARVN*>(army);
    if (ar != nullptr) {
        UnitNode* cur = ar->getUnitList()->getHead();

        while (cur != nullptr) {
            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);

            if (inf != nullptr) {
                double d = center.distance(inf->getCurrentPosition());
                if (d <= 2.0) {
                    int attackScore = inf->getAttackScore();
                    int loss = static_cast<int>(ceil(0.1 * attackScore));
                    int newEXP = ar->getEXP() - loss;
                    ar->setEXP(newEXP);
                }
            }

            cur = cur->next;
        }
    }
}

// Urban class implementation
Urban::Urban(const Position &pos) 
    :TerrainElement(pos) {}

void Urban::getEffect(Army* army) {
    Position center = this->pos;

    LiberationArmy* li = dynamic_cast<LiberationArmy*>(army);
    if (li != nullptr) {
        int deltaEXP = 0;
        int deltaLF = 0;

        UnitNode* cur = li->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());

            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
            if (inf != nullptr) {
                InfantryType t = inf->getInfantryType();

                if ((t == InfantryType::SPECIALFORCES || t == InfantryType::REGULARINFANTRY)
                    && distance <= 5.0) 
                    {
                        int attackScore = inf->getAttackScore();
                        int gain = static_cast<int>(ceil(2.0 * attackScore / distance));
                        deltaEXP += gain;
                    }
            }
            else {
                Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);
                if (veh != nullptr && veh->getVehicleType() == VehicleType::ARTILLERY
                    && distance <= 2.0) 
                    {
                        int attackScore = veh->getAttackScore();
                        int loss = static_cast<int>(ceil(0.50 * attackScore));
                        deltaLF -= loss;
                    }
            }

            cur = cur->next;
        }

        li->setEXP(li->getEXP() + deltaEXP);
        li->setLF(li->getLF() + deltaLF);
        return;
    }

    ARVN* ar = dynamic_cast<ARVN*>(army);
    if (ar != nullptr) {
        int deltaEXP = 0;

        UnitNode* cur = ar->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());

            Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
            if (inf != nullptr 
            && inf->getInfantryType() == InfantryType::REGULARINFANTRY
            && distance <= 3.0) 
            {
                int attackScore = inf->getAttackScore();
                int gain = static_cast<int>(ceil((3.0 * attackScore) / (2.0 * distance)));
                deltaEXP += gain;
            }
            // CQ: các phương tiện không bị ảnh hưởng
            cur = cur->next;
        }

        ar->setEXP(ar->getEXP() + deltaEXP);
    }
}

// Fortification class implementation
Fortification::Fortification(const Position &pos)
    : TerrainElement(pos) {}

void Fortification::getEffect(Army* army) {
    Position center = this->pos;

    LiberationArmy* li = dynamic_cast<LiberationArmy*>(army);
    if (li != nullptr) {
        int deltaLF = 0;
        int deltaEXP = 0;

        UnitNode* cur = li->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());

            if (distance <= 2.0) {
                int attackScore = cur->unit->getAttackScore();
                int loss = static_cast<int>(ceil(0.20 * attackScore));
                Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);
                if (veh != nullptr) {
                    deltaLF -= loss;
                } else {
                    Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
                    if (inf != nullptr) {
                        deltaEXP -= loss;
                    }
                }
            }

            cur = cur->next;
        }

        li->setEXP(li->getEXP() + deltaEXP);
        li->setLF(li->getLF() + deltaLF);
        return;
    }

    ARVN* ar = dynamic_cast<ARVN*>(army);
    if (ar != nullptr) {
        int deltaLF = 0;
        int deltaEXP = 0;

        UnitNode* cur = ar->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());

            if (distance <= 2.0) {
                int attackScore = cur->unit->getAttackScore();
                int gain = static_cast<int>(ceil(0.20 * attackScore));
                Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);
                if (veh != nullptr) {
                    deltaLF += gain;
                } else {
                    Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
                    if (inf != nullptr) {
                        deltaEXP += gain;
                    }
                }
            }

            cur = cur->next;
        }

        ar->setLF(ar->getLF() + deltaLF);
        ar->setEXP(ar->getEXP() + deltaEXP);
    }
}

// Special zone implementation
SpecialZone::SpecialZone(const Position &pos)
    : TerrainElement(pos) {}

void SpecialZone::getEffect(Army* army) {
    Position center = this->pos;

    LiberationArmy* li = static_cast<LiberationArmy*>(army);
    if (li != nullptr) {
        int deltaLF = 0;
        int deltaEXP = 0;

        UnitNode* cur = li->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());
            if (distance <= 1.0) {
                Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);
                if (veh != nullptr) {
                    int attackScore = veh->getAttackScore();
                    deltaLF -= attackScore;
                }
                else {
                    Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
                    if (inf != nullptr) {
                        int attackScore = inf->getAttackScore();
                        deltaEXP -= attackScore;
                    }
                }
            }

            cur = cur->next;
        }

        li->setEXP(li->getEXP() + deltaEXP);
        li->setLF(li->getLF() + deltaLF);
        return;
    }

    ARVN* ar = dynamic_cast<ARVN*>(army);
    if (ar != nullptr) {
        int deltaLF = 0;
        int deltaEXP = 0;

        UnitNode* cur = ar->getUnitList()->getHead();
        while (cur != nullptr) {
            double distance = center.distance(cur->unit->getCurrentPosition());

            if (distance <= 1.0) {
                Vehicle* veh = dynamic_cast<Vehicle*>(cur->unit);

                if (veh != nullptr) {
                    int attackScore = veh->getAttackScore();
                    deltaLF -= attackScore;
                } 
                else {
                    Infantry* inf = dynamic_cast<Infantry*>(cur->unit);
                    if (inf != nullptr) {
                        int attackScore = inf->getAttackScore();
                        deltaEXP -= attackScore;
                    }
                }
            }

            cur = cur->next;
        }

        ar->setLF(ar->getLF() + deltaLF);
        ar->setEXP(ar->getEXP() + deltaEXP);
    }
}

// TerrainElement class implementation
TerrainElement::TerrainElement(const Position &pos) {}
TerrainElement::~TerrainElement() {}

// BattleField class implementation
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                        vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
    : n_rows(n_rows), n_cols(n_cols) {
    // Cấp phát matrix
    terrain = new TerrainElement**[n_rows];
    for (int i = 0; i < n_rows; i++) {
        terrain[i] = new TerrainElement*[n_cols];
    }

    // Khởi tất cả về Road
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            Position pos(i, j);
            terrain[i][j] = new Road(pos);
        }
    }

    // Gán từng vị trí mountain
    for (int i = 0; i < arrayForest.size(); i++) {
        int r = arrayForest[i]->getRow();
        int c = arrayForest[i]->getCol();

        if (r >= 0 && r < n_rows
        && c >= 0 && c < n_cols) 
        {
            delete terrain[r][c]; // Xóa Road mặc định
            terrain[r][c] = new Mountain(*arrayForest[i]);
        }
    }

    // Gán river
    for (int i = 0; i < arrayRiver.size(); i++) {
        int r = arrayRiver[i]->getRow();
        int c = arrayRiver[i]->getCol();

        if (r >= 0 && r < n_rows
        && c >= 0 && c < n_cols) 
        {
            delete terrain[r][c];
            terrain[r][c] = new River(*arrayRiver[i]);
        }
    }

    // Gán FORTIFICATION
    for (int i = 0; i < arrayFortification.size(); i++) {
        int r = arrayFortification[i]->getRow();
        int c = arrayFortification[i]->getCol();

        if (r >= 0 && r < n_rows
        && c >= 0 && c < n_cols)
        {
            delete terrain[r][c];
            terrain[r][c] = new Fortification(*arrayFortification[i]);
        }
    }

    // Gán Urban
    for (int i = 0; i < arrayUrban.size(); i++) {
        int r = arrayUrban[i]->getRow();
        int c = arrayUrban[i]->getCol();

        if (r >= 0 && r < n_rows
        && c >= 0 && c < n_cols) 
        {
            delete terrain[r][c];
            terrain[r][c] = new Urban(*arrayUrban[i]);
        }
    }

    // Gán SPECIALZONE
    for (int i = 0; i < arraySpecialZone.size(); i++) {
        int r = arrayUrban[i]->getRow();
        int c = arrayUrban[i]->getCol();

        if (r >= 0 && r < n_rows
        && c >= 0 && c < n_cols)
        {
            delete terrain[r][c];
            terrain[r][c] = new SpecialZone(*arraySpecialZone[i]);
        }
    }
}

BattleField::~BattleField() {
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            delete terrain[i][j];
        }
        delete[] terrain[i];
    }
    delete[] terrain;
}

string BattleField::str() const {
    // Ví dụ: "BattleField[n_rows=10,n_cols=8]"
    ostringstream oss;
    oss << "BattleField[n_rows=" << n_rows
        << ",n_cols=" << n_cols
        << "]";
    return oss.str();
}

void BattleField::getTerrainEffects(Army* army) {
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            terrain[i][j]->getEffect(army);
        }
    }
}

// Configuration class
void Configuration::parsePositions(string arrayStr, vector<Position*>& positions) 
{
    // Remove [ and ]
    arrayStr = arrayStr.substr(1, arrayStr.length() - 2);

    // Parse each Position
    int start = 0;
    int pos = 0;
    while ((pos = arrayStr.find("),", start)) != string::npos) {
        string posStr = arrayStr.substr(start, pos - start + 1);
        positions.push_back(new Position(posStr));
        start = pos + 2;
    }

    if (start < arrayStr.length()) {
        positions.push_back(new Position(arrayStr.substr(start)));
    }
}

size_t findMatchingParen(const string& str, size_t start) {
    int count = 1;

    for (size_t i = start + 1; i < str.length(); i++) {
        if (str[i] == '(') 
            count++;
        else if (str[i] == ')') 
            count--;
        if (count == 0) 
            return i;
    }

    return string::npos;
}

void Configuration::parseUnitList(const string& value) {
    auto a = value.find('[');
    auto b = value.rfind(']');

    if (a == string::npos 
    || b == string::npos 
    || b <= a) 
        return;

    string body = value.substr(a + 1, b - a - 1);
    size_t pos = 0;
    while (pos < body.size()) {
        // Skip space
        while (pos < body.size() && isspace(body[pos])) ++pos;

        auto p1 = body.find('(', pos);

        if (p1 == string::npos) 
            break;

        // Lấy unitName và trim
        string raw = body.substr(pos, p1 - pos), unitName;
        for (char c : raw) 
            if (!isspace(c)) 
                unitName.push_back(c);

        // Tìm ngoặc đóng của param
        auto p2 = findMatchingParen(body, p1);
        if (p2 == string::npos) 
            break;

        string params = body.substr(p1 + 1, p2 - p1 - 1);

        // Split params chỉ ở depth=0
        vector<string> token;
        string currentToken = "";
        int depth = 0;
        for (char c : params) {
            if (c == ',' && depth == 0) {
                token.push_back(currentToken); 
                currentToken.clear();
            } else {
                if (c == '(') 
                    ++depth;
                else if (c == ')') 
                    --depth;
                currentToken.push_back(c);
            }
        }

        if (!currentToken.empty()) 
            token.push_back(currentToken);

        if (token.size() == 4) {
            int qty       = stoi(token[0]);
            int wgt       = stoi(token[1]);
            Position p(token[2]);
            int side      = stoi(token[3]);
            Unit* u = nullptr;

            // Khởi tạo đúng type
            if (unitName == "TANK")             
                u = new Vehicle(qty, wgt, p, TANK);
            else if (unitName == "ARTILLERY")   
                u = new Vehicle(qty, wgt, p, ARTILLERY);
            else if (unitName == "ARMOREDCAR")  
                u = new Vehicle(qty, wgt, p, ARMOREDCAR);
            else if (unitName == "APC")         
                u = new Vehicle(qty, wgt, p, APC);
            else if (unitName == "TRUCK")       
                u = new Vehicle(qty, wgt, p, TRUCK);
            else if (unitName == "MORTAR")      
                u = new Vehicle(qty, wgt, p, MORTAR);
            else if (unitName == "ANTIAIRCRAFT")
                u = new Vehicle(qty, wgt, p, ANTIAIRCRAFT);
            else if (unitName == "SNIPER")          
                u = new Infantry(qty, wgt, p, SNIPER);
            else if (unitName == "ANTIAIRCRAFTSQUAD")
                u = new Infantry(qty, wgt, p, ANTIAIRCRAFTSQUAD);
            else if (unitName == "MORTARSQUAD")
                u = new Infantry(qty, wgt, p, MORTARSQUAD);
            else if (unitName == "ENGINEER")        
                u = new Infantry(qty, wgt, p, ENGINEER);
            else if (unitName == "SPECIALFORCES")   
                u = new Infantry(qty, wgt, p, SPECIALFORCES);
            else if (unitName == "REGULARINFANTRY") 
                u = new Infantry(qty, wgt, p, REGULARINFANTRY);

            if (u != nullptr) {
                if (side == 0) 
                    liberationUnits.push_back(u);
                else
                    ARVNUnits.push_back(u);
            }
        }
        pos = p2 + 1;
        if (pos < body.size() && body[pos] == ',') 
            ++pos;
    }
}

void Configuration::readConfigFile(const string& filepath) {
    ifstream fin(filepath);

    if (! fin.is_open()) {
        return;
    }

    string currentLine;
    while (getline(fin, currentLine)) {
        if (currentLine.empty()) continue;

        size_t equalPos = currentLine.find('=');
        if (equalPos == string::npos) continue;

        string name = currentLine.substr(0, equalPos);
        string value = currentLine.substr(equalPos + 1);

        if (name == "NUM_ROWS") {
            n_rows = stoi(value);
        }
        else if (name == "NUM_COLS") {
            n_cols = stoi(value);
        }
        else if (name == "ARRAY_FOREST") {
            parsePositions(value, arrayForest);
        }
        else if (name == "ARRAY_RIVER") {
            parsePositions(value, arrayRiver);
        }
        else if (name == "ARRAY_FORTIFICATION") {
            parsePositions(value, arrayFortification);
        }
        else if (name == "ARRAY_URBAN") {
            parsePositions(value, arrayUrban);
        } 
        else if (name == "ARRAY_SPECIAL_ZONE") {
            parsePositions(value, arraySpecialZone);
        }
        else if (name == "UNIT_LIST") {
            parseUnitList(value);
        }
        else if (name == "EVENT_CODE") {
            eventCode = stoi(value);

            if (eventCode > 99) {
                eventCode = eventCode % 100;
            }
            else if (eventCode < 0) {
                eventCode = 0;
            }
        }
    }

    fin.close();
}

Configuration::Configuration(const string& filepath)
    : n_rows(0), n_cols(0), eventCode(0) 
{
    readConfigFile(filepath);
}

Configuration::~Configuration() {
    for (int i = 0; i < arrayForest.size(); i++) {
        delete arrayForest[i];
    }

    for (int i = 0; i < arrayRiver.size(); i++) {
        delete arrayRiver[i];
    }

    for (int i = 0; i < arrayFortification.size(); i++) {
        delete arrayFortification[i];
    }

    for (int i = 0; i < arrayUrban.size(); i++) {
        delete arrayUrban[i];
    }

    for (int i = 0; i < arraySpecialZone.size(); i++) {
        delete arraySpecialZone[i];
    }

    for (int i = 0; i < liberationUnits.size(); i++) {
        delete liberationUnits[i];
    }

    for (int i = 0; i < ARVNUnits.size(); i++) {
        delete ARVNUnits[i];
    }
}

string Configuration::str() const {
    string result = "[";
    result += "num_rows=" + to_string(n_rows) + ",";
    result += "num_cols=" + to_string(n_cols) + ",";
    
    // Add information about the arrays
    result += "arrayForest=[";
    for (int i = 0; i < arrayForest.size(); i++) {
        if (i > 0) 
            result += ",";
        result += arrayForest[i]->str();
    }
    result += "],";
       
    result += "arrayRiver=[";
    for (int i = 0; i < arrayRiver.size(); i++) {
        if (i > 0) 
            result += ",";
        result += arrayRiver[i]->str();
    }
    result += "],";
       
    result += "arrayFortification=[";
    for (int i = 0; i < arrayFortification.size(); i++) {
        if (i > 0) 
            result += ",";
        result += arrayFortification[i]->str();
    }
    result += "],";
       
    result += "arrayUrban=[";
    for (int i = 0; i < arrayUrban.size(); i++) {
        if (i > 0) 
            result += ",";
        result += arrayUrban[i]->str();
    }
    result += "],";
       
    result += "arraySpecialZone=[";
    for (int i = 0; i < arraySpecialZone.size(); i++) {
        if (i > 0) 
            result += ",";
        result += arraySpecialZone[i]->str();
    }
    result += "],";
       
    // Add information about the units
    result += "liberationUnits=[";
    for (int i = 0; i < liberationUnits.size(); i++) {
        if (i > 0) 
            result += ",";
        result += liberationUnits[i]->str();
    }
    result += "],";
       
    result += "ARVNUnits=[";
    for (int i = 0; i < ARVNUnits.size(); i++) {
        if (i > 0) 
            result += ",";
        result += ARVNUnits[i]->str();
    }
    result += "],";
       
    result += "eventCode=" + to_string(eventCode);
    result += "]";
    return result;
}

int Configuration::getRows() const {
    return n_rows;
}

int Configuration::getCols() const {
    return n_cols;
}
    
vector<Position*> Configuration::getArrayForest() const {
    return arrayForest;
}
    
vector<Position*> Configuration::getArrayRiver() const {
    return arrayRiver;
}
    
vector<Position*> Configuration::getArrayFortification() const {
    return arrayFortification;
}
    
vector<Position*> Configuration::getArrayUrban() const {
    return arrayUrban;
}
    
vector<Position*> Configuration::getArraySpecialZone() const {
    return arraySpecialZone;
}
    
vector<Unit*> Configuration::getUnitList() const {
    return unitList;
}

const vector<Unit*>& Configuration::getLiberationUnits() const {
    return liberationUnits;
}

const vector<Unit*>& Configuration::getARVNUnits() const {
    return ARVNUnits;
}

int Configuration::getEventCode() const {
    return eventCode;
}

// HCMCampaign class implementation
HCMCampaign::HCMCampaign(const string& config_file_path) {
    config = nullptr;
    battleField = nullptr;
    liberationArmy = nullptr;
    arvn = nullptr;

    config = new Configuration(config_file_path);

    battleField = new BattleField (
        config->getRows(),
        config->getCols(),
        config->getArrayForest(),
        config->getArrayRiver(),
        config->getArrayFortification(),
        config->getArrayUrban(),
        config->getArraySpecialZone()
    );

    // Split units between armies based on side (parameter 3)
    vector<Unit*> liberUnits = config->getLiberationUnits();
    vector<Unit*> arvnUnits = config->getARVNUnits();

    // Create armies
    Unit** liberArray = new Unit*[liberUnits.size()];
    for (int i = 0; i < liberUnits.size(); i++) {
        liberArray[i] = liberUnits[i];
    }
    liberationArmy = new LiberationArmy(liberArray, 
                                    liberUnits.size(), 
                                    "LiberationArmy", 
                                    battleField);

    Unit** arvnArray = new Unit*[arvnUnits.size()];
    for (int i = 0; i < arvnUnits.size(); i++) {
        arvnArray[i] = arvnUnits[i];
    }
    arvn = new ARVN(arvnArray, 
        arvnUnits.size(), 
        "ARVN", 
        battleField);

    delete[] liberArray;
    delete[] arvnArray;
}

HCMCampaign::~HCMCampaign() {
    delete config;
    delete battleField;
    delete liberationArmy;
    delete arvn;
}

void removeWeaklyUnits(Army* army) {
    if (army == nullptr) return;

    UnitList* unitList = army->getUnitList();
    if (unitList == nullptr) return;

    vector<Unit*> unitsToRemove;

    UnitNode* cur = unitList->getHead();
    while (cur != nullptr) {
        if (cur->unit->getAttackScore() <= 5) {
            unitsToRemove.push_back(cur->unit);
        }
        cur = cur->next;
    }

    for (int i = 0; i < unitsToRemove.size(); i++) {
        unitList->removeUnit(unitsToRemove[i]);
    }

    int newLF = 0;
    int newEXP = 0;

    cur = unitList->getHead();
    while (cur != nullptr) {
        if (dynamic_cast<Vehicle*>(cur->unit) != nullptr) {
            newLF += cur->unit->getAttackScore();
        }
        else if (dynamic_cast<Infantry*>(cur->unit) != nullptr) {
            newEXP += cur->unit->getAttackScore();
        }
        cur = cur->next;
    }

    army->setLF(newLF);
    army->setEXP(newEXP);
}

void HCMCampaign::run() {
    battleField->getTerrainEffects(liberationArmy);
    battleField->getTerrainEffects(arvn);

    int eventCode = config->getEventCode();

    if (eventCode < 75) {
        liberationArmy->fight(arvn, false);
    } else {
        arvn->fight(liberationArmy, false);
        liberationArmy->fight(arvn, false);
    }

    // Remove units with attackScore <= 5
    removeWeaklyUnits(liberationArmy);
    removeWeaklyUnits(arvn);
}

string HCMCampaign::printResult() {
    return "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF()) + 
          ",EXP=" + to_string(liberationArmy->getEXP()) + 
          "]-ARVN[LF=" + to_string(arvn->getLF()) + 
          ",EXP=" + to_string(arvn->getEXP()) + "]";
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////