#pragma once
#include "./format/tgaimage.h"
#include "../utilities/MalinMath.h"
#include <iostream>
using namespace Malin;
class Line {
public:
    Line() {}
    void draw1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
        for (float t = 0.; t < 1.; t += .01) {
            int x = x0 + (x1 - x0) * t;
            int y = y0 + (y1 - y0) * t;
            image.set(x, y, color);
        }
    }
    void draw2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
        for (int x = x0; x <= x1; x++) {
            float t = (x - x0) / float(x1 - x0);
            int y = y0 * (1. - t) + y1 * t;
            image.set(x, y, color);
        }

    }
    void draw3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x = x0; x <= x1; x++) {
            float t = (x - x0) / float(x1 - x0);
            int y = y0 * (1. - t) + y1 * t;
            if (steep) {
                image.set(y, x, color);
            }
            else {
                image.set(x, y, color);
            }

        }
    }

    void draw4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        float derror = std::abs(dy / (float(dx)));
        float error = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++) {
            if (steep) {
                image.set(y, x, color);
            }
            else {
                image.set(x, y, color);
            }
            error += derror;
            if (error > 0.5) {
                y += (y1 > y0 ? 1 : -1);
                error -= 1.;
            }

        }
    }

    void draw5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        float derror2 = std::abs(dy) * 2;
        float error2 = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++) {
            if (steep) {
                image.set(y, x, color);
            }
            else {
                image.set(x, y, color);
            }
            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= 2 * dx;
            }

        }
    }
    void draw(Vec2i u, Vec2i v, TGAImage& image, TGAColor color) {
        int x0 = u.x;
        int y0 = u.y;
        int x1 = v.x;
        int y1 = v.y;
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1 - x0;
        int dy = y1 - y0;
        float derror2 = std::abs(dy) * 2;
        float error2 = 0;
        int y = y0;
        for (int x = x0; x <= x1; x++) {
            if (steep) {
                image.set(y, x, color);
            }
            else {
                image.set(x, y, color);
            }
            error2 += derror2;
            if (error2 > dx) {
                y += (y1 > y0 ? 1 : -1);
                error2 -= 2 * dx;
            }

        }
    }
};

