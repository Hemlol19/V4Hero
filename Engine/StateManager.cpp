#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "StateManager.h"
#include "CoreManager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    SPDLOG_DEBUG("StateManager Destructor start.");

    for (auto& t : loadingThreads)
    {
        t.detach();
    }
    SPDLOG_DEBUG("StateManager Destructor completed.");
}

StateManager& StateManager::getInstance()
{
    static StateManager instance;
    return instance;
}

void StateManager::updateCurrentState()
{
    try
    {
        switch (currentGameState)
        {
            case ENTRY: {

                if (loadingTipPtr == nullptr)
                {
                    loadingTipPtr = new LoadingTip(1);
                } else
                {
                    delete loadingTipPtr;
                    loadingTipPtr = new LoadingTip(1);
                }

                setState(TIPS);
                afterTipState = MAINMENU;

                initStateMT(MAINMENU);

                /* if (mainMenuPtr != nullptr && optionsMenuPtr != nullptr)
                {
                    if (mainMenuPtr->initialized && optionsMenuPtr->initialized)
                    {
                        setState(MAINMENU);
                    }
                } */

                break;
            }

            case NEWGAMEMENU: {
                //newGameMenuPtr->Update();
                break;
            }

            case MAINMENU: {
                if (mainMenuPtr == nullptr)
                {
                    mainMenuPtr = new MainMenu;
                }

                mainMenuPtr->Update();
                break;
            }

            case OPTIONSMENU: {
                if (optionsMenuPtr == nullptr)
                {
                    optionsMenuPtr = new OptionsMenu;
                }

                optionsMenuPtr->Update();
                break;
            }

            case INTRODUCTION: {
                if (introductionPtr == nullptr)
                {
                    introductionPtr = new IntroductionMenu;
                }

                introductionPtr->Update();
                break;
            }

            case TIPS: {

                if (loadingTipPtr == nullptr)
                {
                    loadingTipPtr = new LoadingTip;
                }

                switch (afterTipState)
                {
                    case MAINMENU: {
                        if (mainMenuPtr != nullptr && optionsMenuPtr != nullptr && introductionPtr != nullptr)
                        {
                            if (mainMenuPtr->initialized && optionsMenuPtr->initialized && introductionPtr->initialized)
                            {
                                if (loadingTipPtr != nullptr)
                                {
                                    loadingTipPtr->pressAnyKey = true;

                                    if (loadingTipPtr->tipFinished)
                                    {
                                        setState(afterTipState);
                                    }
                                }
                            }
                        }

                        break;
                    }

                    case PATAPOLIS: {

                        if (patapolisPtr != nullptr && altarPtr != nullptr && barracksPtr != nullptr && obeliskPtr != nullptr)
                        {
                            if (patapolisPtr->initialized && altarPtr->initialized && barracksPtr->initialized && obeliskPtr->initialized)
                            {
                                if (loadingTipPtr != nullptr)
                                {
                                    loadingTipPtr->pressAnyKey = true;

                                    if (loadingTipPtr->tipFinished)
                                    {
                                        setState(afterTipState);
                                    }
                                }
                            }
                        }

                        break;
                    }

                    case MISSIONCONTROLLER: {

                        if (missionControllerPtr != nullptr)
                        {
                            if (missionControllerPtr->initialized)
                            {
                                if (loadingTipPtr != nullptr)
                                {
                                    loadingTipPtr->pressAnyKey = true;

                                    if (loadingTipPtr->tipFinished)
                                    {
                                        setState(afterTipState);
                                        CoreManager::getInstance().getRhythm()->Start();
                                    }
                                }
                            }
                        }

                        break;
                    }
                }

                loadingTipPtr->Draw();

                break;
            }

            case PATAPOLIS: {

                if (patapolisPtr == nullptr)
                {
                    patapolisPtr = new PatapolisMenu;
                }

                patapolisPtr->Update();

                break;
            }

            case PATAPOLIS_ALTAR: {

                if (patapolisPtr == nullptr)
                {
                    patapolisPtr = new PatapolisMenu;
                }

                if (altarPtr == nullptr)
                {
                    altarPtr = new AltarMenu;
                }

                patapolisPtr->Update();
                altarPtr->Update();

                break;
            }

            case BARRACKS: {

                if (barracksPtr == nullptr)
                {
                    barracksPtr = new Barracks;
                }

                barracksPtr->Update();

                break;
            }

            case OBELISK: {

                if (obeliskPtr == nullptr)
                {
                    obeliskPtr = new ObeliskMenu;
                    obeliskPtr->Reload();
                }

                obeliskPtr->Update();

                break;
            }

            case MISSIONCONTROLLER: {

                if (missionControllerPtr == nullptr)
                {
                    missionControllerPtr = new MissionController;
                }

                missionControllerPtr->Update();
                break;
            }

            case MATER_OUTER: {

                if (patapolisPtr == nullptr)
                {
                    patapolisPtr = new PatapolisMenu;
                }

                if (materPtr == nullptr)
                {
                    materPtr = new MaterOuterMenu;
                }

                patapolisPtr->Update();
                materPtr->Update();

                break;
            }

            case TEST_CHAMBER: {

                if (testChamberPtr == nullptr)
                {
                    testChamberPtr = new TestChamber;
                }

                testChamberPtr->Update();
                break;
            }

            case ERROR_CHAMBER: {
                if (errorChamberPtr == nullptr)
                {
                    SPDLOG_ERROR("Error handler is not initialized? Well... If we are here, something must have initialized it! This is stupid!");
                    SPDLOG_ERROR("Resetting the game...");

                    setState(ENTRY);
                }

                errorChamberPtr->Update();
            }
        }
    }
    catch( std::exception& exception )
    {
        SPDLOG_ERROR("Exception occurred: {}", exception.what());
        setState(ERROR_CHAMBER);
    }
    catch( ... )
    {
        SPDLOG_ERROR("Unknown exception occurred.");
        setState(ERROR_CHAMBER);
    }
}

