#include "game/scene/gameScene.h"

#include "world/terrain/block/block.h"

#include <optional>

GameScene::GameScene(InputBus &bus, Renderer &renderer)
    : world(42), gameView(world, renderer), bus(bus), sensibility(0.005f) {}

void GameScene::onEnter() { world.getChunkManager().startLoading(); }

void GameScene::onExit() { world.getChunkManager().stopLoading(); }

void GameScene::update(float dt) {
    applyMovement(dt);
    world.updateTick(dt);
}

void GameScene::render(Renderer &renderer) { gameView.render(); }

void GameScene::onEvent(const InputEvent &e) {
    if (auto a = bus.actionFor(e)) {
        bool pressed = false;
        if (auto k = std::get_if<KeyEvent>(&e))
            pressed = k->action == GLFW_PRESS;
        else if (auto b = std::get_if<MouseButton>(&e))
            pressed = b->action == GLFW_PRESS;

        if (pressed)
            triggerAction(*a);
        return;
    }

    if (auto m = std::get_if<MouseMove>(&e))
        world.getEntityManager().player->look(m->dx, m->dy, sensibility);
}

void GameScene::applyMovement(float dt) {
    Player &player = *world.getEntityManager().player;

    glm::vec3 newDir = glm::vec3(0.0f);
    if (bus.isActionActive(MoveFoward))
        newDir += player.forward();
    if (bus.isActionActive(MoveBackward))
        newDir -= player.forward();
    if (bus.isActionActive(MoveLeft))
        newDir += player.right();
    if (bus.isActionActive(MoveRight))
        newDir -= player.right();

    player.move(newDir, dt);

    if (bus.isActionActive(Jump))
        player.jump();

    if (bus.isActionActive(IncreaseFOV))
        gameView.camera.setFOV(gameView.camera.getFOV() + glm::radians(1.0));
    if (bus.isActionActive(DecreaseFOV))
        gameView.camera.setFOV(gameView.camera.getFOV() - glm::radians(1.0));
}

void GameScene::triggerAction(Action action) {
    switch (action) {
    case DestroyBlock:
        destroyLookedBlock();
        break;
    case PlaceBlock:
        placeLookedBlock();
        break;
    case HotbarSlot1:
    case HotbarSlot2:
    case HotbarSlot3:
    case HotbarSlot4:
    case HotbarSlot5:
    case HotbarSlot6:
    case HotbarSlot7:
    case HotbarSlot8:
    case HotbarSlot9:
        world.getEntityManager().player->hotbarIndex =
            action - HotbarSlot1 + 1;
        break;
    default:
        break;
    }
}

void GameScene::destroyLookedBlock() {
    Player &player = *world.getEntityManager().player;
    glm::vec3 initial_pos =
        player.position + player.size * glm::vec3(0.5f, 0.9f, 0.5f);
    glm::vec3 dir = glm::normalize(player.direction);
    glm::vec3 i = glm::vec3(0);

    while (world.getChunkManager()
               .getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
               .id == BlockID::air) {
        if (i.x > 6)
            return;
        i += glm::vec3(0.25);
    }

    world.getChunkManager().setBlock(player.chunkx, player.chunkz,
                                      initial_pos + dir * i, Block());
}

void GameScene::placeLookedBlock() {
    Player &player = *world.getEntityManager().player;
    glm::vec3 initial_pos =
        player.position + player.size * glm::vec3(0.5f, 0.9f, 0.5f);
    glm::vec3 dir = glm::normalize(player.direction);
    glm::vec3 i = glm::vec3(0);

    while (world.getChunkManager()
               .getBlock(player.chunkx, player.chunkz, initial_pos + dir * i)
               .id == BlockID::air) {
        if (i.x > 6)
            return;
        i += glm::vec3(0.25);
    }
    i -= glm::vec3(0.25);

    if (!Entity::colision(initial_pos + dir * i, glm::vec3(0, 0, 0),
                          glm::floor(player.position),
                          glm::ceil(player.position + player.size) -
                              glm::floor(player.position))) {
        world.getChunkManager().setBlock(
            player.chunkx, player.chunkz, initial_pos + dir * i,
            Block((BlockID)player.hotbarIndex, true));
    }
}