class Triangle {
public:
    void draw(Vec3f* pts, float *zbuffer,TGAImage& image, TGAColor color) {
        Vec2f bboxmax(0, 0);
        Vec2f bboxmin(image.get_width() - 1, image.get_height() - 1);
        Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

        for (int i = 0; i < 3; i++) {

            bboxmax.x = std::min(clamp[0], std::max(bboxmax[0], pts[i][0]));
            bboxmax.y = std::min(clamp[1], std::max(bboxmax[1], pts[i][1]));

            bboxmin.x = std::max(0.f, std::min(bboxmin[0], pts[i][0]));
            bboxmin.y = std::max(0.f, std::min(bboxmin[1], pts[i][1]));

        }
        Vec3f P;
        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
                Vec3f bc = barycentic(pts, P);
                if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
                P.z = 0;
                for (int i = 0; i < 3; i++) {
                    P.z += pts[i][2] * bc[i];
                }

                int zbIdx = P.x + image.get_width() * P.y;
                if (zbuffer[zbIdx] < P.z) {
                    zbuffer[zbIdx] = P.z;
                    image.set(P.x, P.y, color);;
                }

                
            }
        }
    }
    Vec3f barycentic(Vec3f* pts, Vec3f P) {
        Vec3f u = Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]) ^ Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);
        if (std::abs(u.z) > 1e-2) {
            return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
        }
        return Vec3f(-1, 1, 1);

    }
    void drawTex(Vec3f* pts, float* zbuffer, Vec3f* tpts, TGAImage& texture ,TGAImage& image,float intensity) {
        Vec2f bboxmax(0, 0);
        Vec2f bboxmin(image.get_width() - 1, image.get_height() - 1);
        Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

        for (int i = 0; i < 3; i++) {

            bboxmax.x = std::min(clamp[0], std::max(bboxmax[0], pts[i][0]));
            bboxmax.y = std::min(clamp[1], std::max(bboxmax[1], pts[i][1]));

            bboxmin.x = std::max(0.f, std::min(bboxmin[0], pts[i][0]));
            bboxmin.y = std::max(0.f, std::min(bboxmin[1], pts[i][1]));

        }
        Vec3f P;
        for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
            for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
                Vec3f bc = barycentic(pts, P);
                if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
                P.z = 0;
                Vec3f UV(0.0,0.0,0.0);
                for (int i = 0; i < 3; i++) {
                    P.z += pts[i][2] * bc[i];
                    UV = UV + tpts[i] * bc[i];
                }

                int zbIdx = P.x + image.get_width() * P.y;
                if (zbuffer[zbIdx] < P.z) {
                    zbuffer[zbIdx] = P.z;
                    image.set(P.x, P.y, texture.get(int(UV.x*texture.get_width()), int(UV.y*texture.get_height()) ));
                }
                //

            }
        }
    }
    void draw1(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
        if (t0.y == t1.y && t0.y == t2.y) return;
        if (t0.y > t1.y) std::swap(t1, t0);
        if (t0.y > t2.y) std::swap(t0, t2);
        if (t1.y > t2.y) std::swap(t1, t2);
        
        for (int y = t0.y; y <= t1.y; y++) {
            float alpha = (float)(y - t0.y) / (t2.y - t0.y);
            float beta = (float)(y - t0.y) / (t1.y - t0.y + 1);   // ·ÀÖ¹³ý 0
            int xl = t0.x + alpha * (t2.x - t0.x);
            int xr = t0.x + beta* (t1.x - t0.x); 
            if (xl > xr) std::swap(xl, xr);
            for (int x = xl; x < xr; x++) {
                image.set(x, y, color);
                image.set(x, y, color);
            }
            
        }

        for (int y = t1.y; y <= t2.y; y++) {
            float alpha = (float)(y - t0.y) / (t2.y - t0.y);
            float beta = (float)(y - t1.y) / (t2.y - t1.y +1 );
            int xl = t0.x + alpha * (t2.x - t0.x);
            int xr = t1.x + beta * (t2.x - t1.x);
            if (xl > xr) std::swap(xl, xr);
            for (int x = xl; x < xr; x++) {
                image.set(x, y, color);
                image.set(x, y, color);
            }

        }
    }

    void draw2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
        if (t0.y == t1.y && t0.y == t2.y) return;
        if (t0.y > t1.y) std::swap(t1, t0);
        if (t0.y > t2.y) std::swap(t0, t2);
        if (t1.y > t2.y) std::swap(t1, t2);

        for (int y = t0.y; y <= t2.y; y++) {
            bool ishalf = y >= t1.y;
            float alpha = (float)(y - t0.y) / (t2.y - t0.y);
            float beta = ishalf? (float)(y - t1.y) / (t2.y - t1.y) :(float)(y - t0.y) / (t1.y - t0.y );   // ·ÀÖ¹³ý 0
            int xl = t0.x + alpha * (t2.x - t0.x);
            int xr = ishalf ? t1.x + beta * (t2.x - t1.x) :t0.x + beta * (t1.x - t0.x);
            if (xl > xr) std::swap(xl, xr);
            for (int x = xl; x < xr; x++) {
                image.set(x, y, color);
                image.set(x, y, color);
            }

        }
    }
    
    Vec3f barycentic(Vec2i* pts, Vec2i P) {
        Vec3f u = Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]) ^ Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);
        if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
        return Vec3f(1.f - (u.x + u.y) /u.z, u.y / u.z, u.x / u.z);

    }
    void draw3(Vec2i *pts, TGAImage& image, TGAColor color) {
        Vec2i bboxmax(0, 0);
        Vec2i bboxmin(image.get_width() - 1, image.get_height()-1);
        Vec2i clamp(image.get_width() - 1, image.get_height()-1);

        for (int i = 0; i < 3; i++) {
            bboxmax.x = std::min(clamp[0], std::max(bboxmax[0], pts[i][0]));
            bboxmax.y = std::min(clamp[1], std::max(bboxmax[1], pts[i][1]));

            bboxmin.x = std::max(0, std::min(bboxmin[0], pts[i][0]));
            bboxmin.y = std::max(0, std::min(bboxmin[1], pts[i][1]));

        }

        for (int i = bboxmin.x; i <= bboxmax.x; i++) {
            for (int j = bboxmin.y; j <= bboxmax.y; j++) {
                Vec3f u = barycentic(pts, Vec2i(i, j));
                if (u.x < 0 || u.y < 0 || u.z < 0) continue;
                image.set(i, j, color);
            }
        }
    }
    
};

Vec3f world2screen(Vec3f v, int width, int height) {
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);

}
