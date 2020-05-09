#include "BulletsSystem.h"
#include "Manager.h"
#include "SDLGame.h"
#include "BulletsPool.h"
#include "Transform.h"

BulletsSystem::BulletsSystem() :
		System(ecs::_sys_Bullets) {
}

void BulletsSystem::shoot(Vector2D pos, Vector2D vel, double w, double h) {
	Entity *b = mngr_->addEntity<BulletsPool>(pos,vel,w,h);
	if (b != nullptr) {
		b->addToGroup(ecs::_grp_Bullets);
	}
}

void BulletsSystem::disableAll() {
	for (auto &b : mngr_->getGroupEntities(ecs::_grp_Bullets))
		b->setActive(false);
}

void BulletsSystem::update() {
	for (auto &e : mngr_->getGroupEntities(ecs::_grp_Bullets)) {
		Transform *tr = e->getComponent<Transform>(ecs::Transform);

		Vector2D p = tr->position_ + tr->velocity_;

		if ((p.getX() >= game_->getWindowWidth())
				|| (p.getX() + tr->width_ <= 0)
				|| (p.getY() >= game_->getWindowHeight())
				|| (p.getY() + tr->height_ <= 0)) {
			e->setActive(false);
		} else {
			tr->position_ = p;
		}
	}
}

void BulletsSystem::recieve(const msg::Message& msg)
{
	switch (msg.id)
	{
	case msg::_FIGHTER_INFO: 
		break;
	case msg::_PLAYER_INFO:
		break;
	case msg::_CLIENT_DISCONNECTED:
		break;
	case msg::_BULLET_SHOOT_INFO: {
		if (msg.senderClientId == mngr_->getClientId()) return;

		msg::BulletShootMessage info = static_cast<const msg::BulletShootMessage&>(msg);
		shoot(Vector2D(info.x, info.y), Vector2D(info.velx, info.vely), info.w, info.h);
		break;
	}
	case msg::_FIGHTER_DEAD_:
		disableAll();
		break;
	default:
		break;
	}
}

