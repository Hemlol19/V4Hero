#ifndef CREDITS_H
#define CREDITS_H

#include "../Func.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PataText.h"
#include "../Input/InputController.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Credits
{
public:
    Config* config;
    int quality = 0;

    sf::SoundBuffer sb_outro, sb_credits;
    sf::Sound s_outro, s_credits;

    sf::RectangleShape r_black;

    bool is_active = false;

    sf::Clock startTimer;

    bool playOutro = false;

    PataText outro_text;
    PSprite teaser_1, teaser_2, teaser_3;

    int mode = 0; ///0 = outro, 1 = credits

    std::vector<PataText> credits_text;
    PataText anykey;
    float flash_x = 0;

    Credits();
    void addHeaderText(sf::String text);
    void addRegularText(sf::String text, int font = 0);
    void Initialise(Config* thisConfig, V4Core* parent);
    void restart();
    void draw(sf::RenderWindow& window, float fps, InputController& inputCtrl);
};

#endif // CREDITS_H
