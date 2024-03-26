#pragma once

#include "vec2.hpp"
#include "vec3.hpp"

namespace ge::math {
    struct Mat4 {
        float mat[4][4];
        
        static Mat4 identity(){

        } 
        static Mat4 perspective(float fov, float aspect, float min_depth, float max_depth){

        }
        static Mat4 ortho(const Vec2& top_bottom, const Vec2& left_right, float min_depth, float max_depth){

        }

        void transform(const Vec3& pos){

        }
        void rotate(float rads, const Vec3& axis){

        }
        void scale(const Vec3& scale){

        }
    };
}
