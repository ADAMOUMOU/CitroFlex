#include "cf_objects.hpp"
//#include "cf_debug.hpp"

#include <vector>
#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>

// Classe scene avec init et update de tous les objects dans cette derniere
// Possibilité d'y mettre des objects si elle n'est pas en train d'être render
// La classe scene permet aussi de draw des elements


namespace cf
{
    class Scene
    {
    private:
        int currentID = -1;  // -1 = no elements
        std::vector<objects::Object> upScreenSceneObjects;
        std::vector<objects::Object> bottomScreenSceneObjects;
        std::vector<objects::Object> sceneObjects;      // Vector with all objects in scene

        C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
        C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    public:
        bool canLoop = false;       // Manage if the scene can update or not

        void start() {
            init();
            canLoop = true;
            while (canLoop) {
                update();
            }
        }

        virtual void init() {
            /*
             Can be override
            */
        }

        void prepare_top() {
            // Render to the top
		    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		    C2D_TargetClear(top, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));	// Clear screen
		    C2D_SceneBegin(top);
        }

        void prepare_bottom() {
            // Render to the bottom
		    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		    C2D_TargetClear(bottom, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));	// Clear screen
		    C2D_SceneBegin(bottom);
        }
        
        void add_object(objects::Object object, bool isTopScreen) {
            currentID++;
            object.id = currentID;
            sceneObjects.push_back(object);

            if (isTopScreen) {
                upScreenSceneObjects.push_back(object);
            }
            else {
                bottomScreenSceneObjects.push_back(object);
            }
        }

        virtual void update() {
            /*
             Can be overrided
             You need to call parent update (Scene::update()) in order
             to update top and bottom screens.
            */
            draw_top();
            draw_bottom();
        }

        void draw_top() {
            prepare_top();

            for (objects::Object &object : upScreenSceneObjects) {
                object.update();
            }
        }

        void draw_bottom() {
            prepare_bottom();

            for (objects::Object &object : bottomScreenSceneObjects) {
                object.update();
            }
        }

        virtual void destroy() {
            /*
             Can be overrided
            */
            for (objects::Object &object : sceneObjects) {
                if (menuobj::Text *textObj = reinterpret_cast<menuobj::Text*>(&object)) {
                    textObj->free();
                }
                
                if (menuobj::Button *buttonObj = reinterpret_cast<menuobj::Button*>(&object)) {
                    buttonObj->free();
                }
            }
        }
    };
    
    
} // namespace cf
