#include "SceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "GameClear.h"
#include "GameOver.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    //éüÇÃÉVÅ[ÉìÇê∂ê¨
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new TitleScene();
    }

    else if (sceneName == "GAMEPLAY")
    {
        newScene = new GameScene();
    }

    else if (sceneName == "GAMECLEAR")
    {
        newScene = new GameClear();
    }

    else if (sceneName == "GAMEOVER")
    {
        newScene = new GameOver();
    }

    return newScene;
}
