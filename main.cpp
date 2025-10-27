#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>

enum PowerUpType {
    STAR,
    MUSHROOM,
    FIREFLOWER,
    ICEFLOWER,
    NONE
};

enum EnemyType {
    GOOMBA,
    KOOPA,
    PLANT
};

enum BlockType {
    GRASS,
    ROCK,
    LUCKYBL,
    USEDBL,
    PIPE,
    PIPETOP
};

class Player {
private:
    int state;
    float x, y;
    float jump;
    float gravity;
    float speed;
public:
    Player() : state(0), x(0.0f), y(0.0f), jump(0.0f), gravity(0.0f), speed(0.0f) {}

    Player(float x, float y, int state, float jump, float gravity, float speed) {
        this->x=x;
        this->y=y;
        this->state=state;
        this->jump=jump;
        this->gravity=gravity;
        this->speed=speed;
    }
    Player(const Player& player) {
        this->x=player.x;
        this->y=player.y;
        this->state=player.state;
        this->jump=player.jump;
        this->gravity=player.gravity;
        this->speed=player.speed;
    }

    ~Player()=default;
    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        os<<player.x<<" "<<player.y;
        os<<player.state;
        os<<player.jump;
        os<<player.gravity;
        os<<player.speed;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Player& player) {
        std::cout<<"Coord\n";
        is>>player.x>>player.y;
        std::cout<<"Player state:\n";
        is>>player.state;
        std::cout<<"Jump:\n";
        is>>player.jump;
        std::cout<<"Gravity force:\n";
        is>>player.gravity;
        std::cout<<"Player speed:\n";
        is>>player.speed;
        return is;
    }
    Player& operator=(const Player& other) {
        if (this!=&other) {
            this->x=other.x;
            this->y=other.y;
            this->state=other.state;
            this->jump=other.jump;
            this->gravity=other.gravity;
            this->speed=other.speed;
        }
        return *this;
    }
    [[nodiscard]] int getState() const {return state;}
    [[nodiscard]] float getJump() const {return jump;}
    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }

    void takedmg() {
        if (this->state>0) {
            std::cout<<"Ai luat damage\n";
            this->state--;
        }
        else
            std::cout<<"YOU DIED\n";
    }

    void setState(int s) {
        state=s;
    }
    void setX(float x) {
        this->x=x;
    }
    void setY(float y) {
        this->y=y;
    }
};

class Powerup {
private:
    PowerUpType type;
    float x,y;
    bool move_dir;
    float speed;
    float spawn_timer;
    float gravity;
public:

    Powerup() : type(NONE), x(0.0f), y(0.0f), move_dir(false), speed(0.0f), spawn_timer(0.0f), gravity(0.0f) {}

    Powerup(float x, float y, PowerUpType type, float gravity, float speed, float spawn_timer, bool move_dir) {
        this->x=x;
        this->y=y;
        this->type=type;
        this->gravity=gravity;
        this->speed=speed;
        this->move_dir=move_dir;
        this->spawn_timer=spawn_timer;
    }
    Powerup(const Powerup& powerup) {
        this->x=powerup.x;
        this->y=powerup.y;
        this->type=powerup.type;
        this->move_dir=powerup.move_dir;
        this->speed=powerup.speed;
        this->spawn_timer=powerup.spawn_timer;
        this->gravity=powerup.gravity;
    }
    ~Powerup()=default;
    friend std::ostream& operator<<(std::ostream& os, const Powerup& powerup) {
        switch (powerup.type) {
            case STAR:
                os<<"STAR\n";
                break;
            case MUSHROOM:
                os<<"MUSHROOM\n";
                break;
            case FIREFLOWER:
                os<<"FIREFLOWER\n";
                break;
            case ICEFLOWER:
                os<<"ICEFLOWER\n";
                break;
            case NONE:
                os<<"NONE\n";
                break;
            default:
                os<<"UNKNOWN\n";
                break;
        }
        os<<powerup.x;
        os<<powerup.y;
        os<<powerup.move_dir;
        os<<powerup.speed;
        os<<powerup.spawn_timer;
        os<<powerup.gravity;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Powerup& powerup) {
        std::cout<<"Coord\n";
        is>>powerup.x;
        is>>powerup.y;
        std::cout<<"Move direction (-1 or 1)\n";
        is>>powerup.move_dir;
        std::cout<<"Speed\n";
        is>>powerup.speed;
        std::cout<<"Spawn timer\n";
        is>>powerup.spawn_timer;
        std::cout<<"Gravity\n";
        is>>powerup.gravity;
        return is;
    }
    Powerup& operator=(const Powerup& other) {
        if (this!=&other) {
            this->x=other.x;
            this->y=other.y;
            this->type=other.type;
            this->move_dir=other.move_dir;
            this->speed=other.speed;
            this->spawn_timer=other.spawn_timer;
            this->gravity=other.gravity;
        }
        return *this;
    }

    [[nodiscard]] float getGravity() const {return gravity;}
    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }
    void setGravity(float g) {
        gravity=g;
    }
    void setSpeed(float s) {
        speed=s;
    }
};