void StateManager::initState(int state)
{
    switch (state)
    {
        case ENTRY: {

            break;
        }

        case NEWGAMEMENU: {

            break;
        }

        case MAINMENU: {

            // For initializing MainMenu, we want to initialize OptionsMenu as well, so the transition is seamless
            if (mainMenuPtr == nullptr)
            {
                try
                {
                    mainMenuPtr = new MainMenu;
                }
                catch( ... )
                {
                    SPDLOG_ERROR("Could not load Main Menu.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (optionsMenuPtr == nullptr)
            {
                try
                {
                    optionsMenuPtr = new OptionsMenu;
                }
                catch( ... )
                {
                    SPDLOG_ERROR("Could not load Options Menu.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (introductionPtr == nullptr)
            {
                try
                {
                    introductionPtr = new IntroductionMenu;
                    introductionPtr->Initialize();
                    introductionPtr->timeout.restart();
                }
                catch( ... )
                {
                    SPDLOG_ERROR("Could not load Introduction Menu.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            break;
        }

        case OPTIONSMENU: {
            if(optionsMenuPtr == nullptr)
                optionsMenuPtr = new OptionsMenu;

            break;
        }

        case INTRODUCTION: {
            if(introductionPtr == nullptr)
                introductionPtr = new IntroductionMenu;

            break;
        }

        case TIPS: {

            if (loadingTipPtr == nullptr)
            {
                try
                {
                    loadingTipPtr = new LoadingTip;
                }
                catch( ... )
                {
                    SPDLOG_ERROR("Could not load tips.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            break;
        }

        case PATAPOLIS: {
            if (patapolisPtr == nullptr)
            {
                try
                {
                    patapolisPtr = new PatapolisMenu;
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Patapolis.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (altarPtr == nullptr)
            {
                try
                {
                    altarPtr = new AltarMenu;
                    altarPtr->save_loaded = patapolisPtr->save_loaded;
                    altarPtr->reloadInventory();
                    altarPtr->initialized = true;
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Altar.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (barracksPtr == nullptr)
            {
                try
                {
                    barracksPtr = new Barracks;
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Barracks.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (obeliskPtr == nullptr)
            {
                try
                {
                    obeliskPtr = new ObeliskMenu;
                    obeliskPtr->Reload();
                    obeliskPtr->initialized = true;
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Obelisk.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            if (materPtr == nullptr)
            {
                try
                {
                    materPtr = new MaterOuterMenu;
                    materPtr->save_loaded = patapolisPtr->save_loaded;
                    materPtr->showMater();
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Mater.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            break;
        }

        case PATAPOLIS_ALTAR: {
            if(altarPtr == nullptr)
                altarPtr = new AltarMenu;

            break;
        }

        case BARRACKS: {
            if(barracksPtr == nullptr)
                barracksPtr = new Barracks;
            break;
        }

        case OBELISK: {
            if(obeliskPtr == nullptr)
                obeliskPtr = new ObeliskMenu;
            break;
        }

        case MISSIONCONTROLLER: {

            if (missionControllerPtr == nullptr)
            {
                try
                {
                    //Since MissionController is handled separately, tell CoreManager to reinitialize it
                    CoreManager::getInstance().reinitMissionController();

                    missionControllerPtr = CoreManager::getInstance().getMissionController();
                    missionControllerPtr->LoadMission(CoreManager::getInstance().getCore()->mission_file);

                    if (CoreManager::getInstance().getCore()->mission_id >= 0)
                    {
                        //missionControllerPtr->StartMission(CoreManager::getInstance().getCore()->mission_file, false, CoreManager::getInstance().getCore()->mission_id, CoreManager::getInstance().getCore()->mission_multiplier);
                    } else
                    {
                        SPDLOG_ERROR("No load mission specified, returning to Patapolis");
                        missionControllerPtr->loadingError = true;
                        setState(PATAPOLIS);
                        break;
                    }
                }
                catch ( ... )
                {
                    SPDLOG_ERROR("Could not load Mission.");
                    setState(ERROR_CHAMBER);

                    break;
                }
            }

            break;
        }

        case TEST_CHAMBER: {
            if(testChamberPtr == nullptr)
            {
                testChamberPtr = new TestChamber;
            }

            break;
        }

        case ERROR_CHAMBER: {
            break;
        }
    }
}

void StateManager::initStateMT(int state)
{
    loadingThreads.push_back(std::thread(&StateManager::initState, this, state));
}

void StateManager::parseCurrentStateEvents(const std::optional<sf::Event>& event)
{
    switch (currentGameState)
    {
        case NEWGAMEMENU: {
            break;
        }

        case MAINMENU: {
            if (mainMenuPtr == nullptr)
            {
                mainMenuPtr = new MainMenu;
            }
            break;
        }

        case MISSIONCONTROLLER: {
            break;
        }
    }
}

void StateManager::setState(int state)
{
    // Here is a good place to put specific events that always happen when changing states
    if(state == ERROR_CHAMBER)
    {
        if (errorChamberPtr != nullptr)
        {
            delete errorChamberPtr;
        }

        errorChamberPtr = new ErrorChamber;
        errorChamberPtr->badState = currentGameState;
    }

    //return from options to main
    if (currentGameState == OPTIONSMENU && state == MAINMENU)
    {
        if(mainMenuPtr == nullptr)
            initState(state);

        if (mainMenuPtr != nullptr)
        {
            mainMenuPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }

        CoreManager::getInstance().getCore()->changeRichPresence("In Main menu", "logo", "");
    }

    //go from main to options
    if (currentGameState == MAINMENU && state == OPTIONSMENU)
    {
        if(optionsMenuPtr == nullptr)
            initState(state);

        if (optionsMenuPtr != nullptr)
        {
            optionsMenuPtr->state = 0;
            optionsMenuPtr->sel = 0;
            optionsMenuPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        }

        CoreManager::getInstance().getCore()->changeRichPresence("In Options menu", "logo", "");
    }

    //go from main to introduction
    if (currentGameState == MAINMENU && state == INTRODUCTION)
    {
        if(introductionPtr == nullptr)
            initState(state);

        if (introductionPtr != nullptr)
        {
            introductionPtr->timeout.restart();

            //clean main menu components
            if (mainMenuPtr != nullptr)
            {
                delete mainMenuPtr;
                mainMenuPtr = nullptr;

                ResourceManager::getInstance().unloadState(MAINMENU);
            }

            if (optionsMenuPtr != nullptr)
            {
                delete optionsMenuPtr;

                optionsMenuPtr = nullptr;
                ResourceManager::getInstance().unloadState(OPTIONSMENU);
            }
        }

        CoreManager::getInstance().getCore()->changeRichPresence("A new adventure begins!", "logo", "");
    }

    //go from introduction to kami's return mission
    if (currentGameState == INTRODUCTION && state == MISSIONCONTROLLER)
    {
        //load tips
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        //clean introduction menu
        if (introductionPtr != nullptr)
        {
            delete introductionPtr;
            introductionPtr = nullptr;

            ResourceManager::getInstance().unloadState(INTRODUCTION);
        }

        //run tips and load up mission controller
        state = TIPS;
        afterTipState = MISSIONCONTROLLER;

        initStateMT(afterTipState);
    }

    //go from main to patapolis (forward through tips)
    if (currentGameState == MAINMENU && state == PATAPOLIS)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        //clean main menu components
        if (mainMenuPtr != nullptr)
        {
            delete mainMenuPtr;
            mainMenuPtr = nullptr;

            ResourceManager::getInstance().unloadState(MAINMENU);
        }

        if (optionsMenuPtr != nullptr)
        {
            delete optionsMenuPtr;
            optionsMenuPtr = nullptr;

            ResourceManager::getInstance().unloadState(OPTIONSMENU);
        }

        if (introductionPtr != nullptr)
        {
            delete introductionPtr;
            introductionPtr = nullptr;

            ResourceManager::getInstance().unloadState(INTRODUCTION);
        }

        state = TIPS;
        afterTipState = PATAPOLIS;

        initStateMT(afterTipState);

        CoreManager::getInstance().getCore()->changeRichPresence("In Ancient Patapolis", "logo", "");
    }

    // go from patapolis to main menu
    if (currentGameState == PATAPOLIS && state == MAINMENU)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip(1);
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip(1);
        }

        if (patapolisPtr != nullptr)
        {
            delete patapolisPtr;
            patapolisPtr = nullptr;
        }

        if (barracksPtr != nullptr)
        {
            delete barracksPtr;
            barracksPtr = nullptr;
        }

        if (altarPtr != nullptr)
        {
            delete altarPtr;
            altarPtr = nullptr;
        }

        if (obeliskPtr != nullptr)
        {
            delete obeliskPtr;
            obeliskPtr = nullptr;
        }

        ResourceManager::getInstance().unloadState(PATAPOLIS);

        state = TIPS;
        afterTipState = MAINMENU;

        initStateMT(afterTipState);

        CoreManager::getInstance().getCore()->changeRichPresence("In Main menu", "logo", "");
    }

    // go from patapolis to altar
    if (currentGameState == PATAPOLIS && state == PATAPOLIS_ALTAR)
    {
        if(altarPtr == nullptr)
            initState(state);

        if (altarPtr != nullptr)
        {
            altarPtr->reloadInventory();
        }
        else
        {
            altarPtr = new AltarMenu;
            altarPtr->reloadInventory();
        }
    }

    // go from patapolis to barracks
    if (currentGameState == PATAPOLIS && state == BARRACKS)
    {
        if(barracksPtr == nullptr)
            initState(state);

        if (barracksPtr != nullptr)
        {
            barracksPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
            barracksPtr->obelisk = false;
            barracksPtr->refreshStats();
            barracksPtr->updateInputControls();

            CoreManager::getInstance().getCore()->changeRichPresence("In Barracks", "logo", "");
        }
    }

    // go from barracks to patapolis
    if (currentGameState == BARRACKS && state == PATAPOLIS)
    {
        if(patapolisPtr == nullptr)
            initState(state);

        if (patapolisPtr != nullptr)
        {
            patapolisPtr->screenFade.Create(ScreenFade::FADEIN, 1024);

            CoreManager::getInstance().getCore()->changeRichPresence("In Ancient Patapolis", "logo", "");
        }
    }

    // go from patapolis to obelisk
    if (currentGameState == PATAPOLIS && state == OBELISK)
    {
        if(obeliskPtr == nullptr)
            initState(state);

        if (obeliskPtr != nullptr)
        {
            obeliskPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
            obeliskPtr->Reload();

            CoreManager::getInstance().getCore()->changeRichPresence("In Obelisk", "logo", "");
        }
    }

    // go from obelisk to patapolis
    if (currentGameState == OBELISK && state == PATAPOLIS)
    {
        if(patapolisPtr == nullptr)
            initState(state);

        if (patapolisPtr != nullptr)
        {
            patapolisPtr->screenFade.Create(ScreenFade::FADEIN, 1024);

            CoreManager::getInstance().getCore()->changeRichPresence("In Ancient Patapolis", "logo", "");
        }
    }

    // go from obelisk to barracks
    if (currentGameState == OBELISK && state == BARRACKS)
    {
        if(barracksPtr == nullptr)
            initState(state);

        barracksPtr->screenFade.Create(ScreenFade::FADEIN, 1024);
        barracksPtr->obelisk = true;
        barracksPtr->mission_id = obeliskPtr->missions[obeliskPtr->sel_mission].mis_ID;
        barracksPtr->mission_file = obeliskPtr->missions[obeliskPtr->sel_mission].mission_file;
        barracksPtr->refreshStats();
        barracksPtr->updateInputControls();

        CoreManager::getInstance().getCore()->changeRichPresence("In Barracks", "logo", "");
    }

    // go from barracks to obelisk
    if (currentGameState == BARRACKS && state == OBELISK)
    {
        if(obeliskPtr == nullptr)
            initState(state);

        if (obeliskPtr != nullptr)
        {
            obeliskPtr->screenFade.Create(ScreenFade::FADEIN, 1024);

            CoreManager::getInstance().getCore()->changeRichPresence("In Obelisk", "logo", "");
        }
    }

    //go from barracks to missioncontroller
    if (currentGameState == BARRACKS && state == MISSIONCONTROLLER)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        if (patapolisPtr != nullptr)
        {
            delete patapolisPtr;
            patapolisPtr = nullptr;
        }

        if (barracksPtr != nullptr)
        {
            delete barracksPtr;
            barracksPtr = nullptr;
        }

        if (altarPtr != nullptr)
        {
            delete altarPtr;
            altarPtr = nullptr;
        }

        if (obeliskPtr != nullptr)
        {
            delete obeliskPtr;
            obeliskPtr = nullptr;
        }

        ResourceManager::getInstance().unloadState(PATAPOLIS);

        state = TIPS;
        afterTipState = MISSIONCONTROLLER;

        initStateMT(afterTipState);
    }

    //[DEBUG] special case: go from introduction directly to mission
    if (currentGameState == ENTRY && state == MISSIONCONTROLLER)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        if (patapolisPtr != nullptr)
        {
            delete patapolisPtr;
            patapolisPtr = nullptr;
        }

        if (barracksPtr != nullptr)
        {
            delete barracksPtr;
            barracksPtr = nullptr;
        }

        if (altarPtr != nullptr)
        {
            delete altarPtr;
            altarPtr = nullptr;
        }

        if (obeliskPtr != nullptr)
        {
            delete obeliskPtr;
            obeliskPtr = nullptr;
        }

        ResourceManager::getInstance().unloadState(PATAPOLIS);

        state = TIPS;
        afterTipState = MISSIONCONTROLLER;

        initStateMT(afterTipState);
    }

    //go from missioncontroller to patapolis
    if (currentGameState == MISSIONCONTROLLER && state == PATAPOLIS)
    {
        if (loadingTipPtr == nullptr)
        {
            loadingTipPtr = new LoadingTip;
        } else
        {
            delete loadingTipPtr;
            loadingTipPtr = new LoadingTip;
        }

        //clean mission controller components
        if (missionControllerPtr != nullptr)
        {
            //we handle deletion on CoreManager's side
            CoreManager::getInstance().deleteMissionController();

            //but we also have a local pointer which we should set to null, to mark that it doesnt exist anymore
            missionControllerPtr = nullptr;

            //unload assets
            ResourceManager::getInstance().unloadState(MISSIONCONTROLLER);
        }

        state = TIPS;
        afterTipState = PATAPOLIS;

        initStateMT(afterTipState);

        CoreManager::getInstance().getCore()->changeRichPresence("In Ancient Patapolis", "logo", "");
    }

    //go from tips to patapolis (mission load error)
    if (missionControllerPtr != nullptr)
    {
        if (missionControllerPtr->loadingError)
        {
            if (currentGameState == TIPS && state == PATAPOLIS)
            {
                if (loadingTipPtr == nullptr)
                {
                    loadingTipPtr = new LoadingTip;
                } else
                {
                    delete loadingTipPtr;
                    loadingTipPtr = new LoadingTip;
                }

                state = TIPS;
                afterTipState = PATAPOLIS;

                initStateMT(afterTipState);

                CoreManager::getInstance().getCore()->changeRichPresence("In Ancient Patapolis", "logo", "");
            }
        }
    }

    // Change the state
    SPDLOG_DEBUG("Changing state to {}", state);
    prevGameState = currentGameState;
    currentGameState = state;
}

int StateManager::getState()
{
    if (currentGameState == TIPS)
        return afterTipState;
    else
        return currentGameState;
}