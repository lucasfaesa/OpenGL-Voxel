#pragma once

#include <glm/glm.hpp>

struct Plane {
    glm::vec3 normal;
    float distance;

    // Default constructor
    Plane() : normal(0.0f), distance(0.0f) {}

    // Normalize the plane so the distance math is accurate
    void Normalize() {
        float length = glm::length(normal);
        normal /= length;
        distance /= length;
    }

    // Returns the distance of a point from the plane
    // > 0 means the point is in front of the plane
    float GetSignedDistance(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

struct Frustum {
    Plane topFace;
    Plane bottomFace;
    Plane rightFace;
    Plane leftFace;
    Plane farFace;
    Plane nearFace;

    // Extracts 6 planes from the View-Projection matrix
    void Update(const glm::mat4& viewProj) {
        // Left Plane
        leftFace.normal.x = viewProj[0][3] + viewProj[0][0];
        leftFace.normal.y = viewProj[1][3] + viewProj[1][0];
        leftFace.normal.z = viewProj[2][3] + viewProj[2][0];
        leftFace.distance = viewProj[3][3] + viewProj[3][0];

        // Right Plane
        rightFace.normal.x = viewProj[0][3] - viewProj[0][0];
        rightFace.normal.y = viewProj[1][3] - viewProj[1][0];
        rightFace.normal.z = viewProj[2][3] - viewProj[2][0];
        rightFace.distance = viewProj[3][3] - viewProj[3][0];

        // Bottom Plane
        bottomFace.normal.x = viewProj[0][3] + viewProj[0][1];
        bottomFace.normal.y = viewProj[1][3] + viewProj[1][1];
        bottomFace.normal.z = viewProj[2][3] + viewProj[2][1];
        bottomFace.distance = viewProj[3][3] + viewProj[3][1];

        // Top Plane
        topFace.normal.x = viewProj[0][3] - viewProj[0][1];
        topFace.normal.y = viewProj[1][3] - viewProj[1][1];
        topFace.normal.z = viewProj[2][3] - viewProj[2][1];
        topFace.distance = viewProj[3][3] - viewProj[3][1];

        // Near Plane
        nearFace.normal.x = viewProj[0][3] + viewProj[0][2];
        nearFace.normal.y = viewProj[1][3] + viewProj[1][2];
        nearFace.normal.z = viewProj[2][3] + viewProj[2][2];
        nearFace.distance = viewProj[3][3] + viewProj[3][2];

        // Far Plane
        farFace.normal.x = viewProj[0][3] - viewProj[0][2];
        farFace.normal.y = viewProj[1][3] - viewProj[1][2];
        farFace.normal.z = viewProj[2][3] - viewProj[2][2];
        farFace.distance = viewProj[3][3] - viewProj[3][2];

        leftFace.Normalize();
        rightFace.Normalize();
        bottomFace.Normalize();
        topFace.Normalize();
        nearFace.Normalize();
        farFace.Normalize();
    }

    // Step 3 Logic: Check if a Chunk's bounding box is inside the frustum
    bool IsBoxVisible(const glm::vec3& min, const glm::vec3& max) const {
        const Plane* planes[6] = { &leftFace, &rightFace, &bottomFace, &topFace, &nearFace, &farFace };

        for (int i = 0; i < 6; i++) {
            // Check if the box is completely behind any of the 6 planes
            if (planes[i]->GetSignedDistance(glm::vec3(min.x, min.y, min.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(max.x, min.y, min.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(min.x, max.y, min.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(max.x, max.y, min.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(min.x, min.y, max.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(max.x, min.y, max.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(min.x, max.y, max.z)) < 0 &&
                planes[i]->GetSignedDistance(glm::vec3(max.x, max.y, max.z)) < 0) 
            {
                return false;
            }
        }
        return true;
    }
};