class Enemy {
private:
    EnemyType type;
    float x,y;
    int direction;
    float speed;
    float gravity;
    bool isAlive;
public:
    // FIX [UninitMemberVar]: Initialize all members
    Enemy() : type(GOOMBA), x(0.0f), y(0.0f), direction(0), speed(0.0f), gravity(0.0f), isAlive(false) {}

    Enemy(float x, float y, EnemyType type, bool isAlive, int direction, float speed, float gravity) {
        this->x=x;
        this->y=y;
        this->type=type;
        this->isAlive=isAlive;
        this->direction=direction;
        this->speed=speed;
        this->gravity=gravity;
    }
    Enemy(const Enemy& enemy) {
        this->x=enemy.x;
        this->y=enemy.y;
        this->type=enemy.type;
        this->isAlive=enemy.isAlive;
        this->direction=enemy.direction;
        this->speed=enemy.speed;
        this->gravity=enemy.gravity;
    }
    ~Enemy()=default;
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
        switch (enemy.type) {
            case GOOMBA:
                os<<"GOOMBA\n";
                break;
            case KOOPA:
                os<<"KOOPA\n";
                break;
            case PLANT:
                os<<"PLANT\n";
                break;
            default:
                os<<"UNKNOWN\n";
        }
        os<<enemy.x;
        os<<enemy.y;
        os<<enemy.isAlive;
        os<<enemy.direction;
        os<<enemy.gravity;
        os<<enemy.speed;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Enemy& enemy) {
        std::cout<<"Coord\n";
        is>>enemy.x>>enemy.y;
        std::cout<<"IS ALIVE?\n";
        is>>enemy.isAlive;
        std::cout<<"Direction\n";
        is>>enemy.direction;
        std::cout<<"Speed\n";
        is>>enemy.speed;
        std::cout<<"Gravity\n";
        is>>enemy.gravity;
        return is;
    }

    Enemy& operator=(const Enemy& other) {
        if (this!=&other) {
            this->x = other.x;
            this->y = other.y;
            this->type=other.type;
            this->isAlive = other.isAlive;
            this->direction=other.direction;
            this->speed=other.speed;
            this->gravity=other.gravity;
        }
        return *this;
    }
    [[nodiscard]] float getGravity() const {return gravity;}
    [[nodiscard]] float getSpeed() const {return speed;}
    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }
    [[nodiscard]] bool getIsAlive() const {
        return isAlive;
    }
    void stomp() {
        this->isAlive=false;
        this->speed=0;
    }

};

class Platform {
private:
    float x, y;
    BlockType type;
    bool isDestructible;
    bool isEmpty;
    bool isSolid;
public:
    Platform() : x(0.0f), y(0.0f), type(GRASS), isDestructible(false), isEmpty(false), isSolid(false) {}

    Platform(float x, float y, BlockType type, bool isDestructible, bool isEmpty, bool isSolid) {
        this->x=x;
        this->y=y;
        this->type=type;
        this->isDestructible=isDestructible;
        this->isEmpty=isEmpty;
        this->isSolid=isSolid;
    }
    Platform(const Platform& platform) {
        this->x=platform.x;
        this->y=platform.y;
        this->type=platform.type;
        this->isDestructible=platform.isDestructible;
        this->isEmpty=platform.isEmpty;
        this->isSolid=platform.isSolid;
    }
    ~Platform()=default;
    friend std::ostream& operator<<(std::ostream& os, const Platform& platform) {
        switch (platform.type) {
            case GRASS:
                os<<"GRASS\n";
                break;
            case ROCK:
                os<<"ROCK\n";
                break;
            case LUCKYBL:
                os<<"Lucky Block\n";
                break;
            case USEDBL:
                os<<"Used Block\n";
                break;
            case PIPE:
                os<<"PIPE\n";
                break;
            case PIPETOP:
                os<<"Pipe Top\n";
                break;
            default:
                os<<"UNKNOWN\n";
        }
        os<<platform.x;
        os<<platform.y;
        os<<platform.isDestructible;
        os<<platform.isEmpty;
        os<<platform.isSolid;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Platform& platform) {
        std::cout<<"Coord\n";
        is>>platform.x;
        is>>platform.y;
        std::cout<<"IsDestructible\n";
        is>>platform.isDestructible;
        std::cout<<"IsEmpty\n";
        is>>platform.isEmpty;
        std::cout<<"IsSolid\n";
        is>>platform.isSolid;
        return is;
    }

    Platform& operator=(const Platform& other) {
        if (this!=&other) {
            this->x=other.x;
            this->y=other.y;
            this->type=other.type;
            this->isDestructible=other.isDestructible;
            this->isEmpty=other.isEmpty;
            this->isSolid=other.isSolid;
        }
        return *this;
    }
    PowerUpType HitByPlayer(const Player& player) {
        if (this->type==LUCKYBL && !this->isEmpty) {
            this->type=USEDBL;
            this->isEmpty=true;
            if (player.getState()==0)
                return MUSHROOM;
            else
                return FIREFLOWER;
        }
        if (this->isDestructible && player.getState()>0) {
            this->isSolid=false;
            this->isEmpty=false;
            return NONE;
        }
        std::cout<<"SOLID BLOCK\n";
        return NONE;
    }
};

