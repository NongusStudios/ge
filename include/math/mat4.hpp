#pragma once

#include <cstddef>
#include <array>

#include "vec2.hpp"
#include "vec3.hpp"

namespace ge::math {
    struct Mat4 {
        float mat[4][4];
        
        
        static Mat4 zero(){
            return {};
        }
        
        static Mat4 identity(){
            return Mat4{{
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}}
            };
        } 
        static Mat4 perspective(float fov, float aspect, float min_depth, float max_depth){

        }
        static Mat4 ortho(const Vec2& top_bottom, const Vec2& left_right, float min_depth, float max_depth){

        }

        void transform(const Vec3& pos){
            auto m = identity();
            m.mat[0][3] = pos.x;
            m.mat[1][3] = pos.y;
            m.mat[2][3] = pos.z;
            
        }
        void rotate(float rads, const Vec3& axis){

        }
        void scale(const Vec3& scale){

        }

        Mat4 operator + (const Mat4& m) const {
            Mat4 a = {};
            for(size_t i = 0; i < 4; i++){
                for(size_t j = 0; j < 4; j++){
                    a.mat[i][j] = mat[i][j] + m.mat[i][j];
                }
            }
            return a;
        }

        Mat4& operator += (const Mat4& m){
            *this = *this + m;
            return *this;
        }

        Mat4 operator * (float scalar) const {
            Mat4 a = {};
            for(size_t i = 0; i < 4; i++){
                for(size_t j = 0; j < 4; j++){
                    a.mat[i][j] = mat[i][j] * scalar;
                }
            }
            return a;
        }

        Mat4& operator *= (float scalar){
            *this = *this * scalar;
            return *this;
        }

        Mat4 operator * (const Mat4& m) const {
            Mat4 a = {};
            for(size_t i = 0; i < 4; i++){
                for(size_t j = 0; j < 4; j++){
                    for(size_t k = 0; k < 4; k++){
                        a.mat[i][j] += mat[i][k] * m.mat[k][j];
                    } 
                }
            }
            return a;
        }

        Mat4& operator *= (const Mat4& m){
            *this = *this * m;
            return *this;
        }
    };
}
