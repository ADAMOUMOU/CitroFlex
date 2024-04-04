#pragma once
#include "cf_randoms.hpp"

#include <vector>
#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>

#define MAX_LAZERS 5

u32 backgroundColor = clr::clrClear;

namespace objects
{
	class Object {
	public:
		double xpos;
		double ypos;
		int id = -1;	// ID given by the scene (-1: Not bound to a scene)

		virtual void init() {}
		virtual void update() {}
	};

	int numLazers = 0;

	class Lazer : Object {
	public:
		float xStartPos = 0;
		float yStartPos = 0;
		float xEndPos = 0;
		float yEndPos = 0;

		u32 lineClr = clr::clrMagenta;
		u32 firstPassAliasingColor = C2D_Color32(0xFF, 0x6C, 0x58, 0xB3);
		u32 secondPassAliasingColor = C2D_Color32(0xFF, 0xA2, 0x84, 0x8D);

		float thickness = 0;

		bool canDealDamage = false;

		//	/!\ Works only with VSync because shitty implementation
		int destroyCounter = 0;
		int destroyCounterMax = (2 * 60);	// seconds before destroy / frame per seconds

		bool toDestroy = false;	// Used for the update_lazers function to know if this instance of Lazer nedd to be destroyed

		void spawn() {
			if (numLazers >= MAX_LAZERS) {
				maths::vec2 startPos;
				maths::vec2 endPos;

				randoms::generateScreenCoordinates(GSP_SCREEN_WIDTH, GSP_SCREEN_WIDTH, startPos, endPos);

				xStartPos = startPos.x;
				yStartPos = startPos.y;
				xEndPos = endPos.x;
				yEndPos = endPos.y;
			}
		}

		// Lazer update loop with auto-destroy
		void update() override {
			C2D_DrawLine(yStartPos, xStartPos, firstPassAliasingColor, yEndPos, xEndPos, secondPassAliasingColor, thickness + 1, 1);
			C2D_DrawLine(yStartPos, xStartPos, firstPassAliasingColor, yEndPos, xEndPos, firstPassAliasingColor, thickness + 0.5, 1);
			C2D_DrawLine(yStartPos, xStartPos, lineClr, yEndPos, xEndPos, lineClr, thickness, 1);
			
			// Add thickness and lerp from Magenta to red with
			if (thickness < 1) {
				thickness += 0.02;
				lineClr = maths::lerpColor(clr::clrMagenta, clr::clrRed, thickness);
			}
			else {
				canDealDamage = true;
				destroyCounter++;
			}

			if (destroyCounter >= destroyCounterMax) {
				toDestroy = true;
			}
		}
	};

	std::vector<objects::Lazer> lazers;
	bool isDelayFinished = true;

	// Fonction pour le thread asynchrone
    void async_wait_random_time(void *arg) {
		isDelayFinished = false;
        // Generate delay between 1 and 5 sec
        int randomDelay = (rand() % 5 + 1);  // In seconds
        u32 milliseconds = randomDelay * 1000;	// Convert delay to ms

        // wait specified time
        svcSleepThread(static_cast<s64>(milliseconds));	//  * 1000LL

        Lazer currentLazer;
		currentLazer.spawn();
		lazers.push_back(currentLazer);

		isDelayFinished = true;
    }

	void random_lazers_spawn() {
        // Create async thread
		if (isDelayFinished) {
			Thread thread = threadCreate(async_wait_random_time, nullptr, 32 * 1024, 0x19, -2, true);
		}
        

        //if (thread != NULL) {
        //    // Attendez que le thread se termine (vous pouvez également laisser le thread s'exécuter en arrière-plan)
        //    threadJoin(thread, U64_MAX);
        //    threadFree(thread);
        //}
    }

	struct ColliderHitResult {
        bool isHit;
        Lazer lazer;
    };

	// Update or destroy lazers have to be executed in GFX function
	int update_lazers() {
		int counter = 0;

		std::vector<objects::Lazer> lazersToKeep; // Temporary vector for lazers to keep

		for (objects::Lazer &lazer : lazers) {
		    if (!lazer.toDestroy) {
		        lazer.update();
		        lazersToKeep.push_back(lazer); // Add element to temorary vector
		    }
		}
		lazers = lazersToKeep; // Copy elements to keep in the original vector


		if (numLazers != 10) {
			random_lazers_spawn();
		}
		
		return counter;
	}

	// Set toDestroy to all lazers to true and update lazers list
	void destroy_all_lazers() {
		for (objects::Lazer &lazer : lazers) {
		    lazer.toDestroy = true;
		}
		update_lazers();
	}

    class Player : Object {
    public:
        double xPos = 50;
    	double yPos = 50;

    	// u8 max size is 255
    	u8 width = 25;
    	u8 height = 25;

    	float xSpeed = 3.5;
    	float ySpeed = 3.5;

    	u32 playerClr = clr::clrBlack;	// Base player color

		bool isColliding = false;

		u8 health = 5;	// Base health

        void init() override {
		    xPos = 100;
		    yPos = 50;
		    C2D_DrawRectSolid(xPos, yPos, 0, width, height, playerClr);
	    }

	    void setPos(double newX, double newY) {
	    	xPos = newX;
	    	yPos = newY;
	    }
    
	    void addPos(double newX, double newY) {
	    	xPos += newX;
	    	yPos += newY;
	    }

	    void setSize(u8 newWidth, u8 newHeight) {
	    	width = newWidth;
	    	height = newHeight;
	    }

	    void addSize(u8 newWidth, u8 newHeight) {
	    	width += newWidth;
	    	height += newHeight;
	    }

	    void setColor(u32 color) {
	    	playerClr = color;
	    }

