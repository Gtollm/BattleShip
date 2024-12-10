#include <fstream>
#include <gtest/gtest.h>
#include <string>

#include "lib/game/game.hpp"
#include "lib/game_manager/game_manager.hpp"

TEST(GameManagerTestSuite, PingGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("ping"), "pong");
}

TEST(GameManagerTestSuite, WidthGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("set width 4"), "ok");
  ASSERT_EQ(manager.ProcessInput("get width"), "4");
}

TEST(GameManagerTestSuite, HeightGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("set height 4"), "ok");
  ASSERT_EQ(manager.ProcessInput("get height"), "4");
}

TEST(GameManagerTestSuite, CountGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("set count 4 8"), "ok");
  ASSERT_EQ(manager.ProcessInput("get count 4"), "8");
  ASSERT_EQ(manager.ProcessInput("set count 2 3"), "ok");
  ASSERT_EQ(manager.ProcessInput("get count 2"), "3");
  ASSERT_EQ(manager.ProcessInput("set count 1 1"), "ok");
  ASSERT_EQ(manager.ProcessInput("get count 1"), "1");
  ASSERT_EQ(manager.ProcessInput("set count 3 33"), "ok");
  ASSERT_EQ(manager.ProcessInput("get count 3"), "33");
}

TEST(GameManagerTestSuite, StrategyGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("set strategy ordered"), "ok");
  ASSERT_EQ(manager.ProcessInput("set strategy custom"), "ok");
  ASSERT_EQ(manager.ProcessInput("set strategy ordered"), "ok");
}

TEST(GameManagerTestSuite, SlaveGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("create slave"), "ok");
  ASSERT_EQ(manager.ProcessInput("set height 33"), "ok");
  ASSERT_EQ(manager.ProcessInput("set width 120"), "ok");
  ASSERT_EQ(manager.ProcessInput("set count 4 8"), "ok");
  ASSERT_EQ(manager.ProcessInput("set count 2 3"), "ok");
  ASSERT_EQ(manager.ProcessInput("set count 1 1"), "ok");
  ASSERT_EQ(manager.ProcessInput("set count 3 33"), "ok");
  ASSERT_EQ(manager.ProcessInput("start"), "ok");
  ASSERT_EQ(manager.ProcessInput("win"), "no");
  ASSERT_EQ(manager.ProcessInput("lose"), "no");
  ASSERT_EQ(manager.ProcessInput("finished"), "no");
}

TEST(GameManagerTestSuite, MasterGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("create master"), "ok");

  ASSERT_EQ(manager.ProcessInput("start"), "ok");
  ASSERT_EQ(manager.ProcessInput("finished"), "no");
  ASSERT_EQ(manager.ProcessInput("win"), "no");
  ASSERT_EQ(manager.ProcessInput("lose"), "no");
}
TEST(GameManagerTestSuite, ShootGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("create master"), "ok");
  ASSERT_EQ(manager.ProcessInput("set strategy ordered"), "ok");
  ASSERT_EQ(manager.ProcessInput("start"), "ok");
  manager.ProcessInput("shot");
  ASSERT_EQ(manager.ProcessInput("set result hit"), "ok");
  ASSERT_EQ(manager.ProcessInput("win"), "no");
  ASSERT_EQ(manager.ProcessInput("lose"), "no");
  ASSERT_EQ(manager.ProcessInput("finished"), "no");
}

TEST(GameManagerTestSuite, DumpLoadGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("create master"), "ok");
  ASSERT_EQ(manager.ProcessInput("set strategy custom"), "ok");
  ASSERT_EQ(manager.ProcessInput("start"), "ok");
  ASSERT_EQ(manager.ProcessInput("dump my.txt"), "ok");
  ASSERT_EQ(manager.ProcessInput("load my.txt"), "ok");
}

TEST(GameManagerTestSuite, LoadGameManager) {
  BattleShipGame::Game game;
  BattleShipGame::GameManager manager(game);
  BattleShipGame::ConsoleWriter writer(std::cout);
  std::ofstream fout("test.txt");
  fout << "10 10" << std::endl;
  fout << "1 v 0 0" << std::endl;
  fout << "2 h 3 4" << std::endl;
  fout << "4 h 1 8" << std::endl;
  fout.close();
  manager.SetWriter(writer);
  ASSERT_EQ(manager.ProcessInput("create master"), "ok");
  ASSERT_EQ(manager.ProcessInput("set strategy custom"), "ok");
  ASSERT_EQ(manager.ProcessInput("start"), "ok");
  ASSERT_EQ(manager.ProcessInput("load test.txt"), "ok");
  ASSERT_EQ(game.GetPlayerField()->GetCellState(4, 4),
            BattleShipGame::CellState::SHIP);
}
