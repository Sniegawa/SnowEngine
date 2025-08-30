#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "Engine/SnowEngineAPI.h"
#include "Engine/Core/Application.h"

#include "Engine/Core/Logging/Log.h"

#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"

#include "Engine/Core/IWindow.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/Layers/Layer.h"
#include "Engine/Core/ImGui/ImGuiLayer.h"

#include "Engine/Core/Input.h"

#include "Engine/Core/Renderer/Shader.h"
#include "Engine/Core/Renderer/VertexArray.h"
#include "Engine/Core/Renderer/Buffer.h"
#include "Engine/Core/Renderer/Camera/OrthographicCamera.h"
#include "Engine/Core/Renderer/Camera/OrthographicCameraController.h"
#include "Engine/Core/Renderer/Texture.h"
#include "Engine/Core/Renderer/Spritesheet.h"

#include "Engine/Core/Renderer/Renderer.h"
#include "Engine/Core/Renderer/RenderCommand.h"
#include "Engine/Core/Renderer/RendererAPI.h"
#include "Engine/Core/Renderer/Renderer2D.h"
#include "Engine/Core/Renderer/Framebuffer.h"

#include "Engine/Core/Audio/AudioSystem.h"
#include "Engine/Core/Audio/SoundInstance.h"
#include "Engine/Core/Audio/AudioAssets.h"