#pragma once
#include <box2d/box2d.h>
#include <vector>
#include <SFML/System.hpp>
#include <iostream>

class MyContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

     
        if (bodyA == sunBody || bodyB == sunBody) {
            b2Body* planetBody = (bodyA == sunBody) ? bodyB : bodyA;
            if (planetBody->GetType() == b2_dynamicBody) {
                planetsToRemove.push_back(planetBody);
            }
        }
    }

    void EndContact(b2Contact* contact) override {}

    void setWorld(b2World* worldPtr) {
        world = worldPtr;
    }

    void setSunBody(b2Body* sun) {
        sunBody = sun;
    }

    std::vector<b2Body*>& getPlanetsToRemove() {
        return planetsToRemove;
    }

private:
    b2World* world = nullptr;
    b2Body* sunBody = nullptr;
    std::vector<b2Body*> planetsToRemove;
};

class PhysicsHandler {
public:
    PhysicsHandler() {
        b2Vec2 gravity(0.0f, 0.0f); 
        world = new b2World(gravity);
        contactListener.setWorld(world);
        world->SetContactListener(&contactListener);
        starBody = nullptr;
    }

    ~PhysicsHandler() {
        delete world;
    }

    void addStar(float x, float y) {
        b2BodyDef starDef;
        starDef.position.Set(x / SCALE, y / SCALE);
        starBody = world->CreateBody(&starDef);

        b2CircleShape starShape;
        starShape.m_radius = STAR_RADIUS / SCALE;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &starShape;
        fixtureDef.isSensor = true;
        starBody->CreateFixture(&fixtureDef);

        contactListener.setSunBody(starBody);
    }

    void addPlanet(float x, float y) {
        b2BodyDef planetDef;
        planetDef.type = b2_dynamicBody; 
        planetDef.position.Set(x / SCALE, y / SCALE);
        b2Body* planetBody = world->CreateBody(&planetDef);

        b2CircleShape planetShape;
        planetShape.m_radius = PLANET_RADIUS / SCALE;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &planetShape;
        fixtureDef.density = PLANET_MASS;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        planetBody->CreateFixture(&fixtureDef);

        b2Vec2 sunPosition = starBody->GetPosition();
        b2Vec2 direction = planetBody->GetPosition() - sunPosition;
        float distance = direction.Length();

        direction.Normalize();
        float speed = sqrt(GRAVITATIONAL_CONSTANT * STAR_MASS / distance);
        b2Vec2 tangentialVelocity(-direction.y, direction.x);
        tangentialVelocity *= speed;

        planetBody->SetLinearVelocity(tangentialVelocity );
        planets.push_back(planetBody);
    }


    

    void update() {
        world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

        if (starBody) {
            b2Vec2 sunPosition = starBody->GetPosition();

            for (b2Body* planet : planets) {
                if (planet) {
                    b2Vec2 planetPosition = planet->GetPosition();
                    b2Vec2 direction = sunPosition - planetPosition;
                    float distance = direction.Length();

                    direction.Normalize();

                    b2Vec2 gravitationalForce =  (GRAVITATIONAL_CONSTANT * STAR_MASS * PLANET_MASS / (distance * distance))*direction;
                    planet->ApplyForceToCenter(gravitationalForce, true);
                }
            }
        }

      
        for (b2Body* planet : contactListener.getPlanetsToRemove()) {
            world->DestroyBody(planet);
            planets.erase(std::remove(planets.begin(), planets.end(), planet), planets.end());
        }
        contactListener.getPlanetsToRemove().clear();
    }

    sf::Vector2f getPlanetPosition(size_t index) {
        if (index < planets.size()) {
            b2Vec2 pos = planets[index]->GetPosition();
            return sf::Vector2f(pos.x * SCALE, pos.y * SCALE);
        }
        return sf::Vector2f();
    }

private:
    b2World* world;
    b2Body* starBody;
    std::vector<b2Body*> planets;
    MyContactListener contactListener;

    const float SCALE = 20.f;
    const float TIME_STEP = 1.0f / 70.0f;
    const int VELOCITY_ITERATIONS = 8;
    const int POSITION_ITERATIONS = 3;

    const float STAR_RADIUS = 50.f;
    const float PLANET_RADIUS = 10.f;
    const float STAR_MASS = 1000.f;
    const float PLANET_MASS = 1.f;
    const float GRAVITATIONAL_CONSTANT = 0.3f;
};




