#ifndef ERRORCHAMBER_H
#define ERRORCHAMBER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Mission/Units/AnimatedObject.h"
#include "../Dialog/DialogBox.h"

class ErrorChamber
{
public:
    bool initialized = false;
    int badState = -1;

    std::vector<PataDialogBox> dialogboxes;

    void Initialize();
    void Update();
    ErrorChamber();
    ~ErrorChamber();
};



#endif //ERRORCHAMBER_H
