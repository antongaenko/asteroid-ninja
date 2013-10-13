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

#include "Game.h"
#include "Space.h"
#include "Logger.h"
#include "SpaceArchitect.h"

const int Game::FPS = SpaceArchitect::DESIGN_FPS;

Game::Game():_lives(3), _score(0) {
  info("Start game initialization...");
  _space = std::unique_ptr<Space>(new Space());
  
  // seed random
  std::srand((unsigned)time(0));
  
  // bind space events to external listeners
  _space->setListener([&](SpaceEvent e) {
    switch (e) {
      case SpaceEvent::SHIP_CRASH:
        _lives--;
        _score = max(_score + SCORE_FOR_SHIP_CRASH, 0);
        if (_onGameEvent) _onGameEvent(GameEvent::SHIP_CRASH);
        break;
        
      case SpaceEvent::ASTEROID_DESTROYED:
        if (_onGameEvent) _onGameEvent(GameEvent::ASTEROID_BANG);
        _score += SCORE_FOR_ASTEROID_DESTRUCTION;
        _onGameEvent(GameEvent::SCORE_CHANGES);
        break;
        
      case SpaceEvent::ASTEROID_CRACK:
        if (_onGameEvent) _onGameEvent(GameEvent::ASTEROID_BANG);
        _score += SCORE_FOR_ASTEROID_CRACK;
        _onGameEvent(GameEvent::SCORE_CHANGES);
        break;
        
      default:
        error("unknown event %d",e);
        break;
    }
  });
  info("Finish game initialization.");
}

Game::~Game() {
  info("Destroy game. See you!");
}

int Game::getPlayerSpeedMax() const {
  return SpaceArchitect::SHIP_VELOCITY_MAX;
}

int Game::getCurrentLives() {
  return _lives;
}

int Game::getCurrentScore() {
  return _score;
}

void Game::movePlayer(float dx, float dy, float curAngle) {
  _space->moveShip(dx, dy, curAngle);
}

Drawable *Game::getCanvas() {
  return _space.get();
}

void Game::playerAttack() {
  _space->shipAttack();
}

void Game::setListener(std::function<void(GameEvent)> l) {
  _onGameEvent = l;
}