#ifndef YARIPON_H
#define YARIPON_H

#include "../AnimatedObject.h"
#include "../../../Graphics/PataText.h"

class Yaripon {
public:
    AnimatedObject main;
    float local_x=0, local_y=0;
    float global_x=0, global_y=0;

    float maxHP = 100;
    float curHP = maxHP;

    float minDmg=0, maxDmg=0;
    float dmgMultiplier = 1;
    float defMultiplier = 1;

    int order = 0;
    int maxp = 0;

    float attackSpeed = 1000;
    float closestEnemyX = 0;
    // timer when the attack can be done. (spear throwing delay when in air)
    // walk -> jump -> throw throw throw -> fall
    float canAttackIn = 0;
    float distanceToTravel = 0;

    float attack_x = 0, gap_x = 0;
    int attackType = 0;

    bool enemyInSight = false;

    int action = 0;

    float pataSpeed = 0;
    float pataMaxSpeed = 400;
    float pataCurMaxSpeed = pataMaxSpeed;
    float accelerationFactor = 2;
    float decelerationFactor = 2;

    int globalRand = 0;

    sf::Clock attackWalkTimer;
    bool inAttackSequence = false;
    sf::Clock inAttackTimer;

    float vspeed = 0;
    float hspeed = 0;
    float gravity = 2500;

    bool threw = false;
    bool walkBack = false;
    bool performedAttack = true;

    bool toggleDebug = false;
    bool missionEnd = false;
    bool failure = false;

    std::string wpn, hlm;

    sf::Clock dead_timer;
    bool death_start = false;
    bool for_removal = false;
    bool dead = false;

    float cam_offset = 0;

    PataText debugText;

    Yaripon(int which, int maxpons);
    void Advance();
    void Attack(int power);
    void PerformAttack();
    void StopAttack();
    void PerformMissionEnd();
    void PerformDeath();
    void StopAll();
    void Drum(std::string drum);
    std::vector<sf::FloatRect> getHitbox();
    void Draw();
};

#endif //YARIPON_H
