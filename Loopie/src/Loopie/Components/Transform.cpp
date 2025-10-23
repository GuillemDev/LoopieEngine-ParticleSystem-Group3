#include "Transform.h"
namespace Loopie
{
    Transform::Transform() :
        globalMatrix(1.0f),
        position(0.0f), rotation(1, 0, 0, 0), scale(1.0f), eulerAngles(0, 0, 0),
        localScale(1.0f), localRotation(1, 0, 0, 0), localEulerAngles(0, 0, 0)
    {}

    void Transform::translate(const vec3& translation, bool local)
    {
        if (local) {
            localPosition += translation;
            position += localRotation * translation;
        }
        else {
            position += rotation * translation;
        }
    }

    void Transform::rotate(const vec3& axis, float angle, bool local)
    {
        quaternion rotationquat = glm::angleAxis(glm::radians(angle), axis);

        if (local) {
            localRotation = rotationquat;
            localEulerAngles = glm::eulerAngles(localRotation);
        }
        else {
            rotation = rotationquat;
            this->eulerAngles = glm::eulerAngles(rotation);
        }
    }

    void Transform::rotate(const vec3& eulerAngles, bool local)
    {
        quaternion rotationquat = glm::quat(glm::radians(eulerAngles));

        if (local) {
            localRotation = rotationquat;
            localEulerAngles = glm::eulerAngles(localRotation);
        }
        else {
            rotation = rotationquat;
            this->eulerAngles = glm::eulerAngles(rotation);
        }
    }

    void Transform::scaleBy(const vec3& scaling, bool local)
    {
        if (local) {
            localScale *= scaling;
        }
        else {
            scale *= scaling;
        }
    }

    vec3 Transform::getForward()
    {
        updateMatrix();
        return glm::normalize(globalMatrix[2]);
    }

    vec3 Transform::getUp()
    {
        updateMatrix();
        return glm::normalize(globalMatrix[1]);
    }

    vec3 Transform::getRight()
    {
        updateMatrix();
        return glm::normalize(globalMatrix[0]);
    }

    matrix4 Transform::getMatrix()
    {
        updateMatrix();
        return globalMatrix;
    }

    void Transform::updateMatrix()
    {
        globalMatrix = matrix4(1.0f);
        globalMatrix = glm::translate(globalMatrix, position);
        globalMatrix *= glm::mat4_cast(rotation * localRotation);
        globalMatrix = glm::scale(globalMatrix, localScale);
        globalMatrix = glm::scale(globalMatrix, scale);
    }

    matrix4 Transform::GetWorldTransform()
    {
        return globalMatrix;
    }

    vec3 Transform::getPosition() const
    {
        return position;
    }

    void Transform::setPosition(const vec3& newPosition)
    {
        position = newPosition;
    }

    quaternion Transform::getRotation() const
    {
        return rotation;
    }

    quaternion Transform::getLocalRotation() const
    {
        return localRotation;
    }

    vec3 Transform::getEulerAngles() const
    {
        vec3 eulerAngles = glm::eulerAngles(rotation);
        return eulerAngles;
    }

    vec3 Transform::getLocalEulerAngles() const
    {
        vec3 eulerAngles = glm::eulerAngles(localRotation);
        return eulerAngles;
    }

    void Transform::setRotation(const vec3& newRotation)
    {
        eulerAngles = newRotation;
        rotation = EulerAnglesToQuaternion(eulerAngles);
    }

    vec3 Transform::getScale() const
    {
        return scale;
    }

    void Transform::setScale(const vec3& newScale)
    {
        scale = newScale;
    }

    quaternion Transform::EulerAnglesToQuaternion(const vec3& eulerAngles)
    {
        quaternion newRotation;
        newRotation = glm::angleAxis(eulerAngles.z, vec3(0, 0, 1));
        newRotation *= glm::angleAxis(eulerAngles.y, vec3(0, 1, 0));
        newRotation *= glm::angleAxis(eulerAngles.x, vec3(1, 0, 0));
        return newRotation;
    }
}