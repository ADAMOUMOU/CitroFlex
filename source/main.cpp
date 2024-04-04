// main.cpp
//
// Test file for the lib


#include "../include/cf_debug.hpp"
#include "../include/citroFlex.hpp"

#include <citro2d.h>
#include <citro3d.h>


// 
class MainScene : public cf::Scene {
private:
public:
    objects::Player mainPlayer;

    

    void init() {
        mainPlayer.init();
    }

    void update() override {
        cf::Scene::update();
        
    }
    
    void draw_top() {
        cf::Scene::draw_top();

    }

    void draw_bottom() {
        cf::Scene::draw_bottom();

    }
};

int main(int argc, char const *argv[]) {
    Debug::Logger logger;
    MainScene scene = MainScene();

    logger.init("aaa.txt");
    logger.log("AAAA");
    logger.log("[red]This is red !");
    
    scene.start();
}
