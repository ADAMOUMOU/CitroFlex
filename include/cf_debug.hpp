// cf_logging.hpp
//
// This header will arbitrary written messages to a file in the sd card

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>

namespace Debug
{
    class Logger
    {
    private:
        const char *filename; 
    public:
        void init(const char *file) {
            filename = file;
            fsInit();
        }

        bool log(char data[]) {
            FILE *file = fopen(filename, "a");
            size_t dataSize = sizeof(data);
            size_t elementsWritten = fwrite(data, dataSize, sizeof(data) / dataSize, file);
            fclose(file);

            // Check if the writing was successful
            if (elementsWritten == sizeof(data) / dataSize) {
                return true;
            } else {
                return false;
            }
        }

        void destroy() {
            fsExit();
        }
    };
} // namespace debug