class Level {
private:
    std::string name;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Platform> platforms;
    std::vector<Powerup> powerups;
    void checkPlayerEnemyCollision(Player& p, Enemy& e) {
        if (!e.getIsAlive())
            return;

        if (std::abs(p.getX() - e.getX()) < 1.0f && std::abs(p.getY() - e.getY()) < 1.0f) {
            if (p.getJump() > 0)
                e.stomp();
            else
                p.takedmg();
        }
    }
public:
    Level()=default;
    Level(const std::string& name, const Player& player) {
        this->name=name;
        this->player=player;
    }
    Level(const Level& level) {
        this->name=level.name;
        this->player=level.player;
    }
    ~Level()=default;

    Level& operator=(const Level& other) {
        if (this!=&other) {
            this->name=other.name;
            this->player=other.player;
            this->enemies=other.enemies;
            this->platforms=other.platforms;
            this->powerups=other.powerups;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Level& level) {
        os<<"Name\n";
        os<<level.name;
        os<<"Player\n";
        os<<level.player;
        os<<"Enemies\n";
        for (const auto& e:level.enemies)
            os<<e<<"\n";
        os<<"Powerups";
        for (const auto& p:level.powerups)
            os<<p<<"\n";
        os<<"Platforms";
        for (const auto& pw:level.platforms)
            os<<pw<<"\n";
        return os;
    }
    friend std::istream& operator>>(std::istream&is, Level& level) {
        std::cout<<"Name\n";
        is>>level.name;
        std::cout<<"Player\n";
        is>>level.player;
        int count;
        std::cout<<"Enemies\n";
        is>>count;
        level.enemies.clear();
        for (int i=0;i<count;i++) {
            Enemy e;
            is>>e;
            level.enemies.push_back(e);
        }
        std::cout<<"Powerups\n";
        is>>count;
        for (int i=0;i<count;i++) {
            Powerup pw;
            is>>pw;
            level.powerups.push_back(pw);
        }
        std::cout<<"Platforms\n";
        is>>count;
        for (int i=0;i<count;i++) {
            Platform p;
            is>>p;
            level.platforms.push_back(p);
        }
        return is;
    }

    void addEnemies(const Enemy& enemy) {
        enemies.push_back(enemy);
    }
    // FIX [unusedFunction]: Removed addPowerup()
    void addPlatform(const Platform& platform) {
        platforms.push_back(platform);
    }

    Player& getPlayer() {
        return player;
    }
    Platform& getPlatform(int index) {
        if (index>=0 && index < platforms.size())
            return platforms[index];
        throw std::out_of_range("Index invalid\n");
    }
    Enemy& getEnemy(int index) {
        if (index>=0 && index < enemies.size())
            return enemies[index];
        throw std::out_of_range("Index invalid\n");
    }

    void collision() {
        Player& playerRef = getPlayer();
        for (auto& enemy : enemies) {
            checkPlayerEnemyCollision(playerRef, enemy);
        }
    }
};

int main() {
    std::cout<<"----- Creare jucator si nivel ----\n";
    Player mario(3, 3, 0, 0.0f, 0.5f, 1.0f);
    std::cout<<"Jucator creat:\n"<<mario<<"\n";

    Level world1("World 1", mario);

    Platform ground(0, 7, GRASS, false, false, true);
    Platform luckyBlock(5, 3, LUCKYBL, false, false, true);
    Enemy goomba(5, 10, GOOMBA, true, -1, 0.2f, 0.5f);

    world1.addPlatform(ground);
    world1.addPlatform(luckyBlock);
    world1.addEnemies(goomba);

    std::cout<<"---- Starea initiala a nivelului ----\n";
    std::cout<<world1<<"\n";

    Player& playerRef=world1.getPlayer();
    Platform& platformRef=world1.getPlatform(1);

    PowerUpType spawnedItem=platformRef.HitByPlayer(playerRef);
    if (spawnedItem==MUSHROOM) {
        std::cout<<"S-a spawnat un mushroom!\n";
        playerRef.setState(1);
    }
    else if (spawnedItem==FIREFLOWER) {
        std::cout<<"S-a spawnat fireflower!\n";
        playerRef.setState(2);
    }
    else
        std::cout<<"Teapa... Nimic!\n";

    std::cout<<"Starea lui Mario inainte de coliziune\n"<<world1.getPlayer().getState()<<"\n";
    world1.getPlayer().setX(5.1);
    world1.getPlayer().setY(10);
    world1.collision();
    std::cout<<"Starea lui Mario dupa coliziune\n"<<world1.getPlayer().getState()<<"\n";

    const Enemy& enemyRef=world1.getEnemy(0);
    if (enemyRef.getIsAlive())
        std::cout<<"Goomba in viata\n";
    else
        std::cout<<"Goomba invins\n";

    return 0;
}
