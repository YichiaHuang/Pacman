// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/MotionScene.hpp"
#include "Scene/InfoScene.hpp"
#include "Scene/SecondScene.hpp"
#include "Scene/Win.hpp"
#include "Scene/BoardSecond.hpp"
#include "Scene/Lose.hpp"
int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // TODO HACKATHON-2 (2/3): Register Scenes here
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("settings", new SettingsScene());
    game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win-scene", new WinScene());
	game.AddNewScene("scoreboard", new ScoreboardScene());
	game.AddNewScene("motion", new MotionScene());
	game.AddNewScene("info", new InfoScene());
	game.AddNewScene("second", new SecondScene());
	game.AddNewScene("win_second", new Win());
	game.AddNewScene("score_second", new BoardSecond());
	game.AddNewScene("lose_second", new Lose());

    // TODO HACKATHON-1 (1/1): Change the start scene
	//game.Start("motion", 60, 1600, 832);
	game.Start("motion", 60, 1600, 832);
	return 0;
}
