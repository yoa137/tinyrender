#include "./format/tgaimage.h"

#include "./imgui-master/imgui.h"
#include "./imgui-master/imgui_impl_glfw.h"
#include "./imgui-master/imgui_impl_opengl3.h"

#include "utilities/MalinMath.h"
#include "geometry/shape.h"
#include "model.h"
#include <stdexcept>
#include <limits>


//#include "utilities/format.h"

#include <GLFW/glfw3.h>


void Malin_Render();
GLFWwindow* Malin_Init();
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

int main(int argc, char** argv) {


    GLFWwindow* window = Malin_Init();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ************ GUI **************
        ImGui::ShowDemoWindow();
        Malin_Render();
        // ************ GUI **************

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
	
	//return 0;
}


GLFWwindow* Malin_Init() {

    if (!glfwInit())
        exit(1);

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3", nullptr, nullptr);
    if (window == nullptr)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

//void Malin_Render() {
//
//    int width = 1000;
//    int height = 1000;
//    TGAImage image(width, height, TGAImage::RGB);
//    Line line;
//    Model african("./resource/obj/african_head.obj");
//
//
//    for (int i = 0; i < african.nfaces(); i++) {
//        std::vector<int> face = african.face(i);
//        for (int j = 0; j < 3; j++) {
//            Vec3f v0 = african.vert(face[j]);
//            Vec3f v1 = african.vert(face[(j + 1) % 3]);
//            int x0 = (v0.x + 1.0) * width / 2;
//            int x1 = (v1.x + 1.0) * width / 2;;
//            int y0 = (v0.y + 1.0) * height / 2;;
//            int y1 = (v1.y + 1.0) * height / 2;;
//            line.draw5(x0, y0, x1, y1, image, red);
//        }
//    }
//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    image.write_tga_file("african_line.tga");
//};
//void Malin_Render() {
//
//    int width = 200;
//    int height = 200;
//    TGAImage image(width, height, TGAImage::RGB);
//    Line line;
//    Triangle triangle;
//    Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
//    Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
//    Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
//    triangle.draw2(t0[0], t0[1], t0[2], image, red);
//    triangle.draw3(t0, image, red);
//    triangle.draw3(t1, image, white);
//    triangle.draw3(t2, image, green);
//
//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    image.write_tga_file("triangle.tga");
//};


// z buffer
//void Malin_Render() {
//
//    int width = 1000;
//    int height = 1000;
//    TGAImage image(width, height, TGAImage::RGB);
//    Triangle triangle;
//    Model african("./resource/obj/african_head.obj");
//
//
//
//    int n = width * height;
//    float* zbuffer = new float[n];
//    for (int i = 0; i < n; i++) {
//        zbuffer[i] = -std::numeric_limits<float>::max();
//    }
//    Vec3f lightdir(0, 0, -1);
//
//    for (int i = 0; i < african.nfaces(); i++) {
//        std::vector<int> face = african.face(i);
//        Vec3f screen_coords[3];
//        Vec3f world_coords[3];
//        for (int j = 0; j < 3; j++) {
//            Vec3f v = african.vert(face[j]);
//            screen_coords[j] = Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
//            world_coords[j] = v;
//        }
//        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
//        n.normalize();
//        float intensity = n * lightdir;
//        if (intensity > 0) {
//            triangle.draw(screen_coords, zbuffer,image, TGAColor(intensity * 255, intensity * 0, intensity * 0, 255));
//        }
//        
//    }
//    delete[] zbuffer;
//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    image.write_tga_file("african_triangle_z.tga");
//};

//void Malin_Render() {
//
//    int width = 1000;
//    int height = 1000;
//    TGAImage scene(width, height, TGAImage::RGB);
//    Line line;
//
//    // scene "2d mesh"
//    line.draw(Vec2i(20, 34), Vec2i(744, 400), scene, red);
//    line.draw(Vec2i(120, 434), Vec2i(444, 400), scene, green);
//    line.draw(Vec2i(330, 463), Vec2i(594, 200), scene, blue);
//
//    // screen line
//    line.draw(Vec2i(10, 10), Vec2i(790, 10), scene, white);
//
//    scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    scene.write_tga_file("scene.tga");
//};

// texture
void Malin_Render() {

    int width = 1000;
    int height = 1000;
    TGAImage image(width, height, TGAImage::RGB);
    Triangle triangle;
    Model african("tinyrenderer/tinyrender/resource/obj/african_head.obj");
    TGAImage texture;
    texture.read_tga_file("tinyrenderer/tinyrender/resource/obj/african_head_diffuse.tga");
    texture.flip_vertically();

    
    int n = width * height;
    float* zbuffer = new float[n];
    for (int i = 0; i < n; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    Vec3f lightdir(0, 0, -1);

    for (int i = 0; i < african.nfaces(); i++) {
        std::vector<int> face = african.face(i);
        std::vector<int> textureIdx = african.texture(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        Vec3f texture_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = african.vert(face[j]);
            screen_coords[j] = world2screen(v,width,height);
            world_coords[j] = v;
           // std::clog << textureIdx[j] << "\n";
            texture_coords[j] = african.tex(textureIdx[j]-1);
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * lightdir;
        if (intensity > 0) {
            triangle.drawTex(screen_coords, zbuffer, texture_coords, texture,image, intensity);
        }


    }
    delete[] zbuffer;
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("african_triangle_texture.tga");
};