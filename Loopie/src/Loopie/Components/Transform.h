#include "Loopie/Components/Component.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Core/Math.h"
#include <memory>
namespace Loopie
{
    class Transform : public Component
    {
    public:
        Transform();
        ~Transform() = default;

        void translate(const vec3& translation, bool local = true);
        void rotate(const vec3& axis, float angle, bool local = true);
        void rotate(const vec3& eulerAngles, bool local = true);
        void scaleBy(const vec3& scaling, bool local = true);

        vec3 getForward();  
        vec3 getUp();
        vec3 getRight();

        matrix4 getMatrix();
        void updateMatrix();
        matrix4 GetWorldTransform();

        vec3 getPosition() const;
        void setPosition(const vec3& newPosition);

        quaternion getRotation() const;
        quaternion getLocalRotation() const;

        vec3 getEulerAngles() const;
        vec3 getLocalEulerAngles() const;
        void setRotation(const vec3& newRotation);

        vec3 getScale() const;
        void setScale(const vec3& newScale);

        quaternion EulerAnglesToQuaternion(const vec3& eulerAngles);

    public:

        vec3 position;
        vec3 localPosition;
        quaternion rotation;
        quaternion localRotation;
        vec3 eulerAngles;
        vec3 localEulerAngles;
        vec3 scale;
        vec3 localScale;
        matrix4 globalMatrix;
    };
}