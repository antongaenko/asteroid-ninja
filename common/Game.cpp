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

Game::Game():_lives(3), _score(0) {
  info("Start game initialization...");
  _space = std::unique_ptr<Space>(new Space());
  
  // seed random
  std::srand((unsigned)time(0));
  
  // bind space events to external listeners
  _space->setListener([&](SpaceEvent e) {
    switch (e) {
      case SpaceEvent::SHIP_CRASH:
        if (_onGameEvent) _onGameEvent(GameEvent::SHIP_CRASH);
        _score += SCORE_FOR_SHIP_CRASH;
        debug("ship crash!");
        break;
        
      case SpaceEvent::ASTEROID_CRACK:
        debug("asteroid bang!!");
        if (_onGameEvent) _onGameEvent(GameEvent::ASTEROID_BANG);
        _score += SCORE_FOR_ASTEROID_CRACK;
        break;
        
      default:
        debug("unknown event %d",e);
        break;
    }
    if (_onGameEvent) _onGameEvent(GameEvent::SCORE_CHANGES);
  });
  info("Finish game initialization.");
}

Game::~Game() {}

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