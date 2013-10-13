/*
 Copyright (C) 2013 by Anton Gaenko
 Mail anton.gaenko@yahoo.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <functional>
#include <map>

class Shader;
class Space;
class Drawable;
enum class SpaceEvent;

enum class GameEvent {
  ASTEROID_BANG,
  SHIP_CRASH,
  SCORE_CHANGES,
  LIVES_CHANGES,
  PAUSE,
  END
};

class Game {
public:
  static const int FPS;
  static const int SCORE_FOR_SHIP_CRASH = -50;
  static const int SCORE_FOR_ASTEROID_CRACK = 50;
  static const int SCORE_FOR_ASTEROID_DESTRUCTION = 100;

  Game();
  ~Game();

  int getPlayerSpeedMax() const;

  int getCurrentLives();
  int getCurrentScore();
  void movePlayer(float dx, float dy, float curAngle);
  void playerAttack();
  Drawable* getCanvas();
  
  void setListener(std::function<void(GameEvent)> l);

private:
  // player lives
  int _lives;
  // game score
  int _score;
  std::unique_ptr<Space> _space;
  // external listener for game events
  std::function<void(GameEvent)> _onGameEvent;
};

#endif /* GAME_H */