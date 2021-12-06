#pragma once
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"


void Imgui_Start(GLFWwindow* window);
void Imgui_Close();
void Imgui_NewFrame();
void Imgui_Render();