	    void moveUp() {xPos -= xSpeed; }
	    void moveDown() {xPos += xSpeed; }
	    void moveLeft() {yPos -= ySpeed; }
	    void moveRight() {yPos += ySpeed; }

	    void update() override {
	    	C2D_DrawRectSolid(yPos, xPos, 0, width, height, playerClr);

			// Check collision
			is_colliding(lazers);
	    }

		// Reads inputs and move player
        void player_movements() {
            u32 kHeld = hidKeysHeld();

	        // On up button clicked
	        if ((kHeld & KEY_UP)) {		// & !(xPos <= 0)
	        	moveUp();
	        }

	        // On down button clicked
	        if ((kHeld & KEY_DOWN)) {	// & !(xPos >= GSP_SCREEN_WIDTH-height)
	        	moveDown();
	        }

	        // On left button clicked
	        if ((kHeld & KEY_LEFT)) {	// & !(yPos <= 0)
	        	moveLeft();
	        }

	        // On right button clicked
	        if ((kHeld & KEY_RIGHT)) {
	        	moveRight();
	        }

			// if player get out of screen bounds
			if (xPos < 0) { xPos = 0; } 
			if (xPos > GSP_SCREEN_WIDTH-width) { xPos = GSP_SCREEN_WIDTH-width; }
			if (yPos < 0) { yPos = 0; }
			if (yPos > GSP_SCREEN_HEIGHT_TOP-height) { yPos = GSP_SCREEN_HEIGHT_TOP-height; }
        }

		ColliderHitResult is_colliding(const std::vector<Lazer> &lazers) {
		    ColliderHitResult result;
		    result.isHit = false;

		    for (const Lazer& currentLazer : lazers) {
				if (currentLazer.canDealDamage) {
		        	const maths::vec2 &lazerStart = {currentLazer.xStartPos, currentLazer.yStartPos};
		        	const maths::vec2 &lazerEnd = {currentLazer.xEndPos, currentLazer.yEndPos};

		        	if (maths::CheckCollision(xPos, yPos, width, lazerStart, lazerEnd)) {
						isColliding = true;
		        	    result.isHit = true;
		        	    result.lazer = currentLazer;
		        	    return result;
		        	}
				}
		    }

			isColliding = false;

		    return result;
		}
	};
} // namespace objects


namespace menuobj
{
    class Text : objects::Object {
    public:
        C2D_Text text;
        C2D_TextBuf textBuf;

        float xPos = GSP_SCREEN_HEIGHT_TOP / 2;
        float yPos = GSP_SCREEN_WIDTH / 2;

        float uniSize = 1;
		
		int flag = C2D_AlignCenter; // Aligned by default

        void init(const char *string, bool optimize = true) {
            textBuf = C2D_TextBufNew(1024);
            C2D_TextParse(&text, textBuf, string);
            C2D_TextOptimize(&text);

        }

        void update(const char *newText = "", bool optimize = true) {
			if (newText != "") {
				init(newText, optimize);
			}
			
            C2D_DrawText(&text, flag, xPos, yPos, 1.0f, uniSize, uniSize);

        }
        void free() {
        	// Delete the text buffer
        	C2D_TextBufDelete(textBuf);

        }
    };

    class Button : objects::Object {
    private:
        u32 color;

		// Attached text related variables
		Text *attachedText = nullptr;
		char *text;
    public:
        float xPos = 50;
    	float yPos = 50;

        // u8 max size = 255
    	u8 width = 100;
    	u8 height = 37;

        u32 baseClr = clr::clrBlue;
        u32 highlightedClr = clr::clrCian;

		bool isClicked = false;

		void (*boundOnClicked)();	// Function to execute on click

		/**
		 * @brief Initialize Button class
		 * @param x X position of the button, -1 for centered
		 * @param y Y position of the button, -1 for centered
		**/
        void init(float x, float y) {
			if (x == -1) {
				xPos = GSP_SCREEN_HEIGHT_TOP / 2;
			} else {
				xPos = x;
			}

			if (y == -1) {
				yPos = GSP_SCREEN_WIDTH / 2;
			} else {
				yPos = y;
			}
		    
            color = baseClr;
		    C2D_DrawRectSolid(xPos, yPos, 0, width, height, color);
	    }

        void update() {
	    	C2D_DrawRectSolid(yPos, xPos, 0, width, height, color);

			// Check collision
			isClicked = is_colliding();
			

            // Highlighting button
            if (isClicked) {
                color = highlightedClr;
				(*boundOnClicked)();
            }
            else {
                color = baseClr;
            }

			// Updates the text if attached to this button
			if (attachedText != nullptr) {
				attachedText->update();
			}
	    }

        bool is_colliding() {
            bool result;
            touchPosition touchPosition;

		    //Read the touch screen coordinates
		    hidTouchRead(&touchPosition);

            maths::vec2 buttonPos;
            buttonPos.x = xPos;
            buttonPos.y = yPos;

        	maths::vec2 touchPos;
            touchPos.x = touchPosition.px;
            touchPos.y = touchPosition.py;

			bool isTouchScreenPressed = !(touchPos == maths::VECZERO);
            result = maths::isPointInRectangle(buttonPos, width, height, touchPos);
			
            return result && isTouchScreenPressed;
        }

		void addText(char* string) {
			text = string;
			attachedText = new Text();
			attachedText->xPos = xPos + 20;
			attachedText->yPos = yPos - 5;
			attachedText->flag = C2D_AlignLeft;

			attachedText->init(text);
		}

		void free() {
			// Check if text exists
			if (attachedText != nullptr) {
				attachedText->free();
				attachedText = nullptr;
			}
		}
    };
} // namespace menu
