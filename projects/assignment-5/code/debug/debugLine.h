#include "debugShape.h"
#include "../Matrix4D.h"
#include "core/app.h"
#include "string.h"

class DebugLine : public DebugShape
{
    public:
        DebugLine();
        DebugLine(Vector4D point1, Vector4D point2);
        ~DebugLine();
        void draw(Matrix4D viewMatrix);
    
    private:
        void setup();
};